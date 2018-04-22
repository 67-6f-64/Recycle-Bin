#include"ijl15.h"
#include"org_data.h"

DWORD dwExport1, dwExport2, dwExport3, dwExport4, dwExport5, dwExport6;

void _declspec(naked) ijlInit(){_asm{jmp dword ptr [dwExport1]}}
void _declspec(naked) ijlFree(){_asm{jmp dword ptr [dwExport2]}}
void _declspec(naked) ijlRead(){_asm{jmp dword ptr [dwExport3]}}
void _declspec(naked) ijlWrite(){_asm{jmp dword ptr [dwExport4]}}
void _declspec(naked) ijlGetLibVersion(){_asm{jmp dword ptr [dwExport5]}}
void _declspec(naked) ijlErrorStr(){_asm{jmp dword ptr [dwExport6]}}

void CreateExports(){
	DWORD dw;
	HANDLE hFile = CreateFileA("ijl15.old", (GENERIC_READ | GENERIC_WRITE), NULL, NULL, CREATE_ALWAYS, NULL, NULL);

	if(hFile){
		WriteFile(hFile, org_data, sizeof(org_data), &dw, NULL);
		CloseHandle(hFile);
	}

	HMODULE ijl15 = LoadLibraryA("ijl15.old");

	dwExport1 = (DWORD)GetProcAddress(ijl15, "ijlInit");
	dwExport2 = (DWORD)GetProcAddress(ijl15, "ijlFree");
	dwExport3 = (DWORD)GetProcAddress(ijl15, "ijlRead");
	dwExport4 = (DWORD)GetProcAddress(ijl15, "ijlWrite");
	dwExport5 = (DWORD)GetProcAddress(ijl15, "ijlGetLibVersion");
	dwExport6 = (DWORD)GetProcAddress(ijl15, "ijlErrorStr");
}