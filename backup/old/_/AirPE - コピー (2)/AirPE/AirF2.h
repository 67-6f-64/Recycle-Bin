#ifndef __AIRWINDOW2_H__
#define __AIRWINDOW2_H__

#include<Windows.h>

#define MaxWindowCount 128
#define MaxString 1024*3
#define MaxHeader 8

namespace Air2{
	//Function
	void QWORDtoChar(char *temp, DWORD dw1, DWORD dw2);
	void DWORDtoChar(char *temp, DWORD dw);
	void WORDtoChar(char *temp, WORD w);
	void BYTEtoChar(char *temp, BYTE b);
	void BYTEArraytoString(char *temp, DWORD dwSize, BYTE Array[]);
	void INTtoChar(char *temp, DWORD d);
	int GetStringLength(char *_String);
	void _stdcall _CreateFormatString(char *_Output, char *_Format, DWORD *dwArgument);
	void _stdcall CreateFormatString(char *_Output, char *_Format, ...);
}



#endif