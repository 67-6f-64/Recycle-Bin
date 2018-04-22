#include<Windows.h>
#include"MapleStory.h"
#include"apihook.h"
#pragma comment(lib, "ws2_32.lib")
#include"memory.h"
#include"XignCode.h"

void DLLInjection(DWORD processID){
        BOOL success = false;
        HANDLE targetProcess = NULL, createdThread = NULL;
        PWSTR pszLibFileRemote = NULL;
		WCHAR sourceDLL[0x100] = L"C:\\Users\\Elfen Lied\\Desktop\\MapleStory\\BypassBC.dll";
		LoadLibraryW(sourceDLL);
		GetCurrentDirectoryW(0x100, sourceDLL); 
  //MessageBoxW(NULL, sourceDLL, L"Error", MB_OK);
        __try
        {
 
                targetProcess = OpenProcess(
                        PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
                        FALSE, processID);

 
                int cch = 1 + lstrlenW(sourceDLL); //Calculate the number of bytes required for the DLL's path
                int cb = cch * sizeof(wchar_t);
                //cout << cb << endl; //size of string of the dll
                pszLibFileRemote = (PWSTR)VirtualAllocEx(targetProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
                if (pszLibFileRemote == NULL)
                {
                        MessageBoxW(NULL, L"VirtualAlloc Error : Could not allocate dll pathname in target process.", L"Error", MB_OK);
                        __leave;
                }
 
                if (!WriteProcessMemory(targetProcess, pszLibFileRemote, (PVOID)sourceDLL, cb, NULL))
                {
                        MessageBoxW(NULL, L"WriteProcessMemory Error : Could not write dll pathname in target process.", L"Error", MB_OK);
                        __leave;
                }
 
                PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32.dll"), "LoadLibraryW");
                //             
                createdThread = CreateRemoteThread(targetProcess, NULL, 0, pfnThreadRtn, pszLibFileRemote, 0, NULL);
  //MessageBoxW(NULL, L"wait", L"Error", MB_OK);
                WaitForSingleObject(createdThread, INFINITE);
                success = true;
				// MessageBoxW(NULL, L"last", L"Error", MB_OK);
        }
        __finally { // Now, we can clean everything up
                // Free the remote memory that contained the DLL's pathname
                if (pszLibFileRemote != NULL)
                        VirtualFreeEx(targetProcess, pszLibFileRemote, 0, MEM_RELEASE);
                if (createdThread != NULL)
                        CloseHandle(createdThread);
                if (targetProcess != NULL)
                        CloseHandle(targetProcess);
        }	
}

//WAIT FOR UNPACK
HWND (WINAPI *_CreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
HWND WINAPI CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam){
	if(lpClassName && strstr(lpClassName, "StartUpDlgClass")){
		if(*(BYTE *)((DWORD)CreateWindowExA) == 0xEB){
			*(WORD *)((DWORD)CreateWindowExA) = 0xFF8B;//unhook
		}
		//MapleStory();
		//XignCode();
	}
	return _CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

//Multi Client(MS)
HANDLE (WINAPI *_CreateMutexExA)(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCTSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
HANDLE WINAPI CreateMutexExA_Hook(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCTSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess){
	HANDLE hMutex, hDuplicatedMutex;
	if(lpName && strstr(lpName, "WvsClientMtx")){
		hMutex =  _CreateMutexExA(lpMutexAttributes, lpName, dwFlags, dwDesiredAccess);
		DuplicateHandle(GetCurrentProcess(), hMutex, 0, &hDuplicatedMutex, 0, FALSE, DUPLICATE_CLOSE_SOURCE);
		CloseHandle(hDuplicatedMutex);
		return hMutex;
	}
	return _CreateMutexExA(lpMutexAttributes, lpName, dwFlags, dwDesiredAccess);
}

//LOCAL IP BYPASS
LPHOSTENT (WINAPI *_gethostbyname)(char *name);
LPHOSTENT WINAPI gethostbyname_Hook(char *name){
	LPHOSTENT h;
	char localpc[0x40];
	gethostname(localpc, 0x40);
	h = _gethostbyname(name);
	if(strstr(localpc, name)){
		*(BYTE *)(h->h_addr_list[0] + 2) = GetCurrentProcessId();
		*(BYTE *)(h->h_addr_list[0] + 3) = GetCurrentProcessId() >> 8;
	}
	return h;
}

//LOCAL IP BYPASS
int (WINAPI *_getsockname)(SOCKET s, struct sockaddr_in *name, int *namelen);
int WINAPI getsockname_Hook(SOCKET s, struct sockaddr_in *name, int *namelen){
	int r = _getsockname(s, name, namelen);
	name->sin_addr.S_un.S_un_w.s_w2 = GetCurrentProcessId();
	return r;
}

BOOL (WINAPI *_CreateProcessInternalW)(DWORD Always01, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, DWORD Always02);
BOOL WINAPI CreateProcessInternalW_Hook(DWORD Always01, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, DWORD Always02){
	BOOL bResult;
	HANDLE hThread;
	if(lpCommandLine != NULL && wcsstr(lpCommandLine, L"BlackCipher.aes")){
		
		bResult = _CreateProcessInternalW(Always01, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, CREATE_SUSPENDED, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, Always02);
		hThread = OpenThread(THREAD_ALL_ACCESS, NULL, lpProcessInformation->dwThreadId);
		DLLInjection(lpProcessInformation->dwProcessId);
		ResumeThread(hThread);
		CloseHandle(hThread);
		return bResult;
	}
	return _CreateProcessInternalW(Always01, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, Always02);
}


void apihook_ms(){
	apihook user32("user32.dll");
	user32.Hook("CreateWindowExA", CreateWindowExA_Hook, &_CreateWindowExA);

	apihook kernelbase("kernelbase.dll");
	kernelbase.Hook("CreateMutexExA", CreateMutexExA_Hook, &_CreateMutexExA);

	apihook ws2_32("ws2_32.dll");
	ws2_32.Hook("gethostbyname", gethostbyname_Hook, &_gethostbyname);
	ws2_32.Hook("getsockname", getsockname_Hook, &_getsockname);


	apihook kernel32("kernel32.dll");
	kernel32.Hook("CreateProcessInternalW", CreateProcessInternalW_Hook, &_CreateProcessInternalW);
}

char MapleID[128] = {0};
void _stdcall HWID_Randomizer(BYTE HWID[]){
	int i, MapleID_Size;
	DWORD seed = 0, seed2 = 0;

	for(i=0; MapleID[i]; i++){
		seed += MapleID[i];
		seed2 ^= MapleID[i];
	}
	MapleID_Size = i;


	for(i=0; i<0x10; i++){
		HWID[i] ^= MapleID_Size;
		HWID[i] ^= seed;
	}
	for(i=0; i<0x10; i++){
		HWID[i] ^= MapleID[i%MapleID_Size];
	}
	*(DWORD *)&HWID[0x0C] ^= seed2;
	*(DWORD *)&HWID[0x08] ^= *(DWORD *)&HWID[0x0C];
	*(DWORD *)&HWID[0x04] ^= *(DWORD *)&HWID[0x08];
	*(DWORD *)&HWID[0x00] ^= *(DWORD *)&HWID[0x04];
}

void (_fastcall *_Login)(LPVOID ecx, LPVOID edx, LPVOID v1, char *s1, char *s2);
void _fastcall Login_Hook(LPVOID ecx, LPVOID edx, LPVOID v1, char *s1, char *s2){
	RtlCopyMemory(MapleID, s1, strlen(s1) + 1);
	return _Login(ecx, edx, v1, s1, s2);
}

void (_fastcall *_GetHWID)(LPVOID ecx, LPVOID edx);
void _fastcall GetHWID_Hook(LPVOID ecx, LPVOID edx){
	_GetHWID(ecx, edx);
	HWID_Randomizer((BYTE *)((DWORD)ecx + 0x14));
}

//v354.0
void MapleStory(){
	memory Maple;
	//themida crc
	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 45 ?? 53 56 57 50 8D 45 ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? C7 85 ?? ?? ?? ?? 00 00 00 00 6A 01 8B 8D ?? ?? ?? ?? E8", "31 C0 C2 04 00");
	//dr_check
	Maple.scan("55 8B EC 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 45 ?? 53 56 57 E9 -2", "31 C0 C3");
	//process list packet
	//Maple.scan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B F9 E9", "31 C0 C2 04 00");//v353.1
	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 45 ?? 50 8D 45 ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? E9 -2", "31 C0 C2 04 00");//v354.0
	//Login
	//Maple.scan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 50 A1 ?? ?? ?? ?? 33 C4 89 44 24 ?? 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 33 ED C7 44 24 ?? 01 00 00 00 39 AE ?? ?? ?? ?? 74");//v353.1
	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 45 ?? 50 8D 45 ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? C7 45 ?? 01 00 00 00 8B 85 ?? ?? ?? ?? 83 B8 ?? ?? ?? ?? 00 74");//v354.0
	Maple.Hook(Maple.get(), Login_Hook, &_Login);
	//HWID
	Maple.scan("81 EC ?? ?? ?? ?? 33 C0 56 89 44 24 ?? 8B F1 89 44 24 ?? 8D 4C 24 ?? 89 44 24 ?? 51 66 89 44 24 ?? 89 74 24 ?? 50 C6 44 24 ?? 00 88 44 24 ?? 89 44 24 ?? 89 44 24 ?? E8 ?? ?? ?? ?? 83 F8 6F 75");
	Maple.Hook(Maple.get(), GetHWID_Hook, &_GetHWID);
}
