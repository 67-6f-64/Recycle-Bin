#include"ijl15.h"
#include"orgdata.h"

FARPROC _ijlInit, _ijlFree, _ijlRead, _ijlWrite, _ijlGetLibVersion, _ijlErrorStr;

void _declspec(naked ,dllexport) ijlInit(){_asm jmp dword ptr [_ijlInit]}
void _declspec(naked ,dllexport) ijlFree(){_asm jmp dword ptr [_ijlFree]}
void _declspec(naked ,dllexport) ijlRead(){_asm jmp dword ptr [_ijlRead]}
void _declspec(naked ,dllexport) ijlWrite(){_asm jmp dword ptr [_ijlWrite]}
void _declspec(naked ,dllexport) ijlGetLibVersion(){_asm jmp dword ptr [_ijlGetLibVersion]}
void _declspec(naked ,dllexport) ijlErrorStr(){_asm jmp dword ptr [_ijlErrorStr]}

void CreateOrgFile(){
	DWORD dw;
	HANDLE hOrg = CreateFileA("ijl15.old", (GENERIC_READ | GENERIC_WRITE | DELETE), (FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, CREATE_ALWAYS, (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_TEMPORARY), NULL);

	if(hOrg){
		WriteFile(hOrg, orgdata, sizeof(orgdata), &dw, NULL);
		CloseHandle(hOrg);
	}
}

void FixExports(){
	HMODULE _ijl15;

	CreateOrgFile();

	_ijl15 = LoadLibraryA("ijl15.old");

	_ijlInit = GetProcAddress(_ijl15, "ijlInit");
	_ijlFree = GetProcAddress(_ijl15, "ijlFree");
	_ijlRead = GetProcAddress(_ijl15, "ijlRead");
	_ijlWrite = GetProcAddress(_ijl15, "ijlWrite");
	_ijlGetLibVersion = GetProcAddress(_ijl15, "ijlGetLibVersion");
	_ijlErrorStr = GetProcAddress(_ijl15, "ijlErrorStr");
}