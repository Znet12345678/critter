#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <llfs.h>
#include <dirent.h>
#include <sys/types.h>
int countc(const char *str,char c);
char **sep(const char *dir,char c){
        int i = 0;
        int j = 0;
        int k = 0;
        while(dir[i] == c)
                i++;
        if(strcmp(dir,"") == 0){
                char **arr = malloc(sizeof(char**));
                arr[0] = "";
                return arr;
        }
        char **ret = malloc(sizeof(*ret)*countc(dir,c));
        int size = 0;
	ret[0] = malloc(strlen(dir)+1);
        bzero(ret[0],strlen(dir)+1);
        while(i < strlen(dir)){
                if(dir[i] == c){
                        size+=j;
                        j = 0;
			k++;
                        ret[k] = malloc(strlen(dir)-size+1);
                        bzero(ret[k],strlen(dir)-size+1);
                        i++;
                        continue;
                }
                ret[k][j] = dir[i];
                i++;
		j++;
        }
        k++;
        ret[k] = 0;
        return ret;
}

int countc(const char *str,char c){
	int i = 0, ret = 0;
	while(str[i] != 0){
		if(str[i] == c)
			ret++;
		i++;
	}
	return ret;
}
void *subpntr(void *pntr,size_t a,size_t b);
void readDisk(FILE *f,int lba,void *buf){
	fseek(f,lba*512,SEEK_SET);
	fread(buf,1,512,f);
}
void writeDisk(FILE *f,int lba,void *buf){
	fseek(f,lba*512,SEEK_SET);
	fwrite(buf,1,512,f);
}
int allocSector(FILE *f){
	char *buf = malloc(512);
	char *zbuf = malloc(512);
	bzero(zbuf,512);
	int i = 0;
	fseek(f,0,SEEK_SET);
	while(fread(buf,1,512,f) != 0){
		if(memcmp(buf,zbuf,512) == 0){
			struct Entry *hdr = malloc(512);
			bzero(hdr,512);
			hdr->type = TYPE_EMPTY;
			writeDisk(f,i,hdr);
			free(hdr);
			return i;
		}
		i++;
	}
	return -1;
}
int _opendir(char *d,FILE *f);
int getLastIndx(char *pntr,char c);
int _mkdir(char *_d,FILE *f){
	uint8_t *buf = malloc(512);
	int lba = _opendir(subpntr(_d,0,getLastIndx(_d,'/')),f);	
	struct Entry *ent = malloc(sizeof(*ent));
	readDisk(f,lba,buf);
	memcpy(ent,buf,sizeof(*ent));
	lba = ent->contlba;
	int sv = ent->contlba;
	while(ent->contlba != 0){
		lba = ent->contlba;
		readDisk(f,ent->contlba,buf);
		memcpy(ent,buf,sizeof(*ent));
	}
	ent->contlba = allocSector(f);
	memcpy(buf,ent,sizeof(*ent));
	if(strcmp(_d,"/") != 0)
		writeDisk(f,lba,buf);
	lba = ent->contlba;
	ent = malloc(sizeof(*ent));
	ent->type = TYPE_DIR;
	ent->nxtlba = allocSector(f);
	ent->contlba = 0;

	char *name = subpntr(_d,getLastIndx(_d,'/')+1,strlen(_d));
	ent->nsize = strlen(name);
	memcpy(buf,ent,sizeof(*ent)+1);
	memcpy(buf + sizeof(*ent),name,strlen(name)+1);
	writeDisk(f,lba,buf);
	bzero(buf,512);
	lba = ent->nxtlba;
	ent->nsize = 1;
	ent->contlba = allocSector(f);
	memcpy(buf,ent,sizeof(*ent));
	memcpy(buf + sizeof(*ent),".\0",2);
	writeDisk(f,lba,buf);
	bzero(buf,512);
	lba = ent->contlba;
	ent->nsize = 2;
	ent->nxtlba = sv;
	ent->contlba = 0;
	memcpy(buf,ent,sizeof(*ent));
	memcpy(buf+sizeof(*ent),"..\0",3);
	writeDisk(f,lba,buf);
	return 1;
}
int  _opendir(char *dir,FILE *f){
	char *buf = malloc(512);
        struct Entry *ent = malloc(sizeof(*ent));
   	bzero(ent,sizeof(*ent));
   	bzero(buf,512);
	if(feof(f))
		return 0;
	readDisk(f,0,buf);
     	char **dirs = sep(dir,'/');
	memcpy(ent,buf,sizeof(*ent));
	if(strcmp(dir,"") == 0)
		return ent->nxtlba;
	readDisk(f,ent->nxtlba,buf);
	int i = 0;
a:;while(1){
        	if(i > countc(dir,'/'))
			return -1;
		memcpy(ent,buf,sizeof(*ent));
                if(strncmp(buf +sizeof(*ent),dirs[i],strlen(dirs[i])) == 0)
                        break;
                else if(ent->nsize == 0)
                        if(strncmp(dir,"\0",1) == 0)
                                break;
                if(ent->contlba == 0)
                        return 0;
               	readDisk(f,ent->contlba,buf);
   
        }
  	i++;
        if(dirs[i] == 0){
                return ent->nxtlba;
        }
	readDisk(f,ent->nxtlba,buf);
	goto a; 

}
int getLastIndx(char *str,char c){
	int i = 0;
	int sv = -1;
	while(str[i] != 0){
		if(str[i] == c)
			sv = i;	
		i++;
	}
	return sv;
}
void *subpntr(void *pntr,size_t a,size_t b){

	void *ret = malloc(b-a+1);
	bzero(ret,b-a+1);
	for(size_t i = a; i < b;i++)
		*(uint8_t*)(ret+i-a) = *(uint8_t*)(pntr+i);
	return ret;

}
int writefile(char *n,FILE *in,FILE *f){
	n[strlen(n)-1] = n[strlen(n)-1] == '/' ? 0 : n[strlen(n)-1];
	char *sub = subpntr(n,0,getLastIndx(n,'/'));
	int lba = _opendir(sub,f);
	if(lba < 0){
		return -1;
	}

	struct Entry *ent = malloc(512);
	readDisk(f,lba,ent);
	while(1){
		if(ent->contlba == 0)
			break;
		lba = ent->contlba;
		readDisk(f,ent->contlba,ent);
		
	}
	ent->contlba = allocSector(f);
	writeDisk(f,lba,ent);
	lba = ent->contlba;
	char *name = subpntr(n,getLastIndx(n,'/')+1,strlen(n));
	ent->nsize = strlen(name);
	ent->contlba = allocSector(f);
	char *tpntr = ent;
	fseek(in,0,SEEK_END);
	int size = ftell(in);
	fseek(in,0,SEEK_SET);
	ent->size = size;
	memcpy((char*)(tpntr + sizeof(*ent)),name,strlen(name)+1);
	writeDisk(f,lba,tpntr);
	//lba = ent->contlba;
	char *buf = malloc(512);
	int twritten = 0;
	b:;readDisk(f,lba,buf);
	ent->contlba = allocSector(f);
	ent->type = TYPE_FILE;
	ent->size = size;
	memcpy(buf,ent,sizeof(*ent));
	memcpy(buf+sizeof(*ent),name,strlen(name)+1);
	ent->nsize = strlen(name);
	char c;
	int written = 0;
	char *tbuf = malloc(512-sizeof(*ent)-strlen(name)-1);
	int i = 0;
	while(written != 512-sizeof(*ent)-strlen(name)-1 && !feof(in)){
		tbuf[i] = fgetc(in);
		i++;written++;twritten++;
	}
	memcpy(buf+sizeof(*ent)+strlen(name)+1,tbuf,512-sizeof(*ent)-strlen(name)-1);

	writeDisk(f,lba,buf);

	if(feof(in)){
		return twritten;
	}
	lba = ent->contlba;
	free(tbuf);
	bzero(buf,512);
	goto b;
}
void usage(char *name){
	printf("Usage %s img relative_path_to_dir\n",name);
}
int recurse(const char *path,FILE *f){
	char *path2 = malloc(1024);
	bzero(path2,1024);
	int i = 0;
	while(path[i] != 0 && path[i] == '/')
		i++;
	i++;
	while(path[i] != 0 && path[i] == '/')
		i++;
	i++;

	int j = 0;
	while(path[i] != 0){
		path2[j] = path[i];
		j++;
		i++;
	}	
	return _recurse(path,path2,f);
}
int _recurse(const char *path,const char *root,FILE *f){
	DIR *d = opendir(path);
	if(!d)
		return -1;
	struct dirent *ent;
	while((ent = readdir(d)) != NULL){
		if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0)
			continue;
		char *rpath = malloc(1024);
		sprintf(rpath,"%s/%s",root,ent->d_name);
		if(ent->d_type == DT_DIR){
			int ret = _mkdir(rpath,f);
		
		}
	}
	closedir(d);
	d = opendir(path);
	while((ent = readdir(d)) != NULL){
		char *rpath = malloc(1024);
		bzero(rpath,1024);
		sprintf(rpath,"%s/%s",root,ent->d_name);
		if(ent->d_type == DT_REG){
			char *rrpath = malloc(1024);
			sprintf(rrpath,"%s/%s",path,ent->d_name);
			FILE *in = fopen(rrpath,"rb");
			if(!in){
				perror("Failed to open file");
				return -1;
			}

			int ret = writefile(rpath,in,f);
			fclose(in);
		}
		free(rpath);
	}
	closedir(d);
	d = opendir(path);
	ent = 0;
	while((ent = readdir(d)) != NULL){
		if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0)
			continue;
		char *rpath = malloc(1024);
		sprintf(rpath,"%s/%s",path,ent->d_name);

		if(ent->d_type == DT_DIR)
			recurse(rpath,f);
		free(rpath);
	}
	return 0;
}
int main(int argc,char *argv[]){

	if(argc < 3){
		usage(argv[0]);
		return -1;
	}
	FILE *in = fopen(argv[1],"r+b");

	if(!in)
		return -1;
	fseek(in,0,SEEK_END);
	int size = ftell(in);
	fseek(in,0,SEEK_SET);
	for(int i = 0; i < size;i++)
		putc(0,in);
	fseek(in,0,SEEK_SET);
	_mkdir("/",in);
	int v = recurse(argv[2],in);
	fclose(in);
	return v;
}
