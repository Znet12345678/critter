int memcmp(void *a,void *b,unsigned short n){
	char *ac = (char *)a;
	char *bc = (char *)b;
	int i;
	for(i = 0; i < n;i++)
		if(ac[i] != bc[i])
			return ++i;
	return 0;
}
