/*
 * Linked List File System Inscriber
 * (c) December 2018 Zachary James Schlotman
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#define __NOT_WORM
#include "llfs.h"
struct sector{
	uint8_t *data;
	struct sector *nxt;
}
void usage(const char *arg){
	printf("Usage:%s [image] [directory]\n",arg);	
}
struct sector *trev(struct sector *pntr,unsigned int lba){
	for(int i = 0; i < lba;i++)
		pntr = pntr->nxt;
	return pntr;
}
int mkdir(const char *dir,struct sector *head){
	char *tok = strtok(dir,"/");
	struct sector *dup = head;
	while(strtok(NULL,"/") != NULL){
		int i = 0;
		while(i < 512-sizeof(struct Entry)-sizeof(uint16_t)){
			uint16_t namelen = dup->data + sizeof(struct Entry);
			if(strcmp(dup->data + sizeof(uint16_t) + sizeof(struct Entry),tok) == 0 && dup->type == TYPE_DIR)
				break;
			dup = dup->nxt;
			i+=namelen;
		}
		if(i == 512-sizeof(struct Entry)-sizeof(uint16_t))
			return 0;
		dup = trev(head,dup->nxtlba);
		tok = strtok(NULL,"/");
	}
	struct Entry *dir = malloc(sizeof(*dir));
	int size = 0;
	dup = head;
	while(dup->nxt != NULL){
		size++;
		dup = dup->nxt;
	}
	dir->type = TYPE_DIR;
	dir->nxtlba = size+3;
	dir->contlba = size+2;
	dup->nxt = malloc(sizeof(*dup->nxt));
	dup = dup->nxt;
	uint16_t len = strlen(tok);
	memcpy(dup->data,dir,sizeof(*dir));	
	memcpy(dup->data + sizeof(*dir),&len,sizeof(len));
	memcpy(dup->data + sizeof(*dir) + sizeof(len),tok,len);
	dup->nxt = malloc(sizeof(dup->nxt));
	dup->nxt->nxt = malloc(sizeof(dup->nxt->nxt));
	return 1;
}
struct sector *opendir(const char *name,struct sector *head){
	char *tok = strtok(head,"/");
	struct sector *dup = head;
	while(tok != NULL){
		int i = 0;
		uint16_t len = dup->data + sizeof(struct Entry);
		while(i < 512-len-sizeof(struct Entry)){
			if(strcmp(dup->data + sizeof(struct(Entry)) + sizeof(uint16_t),tok) == 0)
				break;
			i+=len;
		}
		tok = strtok(NULL,"/");
		dup = trev(head,dup->nxtlba);

	}
	return dup;
}
struct Entry *mkEntry(uint8_t type,uint32_t nlba,uint32_t contlba){
	struct Entry *ret = malloc(sizeof(*ret));
	ret->type = type;
	ret->nxtlba = nlba;
	ret->contlba = contlba;
	return ret;
}
int write_file(FILE *in,const char *name,struct sector *head){
	char *containing = malloc(strlen(name));
	if(!in)
		return 0;
	char *tok = strtok(name,"/");
	while(strtok(NULL,"/") != NULL){
		sprintf(containing,"%s/%s",containing,tok);
		tok = strtok(NULL,"/");
	}
	struct sector *dir = opendir(containing,head);	
	dir->	
}
int main(int argc,char *argv[]){
	printf("Linked List File System Version Alpha\n");
	printf("(c) 2018 Zachary James Schlotman\n");
	printf("Warning: Please keep directories small. Everything is done in memory before the write to disk...\n");
	if(argc < 3){
		usage(argv[1]);
		return -1;
	}
	struct Entry *ent = malloc(sizeof(*ent));
	ent->type = TYPE_DIR;
	ent->nxtlba = 1;
	ent->contlba = 0;
	struct sector *head = malloc(sizeof(*head));
	bzero(head,sizeof(*head));
	memcpy(head->data,ent,sizeof(*ent));
	memcpy(head->data + sizeof(*ent),0,sizeof(uint16_t));
	return 0;
}
