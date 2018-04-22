#include"ijl15.h"
#include"orgdata.h"

DWORD org_ijl15_1;
DWORD org_ijl15_2;
DWORD org_ijl15_3;
DWORD org_ijl15_4;
DWORD org_ijl15_5;
DWORD org_ijl15_6;

void _declspec(naked) ijl15_1(){
	_asm{
		jmp dword ptr [org_ijl15_1]
	}
}

void _declspec(naked) ijl15_2(){
	_asm{
		jmp dword ptr [org_ijl15_2]
	}
}

void _declspec(naked) ijl15_3(){
	_asm{
		jmp dword ptr [org_ijl15_3]
	}
}

void _declspec(naked) ijl15_4(){
	_asm{
		jmp dword ptr [org_ijl15_4]
	}
}

void _declspec(naked) ijl15_5(){
	_asm{
		jmp dword ptr [org_ijl15_5]
	}
}

void _declspec(naked) ijl15_6(){
	_asm{
		jmp dword ptr [org_ijl15_6]
	}
}


bool InitProxyDll(){
	DWORD dw;
	HANDLE hOrg = CreateFileA("ijl15.old", (GENERIC_READ | GENERIC_WRITE), NULL, NULL, CREATE_ALWAYS, NULL, NULL);

	if(hOrg){
		WriteFile(hOrg, org_data, sizeof(org_data), &dw, NULL);
		CloseHandle(hOrg);
	}

	HMODULE ijl15 = LoadLibraryA("ijl15.old");

	org_ijl15_1 = (DWORD)GetProcAddress(ijl15, "ijlInit");
	org_ijl15_2 = (DWORD)GetProcAddress(ijl15, "ijlFree");
	org_ijl15_3 = (DWORD)GetProcAddress(ijl15, "ijlRead");
	org_ijl15_4 = (DWORD)GetProcAddress(ijl15, "ijlWrite");
	org_ijl15_5 = (DWORD)GetProcAddress(ijl15, "ijlGetLibVersion");
	org_ijl15_6 = (DWORD)GetProcAddress(ijl15, "ijlErrorStr");

	return true;
}