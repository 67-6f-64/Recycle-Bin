#include"NE2T.h"

HANDLE hProcess;
bool F_Attach=false;


void ProcessChecker(){
	F_Attach=true;

	while(F_Attach!=false){
		while(NE2T_OpenProcess("MapleStory.exe")!=true) Sleep(100);
		while(WaitForSingleObject(hProcess, 0)) Sleep(100);
		CloseHandle(hProcess);
		SetDlgItemText(hNE2TDialog, Static_Status, TEXT("NULL"));
	}

}


/*void MemoryTester(){
	DWORD asdf;
	char warosu[36];
	DWORD unko=0;
	while(1){
	unko=unko+4;
	ReadProcessMemory(hProcess, (DWORD *)(0x0147C8D8), &asdf, 4, NULL);
	if(ReadProcessMemory(hProcess, (DWORD *)(asdf+0x1280), &asdf, 4,NULL)){
	wsprintf(warosu, TEXT("%d"), asdf);
	SetDlgItemText(hNE2TDialog,Static_MapID,warosu);
	}else{
	SetDlgItemText(hNE2TDialog,Static_MapID,TEXT("NULL"));
	}

	ReadProcessMemory(hProcess, (DWORD *)(0x0147745C), &asdf, 4, NULL);
	if(ReadProcessMemory(hProcess, (DWORD *)(asdf+0x9528), &asdf, 4,NULL)){
	wsprintf(warosu, TEXT("%d"), asdf);
	SetDlgItemText(hNE2TDialog,Static_CharX,warosu);
	}else{
	SetDlgItemText(hNE2TDialog,Static_CharX,TEXT("NULL"));
	}

	ReadProcessMemory(hProcess, (DWORD *)(0x0147745C), &asdf, 4, NULL);
	if(ReadProcessMemory(hProcess, (DWORD *)(asdf+0x952C), &asdf, 4,NULL)){
	wsprintf(warosu, TEXT("%d"), asdf);
	SetDlgItemText(hNE2TDialog,Static_CharY,warosu);
	}else{
	SetDlgItemText(hNE2TDialog,Static_CharY,TEXT("NULL"));
	}
	Sleep(100);
	}
}*/

bool NE2T_OpenProcess(char Target[]){
	DWORD PidList[1024];
	DWORD Size,MaxSize,Dust,MaxDust;
	DWORD i,j,Last,k;
	char Title[256],FullPath[1024];
	HMODULE ModuleList[1024];
	DWORD Read;



	while(1){
	ZeroMemory(PidList, sizeof(PidList));
	EnumProcesses(PidList, sizeof(PidList), &Size);
	MaxSize = (Size/sizeof(DWORD));
	for(i=0; i<MaxSize; i++){
		hProcess=OpenProcess(PROCESS_ALL_ACCESS, FALSE, PidList[i]);
			if(hProcess!=NULL){
				ZeroMemory(ModuleList, sizeof(ModuleList));
					if(EnumProcessModules(hProcess, ModuleList, sizeof(ModuleList), &Size)){
						ZeroMemory(FullPath,sizeof(FullPath));
						GetModuleFileNameEx( hProcess, ModuleList[0], FullPath, sizeof(FullPath));
							for(j=0; FullPath[j]!=NULL; j++){
								if(FullPath[j]==0x5C)Last=j+1;
							}
						ZeroMemory(Title,sizeof(Title));
							for(k=Last; k<j; k++){
								Title[k-Last]=FullPath[k];
							}

						if(strcmp(Title, Target)==0){
							//sprintf(unko, "%08X ", CERT_HEADER_VERIFICATION[3]);
							SetDlgItemText(hNE2TDialog, Static_Status, Title);
							//CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)MemoryTester,NULL,NULL,NULL);
							return true;
						}
					}
			}
		CloseHandle(hProcess);
	}
	Sleep(100);
	}

	return false;
}