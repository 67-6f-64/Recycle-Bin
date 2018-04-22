#include"AirWindow.h"
#pragma comment(lib, "AirWindow.lib")
#include"AirMemory.h"
#pragma comment(lib, "AirMemory.lib")

AirMemory HM;
AirWindow AF;
AirList AL;

DWORD EHSvc_Start, EHSvc_End, EHSvc;

DWORD ThreadList[128];
DWORD MainThread;

DWORD Is15 = 0;

DWORD St15_Ret;

void _stdcall fuck(){
	MessageBoxA(NULL, NULL, NULL, NULL);
}

void _declspec(naked) St15(){
	_asm{
		mov dword ptr [Is15],1
		push ebp
		mov ebp,esp
		push -1
		jmp dword ptr [St15_Ret]
	}
}

DWORD Ed15_Ret;
void _declspec(naked) Ed15(){
	_asm{
		mov dword ptr [Is15],0
		ret
	}
}

bool CheckAddr(DWORD dwAddr){
	if((dwAddr >= EHSvc_Start) && (dwAddr <=EHSvc_End)){
		return true;
	}
	return false;
}



void _stdcall Log(DWORD dwCall){
	if(MainThread != GetCurrentThreadId()){
		return;
	}
	if(!Is15){
		return;
	}
	if(CheckAddr(dwCall)){
		AL.AddFormatItem("EHSvc.dll+%d", dwCall-EHSvc_Start);

	if((dwCall-EHSvc_Start) == 0x25980){
			//MessageBoxA(NULL, NULL, NULL, NULL);
		}
	}
	else{
		AL.AddFormatItem("%d", dwCall);

		//MessageBoxA(NULL, NULL, NULL, NULL);
	}
	AL.AddFormatItem("%d", GetCurrentThreadId());
}

DWORD dwLogReturn;

void _declspec(naked) _Log(){
	_asm{
		push eax
		lea eax,[esp+0x28]
		pushad
		push [eax]
		call Log
		popad
		pop eax
		push 0x0000637D//org
		jmp dword ptr [dwLogReturn]
	}
}


DWORD dwInjectRet;
DWORD dwInjectFlag = 0;
DWORD dwFunction;

void _stdcall CLog(DWORD dwEax){
	if(CheckAddr(dwFunction)){
		AL.AddFormatItem("EHSvc.dll+%d", dwFunction-EHSvc_Start);
	}
	else{
		AL.AddFormatItem("%d", dwFunction);
	}
	AL.AddFormatItem("%d", dwEax);
}

void _declspec(naked) _CLog(){
	_asm{
		popfd
		push eax
		//mov eax,fs:[0x18]
		//mov eax,[eax+0x24]
		//cmp [MainThread],eax
		//jne ExitCLog
		//mov eax,[dwInjectFlag]
		//test eax,eax
		//jne ExitCLog
		//cmp dword ptr [esp+0x08],0x00400000
		//jb ExitClog
		//mov dword ptr [dwInjectFlag],1
		mov eax,[esp+0x08]
		mov dword ptr [dwInjectRet],eax
		//
		push ebx
		mov ebx,fs:[0x18]
		mov [ebx+0xC4],eax
		pop ebx
		//
		mov eax,CodeInjection
		mov dword ptr [esp+0x08],eax
		mov eax,[esp+0x04]
		mov dword ptr [dwFunction],eax
		//
		push ebx
		mov ebx,fs:[0x18]
		mov [ebx+0xC8],eax
		pop ebx
		//
		pop eax
		ret
CodeInjection:
		//pushad
		//push eax
		//call CLog
		//popad
		//mov dword ptr [dwInjectFlag],0
		push ebx
		mov ebx,fs:[0x18]
		mov ebx,[ebx+0xC8]
		mov dword ptr [dwFunction],ebx
		pop ebx
		pushad
		push eax
		call CLog
		popad
		push ebx
		mov ebx,fs:[0x18]
		mov ebx,[ebx+0xC4]
		mov dword ptr [dwInjectRet],ebx
		pop ebx
		jmp dword ptr [dwInjectRet]
ExitCLog:
		pop eax
		ret
	}
}

void WriteHook(){
	HM.Init("EHSvc.dll");
	HM.GetDumpInfo(&EHSvc_Start, &EHSvc_End, &EHSvc);
	HM.WriteHook(0x24FBA8, JMP, _Log, &dwLogReturn);
	//HM.WriteHook(0x25F9F4, JMP, _CLog);
	HM.WriteHook(0x1BCE0, JMP, St15, &St15_Ret);
	HM.WriteHook(0x1CCEB, JMP, Ed15);
}

#define EDIT_PACKET 1000
#define BUTTON_CLEAR 3000

void AFCreate(HWND hWnd){
	AL.Init(hWnd, 3, 3, 594, 294);
	//AL.AddHeader("In", 140);
	//AL.AddHeader("Key", 140);
	AL.AddHeader("Call", 140);
	AL.AddHeader("Value", 140);
	//AL.AddHeader("Return", 140);
	AF.EditBox(EDIT_PACKET, NULL, 3, 320, 400);
	AF.Button(BUTTON_CLEAR, "Clear", 450, 300);
	WriteHook();
}

void AFControl(WORD wID){
	switch(wID){
		case BUTTON_CLEAR:
			AL.Clear();
			break;
		default:
			break;
	}
}


void AFNotify(){
	HWND hWnd = GetFocus();
	char *Buffer, *Buffer2, *Buffer3, *Buffer4;

	if(hWnd == AL.GetHWND()){
		Buffer = AL.GetSelectedItem(0);//call
		if(Buffer){
			//Buffer2 = AL._GetSelectedItem(1);
			//AF.SetFormatString(EDIT_PACKET, "Function:%s, (%s)", Buffer, Buffer2);
			AF.SetFormatString(EDIT_PACKET, "%s", Buffer);
		}
	}
	
}


void AirF_Run(HINSTANCE hInstance){
	HWND hWnd= NULL;
	while(!(hWnd = FindWindowA(NULL, "MapleStory"))){
		Sleep(50);
	}
	MainThread = GetWindowThreadProcessId(hWnd, ThreadList);

	AF.Init(hInstance, "Air Finder for v5.6.34.449", 600, 400, true, AFCreate, AFControl, AFNotify);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AirF_Run, hinstDLL ,NULL, NULL);

	return TRUE;
}