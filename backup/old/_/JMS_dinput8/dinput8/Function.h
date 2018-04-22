#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include<Windows.h>

void FullAccess(DWORD dwAddr, DWORD dwSize);
void WriteJMP(DWORD dwPrev, DWORD dwNext);
bool WriteCODE(DWORD dwAddr, char Array[], int nop_count = 0);


#endif