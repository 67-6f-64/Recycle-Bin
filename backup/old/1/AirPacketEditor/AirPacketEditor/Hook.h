#ifndef __HOOK_H__
#define __HOOK_H__

void Hook();
void set_hook(DWORD Function, DWORD Size, LPVOID HookFunction, LPVOID NewFunction);
extern DWORD firstthread;
#endif