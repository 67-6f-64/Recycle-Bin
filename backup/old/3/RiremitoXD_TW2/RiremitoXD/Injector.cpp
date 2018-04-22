#include<Windows.h>
#include"AirBypass.h"

typedef struct _CLIENT_ID
{
        PVOID UniqueProcess;
        PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;
 
typedef HANDLE(WINAPI*_RtlCreateUserThread)(HANDLE,
        PSECURITY_DESCRIPTOR,
        BOOLEAN, ULONG,
        SIZE_T(ULONG_PTR), SIZE_T(ULONG_PTR),
        PVOID, PVOID,
        PHANDLE, PCLIENT_ID);
 
_RtlCreateUserThread RtlCreateUserThread;
 
BOOL WINAPI Inject(DWORD processID, wchar_t * sourceDLL)
{
 
        BOOL success = false;
        HANDLE targetProcess = NULL, createdThread = NULL;
        PWSTR pszLibFileRemote = NULL;
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
 
                PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryW");
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
 //MessageBoxW(NULL, L"END", L"Error", MB_OK);
        return success;
}