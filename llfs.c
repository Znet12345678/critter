#include <mem.h>
extern int strncmp(const char *str1,const char *str2,unsigned int n);
#include <lib.h>
#include <llfs.h>
int llfs_ata_read_master(void *buf, uint32_t lba,uint16_t drive,uint8_t slave){
	return ata_read_master(buf,lba+*(uint32_t*)0x103,drive,slave);
}
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
	char **ret = malloc(sizeof(*ret)*(strlen(dir)+1));
	int size = 0;
	ret[0] = malloc(1024);
	bzero(ret[0],strlen(dir));
	while(i < strlen(dir)){
		if(dir[i] == c){
			ret[k][j] = 0;
			size+=j;
			j = 0;
			k++;
			ret[k] = malloc(strlen(dir)-size+1);
			bzero(ret[k],strlen(dir)-size+1);
			i++;
			continue;
		}
		ret[k][j] = dir[i];
		j++;
		i++;
	}
	k++;
	ret[k] = 0;
	return ret;
}

struct Entry *__opendir(const char *dir){
#ifdef __FS_DEBUG
	puts("Opening directory:");
	puts(dir);
	puts("\n");
#endif
	char *buf = malloc(512);
	struct Entry *ent = malloc(sizeof(*ent));
	bzero(buf,512);
	llfs_ata_read_master(buf,0,*(uint16_t*)0x100,*(uint8_t*)0x102);
	bzero(ent,sizeof(*ent));
	char **dirs = sep(dir,'/');
	memcpy(ent,buf,sizeof(*ent));
	if(strcmp(dir,"") == 0)
		return ent;
	llfs_ata_read_master(buf,ent->nxtlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
	int i = 0;

a:;while(1){
		memcpy(ent,buf,sizeof(*ent));
		if(strncmp(buf +sizeof(*ent),dirs[i],strlen(dirs[i])) == 0){
			break;
		}
		else if(ent->nsize == 0)
			if(strncmp(dir,"\0",1) == 0)
				break;
	
		if(ent->contlba == 0){
			puts("No such directory!\n");
			return 0;
  		}
		llfs_ata_read_master(buf,ent->contlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
	  }
	i++;
	if(dirs[i] == 0){
#ifdef __FS_DEBUG
		puts("Opened directory:");
		puts(dir);
		puts("\n");
#endif
		return ent;
	}
	llfs_ata_read_master(buf,ent->nxtlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
	goto a;	
}
struct llfd *getAddressSpace(){
	if(*(uint8_t*)0x200 == 1){
		return (struct llfd*)*(uint32_t*)0x201;
	}
	*(uint8_t *)(0x200) = 1;
	*(uint32_t *)(0x201) = (uint32_t)malloc(0x1000);
	struct llfd *pntr = (struct llfd*)*(uint32_t*)0x201;
	pntr->pntr = 0;
	pntr->nxt = malloc(sizeof(*pntr->nxt));
	pntr->t = O_RDONLY;
	pntr->nxt->t = O_WRONLY;
	pntr->nxt->pntr = 0;
	pntr->nxt->nxt = 0;
	return (struct llfd*)*(uint32_t*)0x201;
}
struct llfd *getllfd(int llfd){
	struct llfd *pntr = getAddressSpace();
	for(int i = 0; i < llfd;i++)
		pntr = pntr->nxt;
	return pntr;
}
struct llfd *allocllfd(){
	struct llfd *pntr = getAddressSpace();
	int i = 0;
	while(pntr->nxt != 0){
		pntr = pntr->nxt;
	}
	pntr->nxt = malloc(sizeof(*pntr->nxt));
	bzero(pntr->nxt,sizeof(*pntr->nxt));
	return pntr->nxt;
}
int getLLFD(struct llfd *l){
	struct llfd *pntr = getAddressSpace();
	int i = 0;
	while(pntr->nxt != 0){
		if(pntr == l)
		       return i;
		pntr = pntr->nxt;
		i++;
	}
	return i;
}
char *substring(char *a, int b,int c){
	int i = b;
	char *ret = malloc(c-b+1);
	bzero(ret,c-b+1);
	for(i = b; i < c;i++)
		ret[i-b] = a[i];
	return ret;

}
void strip(char *a){
	int i = strlen(a)-1;
	while(a[i] == '/')
		i--;
	a[i+1] = 0;
}
int finc(const char *str,char c){
	for(int i = strlen(str)-1; i >= 0;i--)
		if(str[i] == c)
			return i;
	return -1;
}
char* fins(const char *str,char c){
	int i = strlen(str)-1;
	char *ret = malloc(strlen(str)-finc(str,c));
	int indx = strlen(str)-2;
	while(i >= 0){
		if(str[i] == c)
			break;
		ret[indx] = str[i];
		indx--;
		i--;
	}
	return ret;
}
int open(const char *_file,int options){
	char *file = malloc(strlen(_file));
	bzero(file,strlen(_file));
	int i = 0;
	while(_file[i] == '/')
		i++;
	strcpy(file,substring((char*)_file,i,strlen(_file)));
	uint8_t *buf = malloc(513);
	bzero(buf,513);
	struct Entry *ent = __opendir(substring(file,0,finc(file,'/')));
	if(ent == 0)
		return -1;
	llfs_ata_read_master(buf,ent->nxtlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
	ent =(struct Entry *) buf;
	int lba= 0;
	char *filen = substring(file,finc(file,'/')+1,strlen(file));
	while(1){
		if(strcmp(buf + sizeof(*ent),filen) == 0){
			struct llfd *llfd = allocllfd();
			llfd->pntr = (struct Entry *)buf;
			llfd->t = options;
			llfd->pntr->contlba = lba;
			return getLLFD(llfd);	
		}
		if(ent->contlba == 0){
			puts("No such file or directory!\n");
			return -1;
		}
		lba=  ent->contlba;
		llfs_ata_read_master(buf,ent->contlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
		ent = (struct Entry *)buf;
	}
	struct llfd *llfd = allocllfd();
	llfs_ata_read_master(buf,ent->contlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
	llfd->pntr = (struct Entry *)buf;
	llfd->nxt = 0;
	llfd->t = options;
	return getLLFD(llfd);	
}
int abs(int n){
	return n < 0 ? n*-1 : n;
}
int llread(int fd,char *buf,unsigned int n){
	if(fd < 0)
		return -1;
	char *tbuf = malloc(1024);
	struct llfd *llfd = getllfd(fd);
	int j = 0;
	struct Entry *ent = malloc(sizeof(*ent));
	memcpy(ent,llfd->pntr,sizeof(*llfd->pntr));	
//	puti(llfd->pntr->contlba);
	while(j < n){
		bzero(tbuf,1024);
		uint32_t sz = ((j + 512-sizeof(*ent)-ent->nsize) > n) ? abs(n-j) : 512-sizeof(*ent)-ent->nsize-1 ;
		llfs_ata_read_master(tbuf,ent->contlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
		memcpy(ent,tbuf,sizeof(*ent));
		memcpy(buf + j,tbuf + sizeof(*ent) + ent->nsize+1,sz);
//		putx(j);
//		puts("\n");

		if(ent->contlba == 0)
			return j;


		j+=sz;
	}
	free(tbuf);
	return j;
}
int fsize(int fd){
	if(fd < 0)
		return -1;
	char *tbuf = malloc(1024);
	struct llfd *llfd = getllfd(fd);
	int size = 0;
	struct Entry *ent = malloc(sizeof(*ent));
	memcpy(ent,llfd->pntr,sizeof(*llfd->pntr));
	return ent->size;
}
