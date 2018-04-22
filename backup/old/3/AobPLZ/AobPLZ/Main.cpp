#include<Windows.h>
#include<stdio.h>

void DLLInjection(HANDLE hProcess){
	char DLL[] = "HookPLZ.dll";
	char ExePath[1024];
	char Drive[1024], Directory[1024];
	void *Memory;
	HANDLE hThread;

	GetModuleFileNameA(NULL, ExePath, 1024);
	_splitpath(ExePath, Drive, Directory, NULL, NULL);
	_makepath(ExePath, Drive, Directory, NULL, NULL);
	
	Memory = VirtualAllocEx(hProcess, NULL, sizeof(ExePath), MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, Memory, ExePath, sizeof(ExePath), NULL);
	hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)SetDllDirectoryA, Memory, NULL, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, Memory, 0, MEM_RELEASE);
	
	Memory = VirtualAllocEx(hProcess, NULL, sizeof(DLL), MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, Memory, DLL, sizeof(DLL), NULL);
	hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, Memory, NULL, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, Memory, 0, MEM_RELEASE);
	

	
	/*
	Memory = VirtualAllocEx(hProcess, NULL, sizeof(InjectDLL), MEM_COMMIT, PAGE_READWRITE);

	if(WriteProcessMemory(hProcess, Memory, InjectDLL, sizeof(InjectDLL), NULL)){
		hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, Memory, NULL, NULL);
		WaitForSingleObject(hThread, INFINITE);
	}

	*/
}


void Test(LPSTR lpCmdLine){
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	CreateProcessA(NULL, lpCmdLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	DLLInjection(pi.hProcess);
	ResumeThread(pi.hThread);
	//WaitForInputIdle(pi.hProcess, INFINITE);
	//DLLInjection(pi.hProcess);

	//SuspendThread(pi.hThread);
	//TerminateProcess(pi.hProcess, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	if(lpCmdLine[0]){
		Test(lpCmdLine);
		//MessageBoxA(NULL, lpCmdLine, "AobPLZ", NULL);
	}
	else{
		MessageBoxA(NULL, "?", "AobPLZ", NULL);
	}
	
	return 0;
}