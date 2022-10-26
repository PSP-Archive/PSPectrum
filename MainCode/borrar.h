#ifndef __PERRA__
#define __PERRA__

#include "stdio.h"

extern FILE* pFile;
extern char Entries[100000][20];
extern int iNumEntries;

void Inicializa();
void Finaliza();

#endif
