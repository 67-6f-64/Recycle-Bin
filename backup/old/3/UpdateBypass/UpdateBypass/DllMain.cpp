#include<Windows.h>
#include"AirMemory.h"
#pragma comment(lib, "AirMemory.lib")
#include<stdio.h>
#include"ExportHook.h"

DWORD _DllInitialize;
void _declspec(naked) DllInitialize(){
	_asm{
		jmp dword ptr [_DllInitialize]
	}
}

DWORD _AlphaBlend;
void _declspec(naked) AlphaBlend(){
	_asm{
		jmp dword ptr [_AlphaBlend]
	}
}

DWORD _GradientFill;
void _declspec(naked) GradientFill(){
	_asm{
		jmp dword ptr [_GradientFill]
	}
}

DWORD _TransparentBlt;
void _declspec(naked) TransparentBlt(){
	_asm{
		jmp dword ptr [_TransparentBlt]
	}
}

DWORD _vSetDdrawflag;
void _declspec(naked) vSetDdrawflag(){
	_asm{
		jmp dword ptr [_vSetDdrawflag]
	}
}

void CreateExport(){
	HMODULE hModule;
	hModule = LoadLibraryA("C:\\Windows\\System32\\msimg32.dll");
	if(!hModule){
		MessageBoxA(NULL, "C:\\Windows\\System32\\msimg32.dll‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñ!", "Error", NULL);
		ExitProcess(0);
	}
	_DllInitialize = (DWORD)GetProcAddress(hModule, "DllInitialize");
	_AlphaBlend = (DWORD)GetProcAddress(hModule, "AlphaBlend");
	_GradientFill = (DWORD)GetProcAddress(hModule, "GradientFill");
	_TransparentBlt = (DWORD)GetProcAddress(hModule, "TransparentBlt");
	_vSetDdrawflag = (DWORD)GetProcAddress(hModule, "vSetDdrawflag");
}

int HideWindow(HWND hWnd){
	int nCmdShow = SW_SHOW;
	HWND hWndParent;
	LPWSTR lpClassName = (LPWSTR)malloc(256);//, lpWindowName = (LPWSTR)malloc(256);
	ZeroMemory(lpClassName, 256);

	if(GetClassNameW(hWnd, lpClassName, 256)){
		if(wcscmp(lpClassName, L"TUpgradeForm") == 0){
			nCmdShow = SW_HIDE;
		}
		else if(wcscmp(lpClassName, L"Shell Embedding") == 0){

			hWndParent = GetParent(hWnd);
			if(hWndParent){
				if(GetClassNameW(hWndParent, lpClassName, 256)){
					if(wcscmp(lpClassName, L"TChatBanner") == 0){
						nCmdShow = SW_HIDE;
					}
				}
			}
			
			/*
			FILE *fp;
			fp = fopen("C:\\Users\\URYYY\\Desktop\\mem\\Memo112.txt", "a");
			fprintf(fp, "[%S -> ", lpClassName);
			hWndParent = hWnd;
			while(hWndParent = GetParent(hWndParent)){
				GetClassNameW(hWndParent, lpClassName, 256);
				fprintf(fp, "%S -> ", lpClassName);
			}
			fprintf(fp, "]\n");
			fclose(fp);
			*/
			//if(!FindWindowW(L"TLoginForm", NULL)){
			//	nCmdShow = SW_HIDE;
			//}
		}
		
	}
	free(lpClassName);
	return  nCmdShow;
}

BOOL (WINAPI *_ShowWindow)(HWND hWnd, int nCmdShow);
BOOL WINAPI ShowWindowHook(HWND hWnd, int nCmdShow){
	if(HideWindow(hWnd) == SW_HIDE){
		return _ShowWindow(hWnd, SW_HIDE);
	}
	return _ShowWindow(hWnd, nCmdShow);
}

BOOL (WINAPI *_SetWindowPos)(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
BOOL WINAPI SetWindowPosHook(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags){
	if(HideWindow(hWnd) == SW_HIDE){
		return 0;
	}
	return _SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

void NewPatch(){
	ExportHook EH;
	EH.Init("user32.dll");
	EH.Hook("ShowWindow", ShowWindowHook, &_ShowWindow);
	EH.Hook("SetWindowPos", SetWindowPosHook, &_SetWindowPos);

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateExport();
	NewPatch();

	return TRUE;
}

/*
C:\Program Files (x86)\Skype\Phone\

*/