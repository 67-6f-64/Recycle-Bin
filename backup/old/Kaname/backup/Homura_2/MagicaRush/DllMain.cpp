#include<Windows.h>
#include"gui.h"
#include"Magica.h"
#include"Packet.h"
#include"Action.h"

gui wt;

#define S_MESO 101
#define S_NAME 102
#define S_MESSAGE 103
#define S_MESOVALUE 104
#define S_TIME 105
#define B_DELIVERY 201
#define B_POST 202
#define C_MESOPLZ 301
#define C_MESOPLZ2 302
#define E_NAME 401
#define E_MESO 402

DWORD MesoAction;
void gui_update_action(DWORD Action){
	MesoAction = Action;
	switch(MesoAction){
	case ACTION_GETREWARD:
		wt.SetString(S_MESSAGE, "•ñVŠl“¾’†");
		break;

	case ACTION_OPENSHOP:
		wt.SetString(S_MESSAGE, "”„‹pŠJŽn’†");
		break;

	case ACTION_SELLREWARD:
		wt.SetString(S_MESSAGE, "”„‹p’†");
		break;

	case ACTION_CLOSESHOP:
		wt.SetString(S_MESSAGE, "”„‹pI—¹’†");
		break;

	case ACTION_CLOSINGSHOP:
		wt.SetString(S_MESSAGE, "”„‹pI—¹");
		break;

	case ACTION_MAPTELEPORT:
		wt.SetString(S_MESSAGE, "ˆÚ“®’†");
		break;

	case ACTION_MAPTELEPORTING:
		wt.SetString(S_MESSAGE, "Ø’f’†");
		break;

	case ACTION_MESOMAX:
		wt.SetString(S_MESSAGE, "ÌŒ@I—¹");
		break;

	case ACTION_OPENDELIVERY:
		wt.SetString(S_MESSAGE, "‘î”zŠJŽn");
		break;
		
	case ACTION_DELIVERY:
	case ACTION_DELIVERING:
		wt.SetString(S_MESSAGE, "‘î”z’†1");
		break;
	case ACTION_POSTING:
		wt.SetString(S_MESSAGE, "‘î”z’†2");
		break;

	case ACTION_CHARMAX:
		wt.SetString(S_MESSAGE, "‘î”zŠ®—¹");
		break;

	case ACTION_EXIT:
		wt.SetString(S_MESSAGE, "³íI—¹");
		break;

	case ACTION_EXCEPTION:
		wt.SetString(S_MESSAGE, "’âŽ~’†(—áŠO”­¶)");
		break;

	case ACTION_DEFAULT:
		wt.SetString(S_MESSAGE, "’âŽ~’†");
		break;

	default:
		wt.SetString(S_MESSAGE, "–¢’è‹`");
		break;
	}
}

void gui_update_meso(ULONGLONG Meso){
	char meso[32];
	sprintf(meso, "%lld", Meso);
	wt.SetString(S_MESOVALUE, meso);
}

void gui_update_name(char *Name){
	SYSTEMTIME stTime;
	char ltime[32];
	wt.SetString(S_NAME, Name);
	GetLocalTime(&stTime);
	sprintf(ltime, "%02d/%02d %02d:%02d", stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute);
	wt.SetString(S_TIME, ltime);
}

void wt_create(HWND hWnd){
	wt.StaticText(S_MESO, "Meso:", 3 + 800, 3);
	wt.StaticText(S_MESOVALUE, "0000000000", 33 + 800, 3);
	wt.StaticText(S_MESSAGE, "’âŽ~’†(‚¾‚ÆŽv‚¤)", 3 + 800, 20);
	
	wt.StaticText(S_NAME, "no data      ", 3 + 800, 80);
	wt.StaticText(S_TIME, "00/00 00:00", 3 + 800, 100);
	wt.Button(B_DELIVERY, "ŠJ‚­", 120 + 800, 3);
	wt.CheckBox(C_MESOPLZ, "Magica Rush", 3 + 800, 40);
	wt.CheckBox(C_MESOPLZ2, "Magica Rush (K)", 3 + 800, 60);

	SetWindowLongA(Client->hWnd, GWL_STYLE, WS_CHILD | WS_POPUP);
	ShowWindow(Client->hWnd, SW_SHOW);
	SetParent(Client->hWnd, hWnd);
	SetWindowPos(Client->hWnd, HWND_TOP, 0, 0, 800, 600, NULL);
}

void wt_control(WORD wID){
	switch(wID){
	case C_MESOPLZ:
		if(wt.CheckBoxStatus(wID)){
			bMagica = true;
			gui_update_action(ACTION_GETREWARD);
		}
		else{
			gui_update_action(ACTION_DEFAULT);
			bMagica = false;
		}
		break;
	case C_MESOPLZ2:
		if(wt.CheckBoxStatus(wID)){
			bMagicaK = true;
		}
		else{
			bMagicaK = false;
		}
		break;
	case B_DELIVERY:
		gui_update_action(ACTION_OPENDELIVERY);
		break;
	default:
		break;
	}
}

void gui_wt(HINSTANCE hInstance){
	wt.Run(hInstance, "Magica Rush", 160 + 800, 600/*60*/, wt_create, wt_control);
}

void start(HINSTANCE hinstDLL){
	magica();
	gui_wt(hinstDLL);
	TerminateProcess(GetCurrentProcess(), 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, hinstDLL, NULL ,NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}
