#include"AirWindow.h"
#pragma comment(lib, "AirWindow.lib")

AirWindow BW;

#define EDIT_LOG 50
#define CHECK_AUTOCLOSE 60

bool bAutoClose = true;

void AirBypass(DWORD ThreadId){
	BW.AddString(EDIT_LOG, "HackShieldのロードを待っています...\r\n");
	Sleep(3000);
	BW.AddString(EDIT_LOG, "HackShield Bypass 完了\r\n");
	Sleep(3000);
	if(bAutoClose){
		BW.UnInit();
	}
}

void Create(HWND hWnd){
	BW.EditBox(EDIT_LOG, "Air Bypass for JMS (HS v5.6.34.449) by リレミト\r\n", 3, 3, 394, 20);
	BW.CheckBox(CHECK_AUTOCLOSE, "自動終了", 3, 250, bAutoClose);
	BW.ReadOnly(EDIT_LOG);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AirBypass, NULL, NULL, NULL);
}

void Control(WORD wID){
	switch(wID){
		case CHECK_AUTOCLOSE:
			bAutoClose = BW.CheckBoxStatus(wID);
			break;

		default:
			break;
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	BW.Init(hInstance, "Air Bypass", 400, 300, true, Create, Control);
	return 0;
}
