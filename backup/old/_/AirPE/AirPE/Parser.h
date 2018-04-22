#ifndef __PARSER_H__
#define __PARSER_H__

#include<Windows.h>

extern char SBuffer[8192*4];
extern DWORD SCurrentPointer;
extern DWORD SPacketSize;
extern DWORD orgRSize;
extern DWORD orgRReturn;
extern char RBuffer[8192*4];

void EncodeHook();
void _stdcall SFinish();
void _stdcall RInit();
void _stdcall RFinish();
void DecodeHook();
void _stdcall DecodeFinish();
extern DWORD RPacketSize;
extern DWORD RCurrentPointer;
extern bool bFormatView;
#endif