#include"dinput8.h"

DWORD org_dinput8_1;
DWORD org_dinput8_2;
DWORD org_dinput8_3;
DWORD org_dinput8_4;
DWORD org_dinput8_5;

void _declspec(naked) dinput8_1(){
	_asm{
		jmp dword ptr [org_dinput8_1]
	}
}

void _declspec(naked) dinput8_2(){
	_asm{
		jmp dword ptr [org_dinput8_2]
	}
}

void _declspec(naked) dinput8_3(){
	_asm{
		jmp dword ptr [org_dinput8_3]
	}
}

void _declspec(naked) dinput8_4(){
	_asm{
		jmp dword ptr [org_dinput8_4]
	}
}

void _declspec(naked) dinput8_5(){
	_asm{
		jmp dword ptr [org_dinput8_5]
	}
}

bool InitProxyDll(){
	HMODULE dinput8 = LoadLibraryA("C:/Windows/System32/dinput8.dll");

	if(!dinput8){
		MessageBoxA(NULL, "Shit! You don't have dinput8.dll(org)\nor System FilePath is not default", "dinput8 bypass v103.1", NULL);
		return false;
	}

	org_dinput8_1 = (DWORD)GetProcAddress(dinput8, "DllGetClassObject");
	org_dinput8_2 = (DWORD)GetProcAddress(dinput8, "DllCanUnloadNow");
	org_dinput8_3 = (DWORD)GetProcAddress(dinput8, "DirectInput8Create");
	org_dinput8_4 = (DWORD)GetProcAddress(dinput8, "DllRegisterServer");
	org_dinput8_5 = (DWORD)GetProcAddress(dinput8, "DllUnregisterServer");

	if(!org_dinput8_1 || !org_dinput8_2 || !org_dinput8_3 || !org_dinput8_4 || !org_dinput8_5){
		MessageBoxA(NULL, "Your dinput8.dll(org) is broken\nor Are you using pirated Windows?", "dinput8 bypass v103.1", NULL);
		return false;
	}

	return true;
}