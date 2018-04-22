#include"AirPL.h"

#define MaxPacketSize 1024

AirWindow AP;
AirList AL;

bool bSHook = true, bRHook = true;

void _stdcall SendLog(BYTE Packet[], DWORD dwSize, DWORD dwReturn){
	if(!bSHook){
		return;
	}
	AL.AddFormatItem("%s", "OUT");
	AL.AddFormatItem("%d", dwReturn);
	AL.AddFormatItem("%i", dwSize);
	if(dwSize < MaxPacketSize){
		AL.AddFormatItem("%xa", Packet, dwSize);
	}
	else{
		AL.AddFormatItem("%xa %s", Packet, 2, "(È—ª‚³‚ê‚Ü‚µ‚½)");
	}
}

void _stdcall RecvLog(BYTE Packet[], DWORD dwSize, DWORD dwReturn){
	if(!bRHook){
		return;
	}
	if(dwSize < 6){
		return;
	}
	dwSize -= 4;
	AL.AddFormatItem("%s", "IN");
	AL.AddFormatItem("%d", dwReturn);
	AL.AddFormatItem("%i", dwSize);
	if(dwSize < MaxPacketSize){
		AL.AddFormatItem("%xa", Packet+4, dwSize);
	}
	else{
		AL.AddFormatItem("%xa %s", Packet+4, 2, "(È—ª‚³‚ê‚Ü‚µ‚½)");
	}
}

#define EDIT_PACKET 1000
#define EDIT_RETURN 1001
#define CHECK_HOOKS 2000
#define CHECK_HOOKR 2001
#define BUTTON_CLEAR 3000

void PECreate(HWND hWnd){
	AL.Init(hWnd, 3, 3, 494, 294);
	AL.AddHeader("Type", 40);
	AL.AddHeader("Return", 80);
	AL.AddHeader("Size", 50);
	AL.AddHeader("Packet", 290);
	AP.EditBox(EDIT_PACKET, NULL, 3, 320, 400);
	AP.EditBox(EDIT_RETURN, NULL, 3, 300, 80);
	AP.CheckBox(CHECK_HOOKS, "SendHook", 100, 300, true);
	AP.CheckBox(CHECK_HOOKR, "RecvHook", 200, 300, true);
	AP.Button(BUTTON_CLEAR, "Clear", 450, 300);
	WriteHooks();
}

void PEControl(WORD wID){
	switch(wID){
		case BUTTON_CLEAR:
			AL.Clear();
			break;
		case CHECK_HOOKS:
			bSHook = AP.CheckBoxStatus(wID);
			break;
		case CHECK_HOOKR:
			bRHook = AP.CheckBoxStatus(wID);
			break;
		default:
			break;
	}
}


void PENotify(){
	HWND hWnd = GetFocus();
	char *Buffer, *Buffer2;

	if(hWnd == AL.GetHWND()){
		Buffer = AL.GetSelectedItem(3);
		if(Buffer){
			Buffer2 = AL._GetSelectedItem(1);
			AP.SetString(EDIT_PACKET, Buffer);
			AP.SetString(EDIT_RETURN, Buffer2);
		}
	}
	
}


void AirPL_Run(HINSTANCE hInstance){
	AP.Init(hInstance, "Air Packet Logger", 600, 400, true, PECreate, PEControl, PENotify);
}