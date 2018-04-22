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
#define B_DELIVERY 201
#define B_POST 202
#define C_MESOPLZ 301
#define E_NAME 401
#define E_MESO 402

DWORD MesoAction;
void gui_update_action(DWORD Action){
	MesoAction = Action;
	switch(MesoAction){
	case ACTION_GETREWARD:
		wt.SetString(S_MESSAGE, "ïÒèVälìæíÜ");
		break;

	case ACTION_OPENSHOP:
		wt.SetString(S_MESSAGE, "îÑãpäJéníÜ");
		break;

	case ACTION_SELLREWARD:
		wt.SetString(S_MESSAGE, "îÑãpíÜ");
		break;

	case ACTION_CLOSESHOP:
		wt.SetString(S_MESSAGE, "îÑãpèIóπíÜ");
		break;

	case ACTION_CLOSINGSHOP:
		wt.SetString(S_MESSAGE, "îÑãpèIóπ");
		break;

	case ACTION_MAPTELEPORT:
		wt.SetString(S_MESSAGE, "à⁄ìÆíÜ");
		break;

	case ACTION_MAPTELEPORTING:
		wt.SetString(S_MESSAGE, "êÿífíÜ");
		break;

	case ACTION_MESOMAX:
		wt.SetString(S_MESSAGE, "çÃå@èIóπ");
		break;

	case ACTION_EXIT:
		wt.SetString(S_MESSAGE, "ê≥èÌèIóπ");
		break;

	case ACTION_EXCEPTION:
		wt.SetString(S_MESSAGE, "í‚é~íÜ(ó·äOî≠ê∂)");
		break;

	case ACTION_DEFAULT:
		wt.SetString(S_MESSAGE, "í‚é~íÜ");
		break;

	default:
		wt.SetString(S_MESSAGE, "ñ¢íËã`");
		break;
	}
}

void gui_update_meso(ULONGLONG Meso){
	char meso[32];
	sprintf(meso, "%lld", Meso);
	wt.SetString(S_MESOVALUE, meso);
}

void wt_create(HWND hWnd){
	//wt.StaticText(S_MESO, "ã‡äz", 3, 3);
	//wt.StaticText(S_NAME, "ñºëO", 3, 20);
	//wt.EditBox(E_MESO, "1000000", 30, 3);
	//wt.EditBox(E_NAME, "ìcä}ñÿÉVÉYÉN", 30, 20);
	wt.StaticText(S_MESO, "Meso:", 3, 3);
	wt.StaticText(S_MESOVALUE, "0000000000", 33, 3);
	wt.StaticText(S_MESSAGE, "í‚é~íÜ(ÇæÇ∆évÇ§)", 3, 20);
	wt.Button(B_DELIVERY, "äJÇ≠", 120, 3);
	//wt.Button(B_POST, "ëóÇÈ", 120, 20);
	wt.CheckBox(C_MESOPLZ, "Magica Rush", 3, 40);
}

void wt_control(WORD wID){
	switch(wID){
	case B_DELIVERY:
		SendPacketQueue(psOpen);//Deli
		break;
	case B_POST:
		bMagica = true;
		break;
	case C_MESOPLZ:
		if(wt.CheckBoxStatus(wID)){
			gui_update_action(ACTION_GETREWARD);
		}
		else{
			gui_update_action(ACTION_DEFAULT);
		}
		break;
	default:
		break;
	}
}

void gui_wt(HINSTANCE hInstance){
	wt.Run(hInstance, "Magica Rush", 160, 60, wt_create, wt_control);
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
