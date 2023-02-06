/*
Acess64 - Build Helper
 Build Incrementer
*/
#include <stdio.h>

/*
FORMAT:
#define	ACESS_BUILD	000000
#define	BUILD_STRING	EXPAND2STR(BUILD)

*/

int main(int argc, char *argv[])
{
	FILE	*fp;
	 int	ver = 0;
	char	verS[7] = {0};
	fp = fopen(argv[1], "r");
	if(fp == NULL)
		return -1;
	fseek(fp, 21, SEEK_SET);
	//fscanf(fp, "%u", &ver);
	fscanf(fp, "%6s", verS);
	fclose(fp);
	
	ver  = (verS[0]-'0') * 100000;
	ver += (verS[1]-'0') * 10000;
	ver += (verS[2]-'0') * 1000;
	ver += (verS[3]-'0') * 100;
	ver += (verS[4]-'0') * 10;
	ver += (verS[5]-'0') * 1;
	ver ++;
	
	fp = fopen(argv[1], "w");
	fprintf(fp, "#define	ACESS_BUILD		%06u\n", ver);
	fprintf(fp, "#define	BUILD_STRING	EXPAND2STR(BUILD)\n", ver);
	fclose(fp);
	return 0;
}
