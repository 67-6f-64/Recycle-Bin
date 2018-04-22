#include<Windows.h>

int (PASCAL FAR *_connect)(SOCKET s, const struct sockaddr FAR *name, int namelen);
int PASCAL FAR connectHook(SOCKET s, const struct sockaddr FAR *name, int namelen){
	return _connect(s, name, namelen);
}

void FullAccess(DWORD Address, DWORD Size){
	DWORD OldProtect;
	VirtualProtect((void *)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
}


void WriteHook(){
	DWORD AddrConnect = (DWORD)GetProcAddress(GetModuleHandleA("WS2_32.dll"), "connect");
	//*(DWORD *)&_connect = AddrConnect;
	FullAccess(0x01CCF788, 4);
	*(DWORD *)0x01CCF788 = (DWORD)AddrConnect;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}
	//MessageBoxA(NULL, NULL, NULL, NULL);
	WriteHook();
	return TRUE;
}
