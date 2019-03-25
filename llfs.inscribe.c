#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <llfs.h>
#include <dirent.h>
#include <sys/types.h>
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
int getLastIndx(char *pntr,char c);
int _mkdir(char *_d,FILE *f){
	char *d = malloc(1024);
	strcpy(d,_d);
	d[strlen(d)-1] = d[strlen(d)-1]=='/' ? 0 : d[strlen(d)-1];
	char *d2 = malloc(1024);
	strcpy(d2,d);
	struct Entry *node = malloc(512);
	readDisk(f,0,node);
	char *tok = malloc(1024);
	bzero(tok,1024);
	int i = 0;
	char *alloc = malloc(512);
	alloc[0] = 3;
a:;	if(strcmp(_d,"/") == 0 || tok == NULL){
		 goto b;
  	}
	readDisk(f,node->nxtlba,node);
	while(node->contlba != 0 && (strcmp((char*)(node + sizeof(*node)),tok) != 0 && node->type != TYPE_DIR)){
		printf("%s\n",node + sizeof(*node));
		readDisk(f,node->contlba,node);
	}
	if(countc(_d,'/') <= 1)
		goto b;
	if(node->contlba == 0 && strcmp(d,"/") != 0 || node->type != TYPE_DIR){
		return 0;
	}
	if(0){
b:;		uint8_t *buf = malloc(512);
		struct Entry *hdr = malloc(512);
		int sv2;
		while(hdr->contlba != 0){
			sv2 = hdr->contlba;
			readDisk(f,hdr->contlba,hdr);
		}
		node->contlba = allocSector(f);
		writeDisk(f,sv2,hdr);
		int sv = node->contlba;
		char *tmp = subpntr(d2,getLastIndx(d2,'/')+1,strlen(d2));
		memcpy(buf + sizeof(*hdr),tmp,strlen(tmp));
		writeDisk(f,node->contlba,buf);
		hdr->contlba = allocSector(f);
		hdr->nxtlba = allocSector(f);
		hdr->type = TYPE_DIR;
		memcpy(buf,hdr,sizeof(*hdr));
		writeDisk(f,sv,buf);
		int lba = hdr->nxtlba;
		hdr->nxtlba = lba;
		hdr->contlba = allocSector(f);
		bzero(buf,512);
		memcpy(buf,hdr,sizeof(*hdr));
		memcpy(buf + sizeof(*hdr),".",strlen("."));
		writeDisk(f,lba,buf);
		lba = hdr->contlba;
		hdr->nxtlba = hdr->contlba;
		hdr->contlba = allocSector(f);
		bzero(buf,512);
		memcpy(buf,hdr,sizeof(*hdr));
		memcpy(buf + sizeof(*hdr),"..",strlen(".."));
		writeDisk(f,lba,buf);
		free(buf);
		free(hdr);
		free(node);
		return 1;
	}
	tok = strtok(strcmp(tok,"") == 0 ? d : NULL,"/");
	i++;
	goto a;
		
}
int _opendir(char *d,FILE *f){
	struct Entry *node = malloc(512);
	readDisk(f,0,node);
	char *tok = strtok(d,"/");
a:;readDisk(f,node->nxtlba,node);
	int lba = node->nxtlba;
	if(tok == NULL)
		goto b;
	while(node->contlba != 0 && strcmp((char*)(node + sizeof(*node)),tok) != 0 && node->type != TYPE_DIR){
		lba = node->contlba;
		readDisk(f,node->contlba,node);	

	}
	if(node->contlba == 0 || node->type != TYPE_DIR){
		free(node);
		return 0;
	}
	if(0){
		while(node->contlba != 0)
			readDisk(f,node->contlba,node);
b:;		return lba;
	}
	tok = strtok(NULL,"/");
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
	ent->nxtlba = allocSector(f);
	writeDisk(f,lba,ent);
	readDisk(f,ent->nxtlba,ent);
	char *buf = malloc(512);
	int twritten = 0;
	b:;readDisk(f,lba,buf);
	ent->contlba = allocSector(f);
	ent->type = TYPE_FILE;
	bzero(buf,512);
	char *name = subpntr(n,getLastIndx(n,'/')+1,strlen(n));
	memcpy(buf+sizeof(*ent),name,strlen(name));
	ent->nsize = strlen(name);
	char c;
	int written = 0;
	char *tbuf = malloc(512-sizeof(*ent)-strlen(name));
	int i = 0;
	while(written != 512-sizeof(*ent)-strlen(name) && !feof(in)){
		tbuf[i] = fgetc(in);
		i++;written++;twritten++;
	}
	memcpy(buf+sizeof(*ent)+strlen(name),tbuf,512-sizeof(*ent)-strlen(name));
	writeDisk(f,lba,buf);
	if(feof(in)){
		return twritten;
	}
	lba = ent->contlba;
	free(name);
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
	while(path[i] == '/')
		i++;
	i++;
	while(path[i] != '/')
		i++;
	i++;
	int j = 0;
	while(path[i] != 0){
		path2[j] = path[i];
		j++;
		i++;
	}	
	return _recurse(path,"",f);
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
