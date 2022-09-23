#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

BOOL CloseCOM();
BOOL VerifStr(char * cStr,int iStrSize);
int StrToInt(char * cStr,int iStrSize);
BOOL parseBuffer(int tab[],int * iTabSize, char * cStr);
BOOL OpenCOM(int nId);
BOOL ReadCOM(void* buffer, int nBytesToRead, int* pBytesRead);
BOOL WriteCOM(void* buffer, int nBytesToWrite, int* pBytesWritten);