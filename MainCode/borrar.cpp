#include "stdio.h"
#include "CMGELog.h"

FILE* pFile;
char Entries[50000][40];
int iNumEntries;


void Inicializa()
{
	pFile=fopen("log.log","w");
	iNumEntries=0;
}

void Finaliza()
{
	CMGELog::SysLog(CMGELog::lError,"Grabando strings");
	fprintf(pFile,"Num entries %i\n", iNumEntries);
	for(int i=0;i<iNumEntries;++i)
		fprintf(pFile,Entries[i]);
	fclose(pFile);
}

