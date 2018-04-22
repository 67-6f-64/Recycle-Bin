#ifndef __AIRFUNCTION_H__
#define __AIRFUNCTION_H__

#include<Windows.h>

namespace Air{
	void DWORDtoChar(char *temp, DWORD dw);
	void WORDtoChar(char *temp, WORD w);
	void BYTEtoChar(char *temp, BYTE b);
	void BYTEArraytoString(char *temp, DWORD dwSize, BYTE Array[]);
	void INTtoChar(char *temp, DWORD d);
	void _stdcall _CreateFormatString(char *_Output, char *_Format, DWORD *dwArgument);
	void _stdcall CreateFormatString(char *_Output, char *_Format, ...);
	int GetStringLength(char *_String);
}

#endif