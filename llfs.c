#include <mem.h>
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
	char **ret = malloc(sizeof(*ret)*(strlen(dir)+1));
	int size = 0;
	while(i < strlen(dir)){
		if(dir[i] == c){
			size+=j;
			j = 0;
			k++;
			ret[k] = malloc(strlen(dir)-size);
			i++;
			continue;
		}
		ret[k][j] = dir[i];
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
	puts("Exec\n");
	bzero(ent,sizeof(*ent));
	char **dirs = sep(dir,'/');
	int i = 0;
a:;while(1){
		memcpy(ent,buf,sizeof(*ent));
		puts(buf + sizeof(*ent));
		if(strcmp(buf +sizeof(*ent),dirs[i]) == 0)
			break;
		if(ent->contlba == 0)
			return 0;
		llfs_ata_read_master(buf,ent->contlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
		i++;
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
	goto a;	
}
struct llfd *getAddressSpace(){
	if(*(uint8_t*)0x200 == 1){
		return (struct llfd*)*(uint32_t*)0x201;
	}
	*(uint8_t *)(0x200) = 1;
	*(uint32_t *)(0x201) = (uint32_t)malloc(1024000);
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
	while(i >= 0){
		if(str[i] == c)
			break;
		ret[strlen(str)-1-i] = str[i];
		i--;
	}
	return ret;
}
int open(const char *_file,int options){
	char *file = malloc(strlen(_file)+1);
	bzero(file,strlen(_file)+1);
	strcpy(file,_file);
	strip(file);
	uint8_t *buf = malloc(512);
	struct Entry *ent = __opendir(substring(file,0,finc(file,'/')));
	llfs_ata_read_master(buf,ent->nxtlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
	ent =(struct Entry *) buf;
	while(1){
		if(strcmp(buf + sizeof(*ent),fins(file,'/')) == 0){
			struct llfd *llfd = allocllfd();
			llfd->pntr = (struct Entry *)ent;
			llfd->t = options;
			return getLLFD(llfd);		
		}
		llfs_ata_read_master(buf,ent->nxtlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
		if(ent->nxtlba == 0)
			break;
		ent = (struct Entry *)buf;
	}
	if(options == O_WRONLY || options == O_RDWR){
		struct llfd *llfd = allocllfd();
		llfs_ata_read_master(buf,ent->contlba,*(uint16_t*)0x100,*(uint8_t*)0x102);
		llfd->pntr = (struct Entry *)buf;
		llfd->nxt = 0;
		llfd->t = options;
		return getLLFD(llfd);
	}
	return -1;
	
}
int llread(int fd,char *buf,unsigned int n){
	if(fd < 0)
		return -1;
	struct llfd *llfd = getllfd(fd);
	int i = llfd->pntr->contlba, j = 0;
	while(j < n){
		uint8_t *tbuf = malloc(512);
		uint8_t sz = 512-sizeof(*llfd->pntr)-llfd->pntr->nsize;
		llfs_ata_read_master(tbuf,llfd->pntr->nxtlba,*(uint16_t*)0x100,*(uint8_t*)0x102);

		memcpy(buf + j,tbuf + sizeof(*llfd->pntr) + llfd->pntr->nsize,j+sz > n ? n-j : sz);
		if(llfd->pntr->contlba == 0)
			return j;
		free(tbuf);
		j+=j+sz > n ? n-j: sz;
	}
	return j;
}
