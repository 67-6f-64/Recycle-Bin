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

void _stdcall InLog(DWORD dwIn, DWORD dwKey){
	if(!Is15) return;
	AL.AddFormatItem("%d", NULL);
	AL.AddFormatItem("%d", dwKey);
}

void _stdcall OutLog(DWORD dwFunction, DWORD dwOut){
	if(!Is15) return;
	if(CheckAddr(dwFunction)){
		AL.AddFormatItem("EHSvc.dll+%d", dwFunction-EHSvc_Start);
	}
	else{
		AL.AddFormatItem("%d", dwFunction);
	}
	if(CheckAddr(dwOut)){
		AL.AddFormatItem("EHSvc.dll+%d", dwOut-EHSvc_Start);
	}
	else{
		AL.AddItem(NULL);
	}
}

DWORD InHook_Return;

void _declspec(naked) InHook(){
	_asm{
		push eax
		mov eax,fs:[0x18]
		mov eax,[eax+0x24]
		cmp [MainThread],eax
		pop eax
		jne In_End
		push eax
		mov eax,esp
		pushad
		push [eax+0x04]
		push 00
		call InLog
		popad
		pop eax
In_End:
		pushfd
		push 0x00006928
		jmp dword ptr [InHook_Return]
	}
}

void _declspec(naked) OutHook(){
	_asm{
		popfd
		push eax
		mov eax,fs:[0x18]
		mov eax,[eax+0x24]
		cmp [MainThread],eax
		pop eax
		jne Out_End
		push eax
		mov eax,esp
		pushad
		push [eax+0x08]//Out
		push [eax+0x04]//Call
		call OutLog
		popad
		pop eax
Out_End:
		ret
	}
}

void WriteHook(){
	HM.Init("EHSvc.dll");
	HM.GetDumpInfo(&EHSvc_Start, &EHSvc_End, &EHSvc);
	HM.WriteHook(0x24F195, JMP, InHook, &InHook_Return, 1);
	HM.WriteHook(0x25F9F4, JMP, OutHook);
	HM.WriteHook(0x1BCE0, JMP, St15, &St15_Ret);
	HM.WriteHook(0x1CCEB, JMP, Ed15);
}

#define EDIT_PACKET 1000
#define BUTTON_CLEAR 3000

void AFCreate(HWND hWnd){
	AL.Init(hWnd, 3, 3, 594, 294);
	AL.AddHeader("In", 140);
	AL.AddHeader("Key", 140);
	AL.AddHeader("Out", 140);
	AL.AddHeader("Out(2)", 140);
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
		Buffer = AL.GetSelectedItem(1);//call
		if(Buffer){
			Buffer2 = AL._GetSelectedItem(2);
			Buffer3 = AL._GetSelectedItem(3);
			AF.SetFormatString(EDIT_PACKET, "Key:%s -> Out:%s -> Out(2):%s", Buffer, Buffer2, Buffer3);
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