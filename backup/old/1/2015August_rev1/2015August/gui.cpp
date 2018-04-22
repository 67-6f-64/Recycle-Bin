#include"gui.h"
#include"hook.h"
#include"packet.h"
#include<stdio.h>
#include"AutoLogin.h"

AirWindow AW;

#define C_DO 101
#define C_AUTOLOGIN 102
#define C_EXPEXPLOIT 103
#define C_SELFSELECT 104

#define B_TEST 201
#define B_TELE1 202
#define B_TELE2 203
#define B_SELFSELL 204
#define B_SHOW 205


#define S_MAP 301
#define S_MESO 302
#define S_CHANNEL 303
#define S_NAME 304


bool bTeleporting = false;
bool bDo = false;
bool bExpExploit = false;
bool bSelfSelect = false;//true;


void gui_update_map(DWORD MapID){
	AW.SetFormatString(S_MAP, "Map:%i", MapID);
	if(MapID != 680000502){
		bDo = false;
		Action = 0;
	}
}

void gui_update_map2(){
	AW.SetString(S_MAP, "Map:Login");
	bDo = false;
	Action = 0;
}

void gui_update_channel(BYTE Channel){
	AW.SetFormatString(S_CHANNEL, "Channel:%i", Channel);
}
char sMeso[256];
void gui_update_meso(DWORD Meso1, DWORD Meso2){
	long long int MesoHigh = Meso2;
	long long int MesoLow = Meso1;
	long long int Meso = (MesoHigh << 32) | MesoLow;

	sprintf(sMeso, "%lld", Meso);
	//AW.SetFormatString(S_MESO, "Meso:%i", Meso);//may over flow
	AW.SetFormatString(S_MESO, "Meso:%s", sMeso);//may over flow
}

void MemoryReducer(){
	while(1){
		EmptyWorkingSet(GetCurrentProcess());
		Sleep(60000);
	}
}

void gui_update_name(char *Name){
	AW.SetFormatString(S_NAME, "Character:%s", Name);
}

void gui_create(HWND hWnd){
	//AW.Button(B_TEST, "TEST", 3, 3);
	AW.StaticText(S_NAME, "Character:XXXXXXXXXXXX", 3, 3);
	AW.StaticText(S_CHANNEL, "Channel:XX", 3, 23);
	AW.StaticText(S_MAP, "Map:XXXXXXXXX", 3, 43);
	AW.StaticText(S_MESO, "Meso:XXXXXXXXXX", 3, 63);
	
	AW.Button(B_TELE1, "Exploit Map", 150, 3);
	AW.Button(B_TELE2, "FreeMarket13", 150, 23);
	AW.CheckBox(C_DO, "Meso Exploit", 150, 43);
	AW.CheckBox(C_AUTOLOGIN, "Auto Login", 150, 63, true);
	//bAutoLogin = true;
	//AW.CheckBox(C_EXPEXPLOIT, "Exp Exploit", 150, 83);
	//AW.CheckBox(C_SELFSELECT, "Self Select", 150, 103, false/*true*/);
	//AW.Button(B_SELFSELL, "Self Sell", 150, 123);
	
	AW.Button(B_SHOW, "Show Window", 150, 83);

	hook();
	//packet();
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MemoryReducer, NULL, NULL, NULL);
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lp) {
	char ClassName[256];
	DWORD ProcessId;
	GetWindowThreadProcessId(hwnd, &ProcessId);
	if(GetCurrentProcessId() == ProcessId){
		if(GetClassNameA(hwnd, ClassName, sizeof(ClassName))){
			if(strstr(ClassName, "MapleStoryClass")){
				ShowWindowAsync(hwnd, SW_RESTORE);
				SetWindowPos(hwnd, HWND_TOP , 100, 100, 0, 0, SWP_NOSIZE);
				return FALSE;
			}
		}
	}
	return TRUE;
}


void ShowMaple(){
	EnumWindows(EnumWindowsProc, NULL);
}

void gui_control(WORD wID){
	switch(wID){
	case B_SHOW:
		ShowMaple();
		break;
	case B_TEST:
//		packet_test();
		break;
	case B_SELFSELL:
		bDo = true;
		Action = 100;
		break;
	case C_DO:
		bDo = AW.CheckBoxStatus(wID);
		if(bDo){
			//AllDrop();
		}
		Action = 0;
		break;
	case B_TELE1:
		if(bTeleporting){
			bTeleporting = false;
		}
		else{
			bTeleporting = true;
			MapTeleport(680000502);
		}
		break;
	case B_TELE2:
		if(bTeleporting){
			bTeleporting = false;
		}
		else{
			bTeleporting = true;
			MapTeleport(910000013);
			//MapTeleport(180000105);
		}
		break;
	case C_AUTOLOGIN:
		bAutoLogin = AW.CheckBoxStatus(wID);
		if(bAutoLogin){
			LoginPacketCount = 0;
		}
		break;
	case C_SELFSELECT:
		bSelfSelect = AW.CheckBoxStatus(wID);
		break;
	case C_EXPEXPLOIT:
		bExpExploit = AW.CheckBoxStatus(wID);
		if(bExpExploit){
			bTeleporting = true;
			MapTeleport(865090002);
		}else{
			bTeleporting = false;
//			bEE = false;
		}
		break;
	default:
		break;
	}
}

void gui(HINSTANCE hinstDLL){
	AW.Init(hinstDLL, "Meso Exploit", 250, 100/*150*/, true, gui_create, gui_control);
}