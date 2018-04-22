#ifndef __AIRASM_H__
#define __AIRASM_H__

#include<Windows.h>

BOOL FullAccess(DWORD dwAddress, DWORD dwSize);
bool Hook(char *Type, DWORD dwPrev, DWORD dwNext);
bool HexWriter(DWORD dwAddress, LPCSTR lpString);
int ChartoByte(LPBYTE lpByte, LPCSTR lpString);
int StringCleaner(LPSTR lpTemp, LPCSTR lpString);

#endif