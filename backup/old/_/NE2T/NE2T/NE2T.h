#include<Windows.h>
#include<tchar.h>
#include<psapi.h>
#include<stdio.h>
#pragma comment(lib,"psapi.lib")
#include"resource.h"

extern HINSTANCE hNE2T;
extern HWND hNE2TDialog;
extern HANDLE hProcess;

void SetIcon(HINSTANCE hIns,HWND hwnd,int IconID);
INT_PTR CALLBACK NE2TDialog(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);

bool NE2T_OpenProcess(char Target[]);

extern bool F_Attach;
void ProcessChecker();

extern unsigned char Data[1024];
bool MemoryReader(DWORD Addr, DWORD Size);
extern DWORD byte4;
bool PointerReader(DWORD Pointer,DWORD Offset);