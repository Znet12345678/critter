void memcpy(void *dest,void *src,unsigned short n){
	int i = 0;
	for(i = 0; i < n;i++)
		*((char *)dest + i) = *((char *)src + i);
}
