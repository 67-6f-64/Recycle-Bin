#include<Windows.h>
#include"gui.h"
#include"Magica.h"

gui wt;

#define C_MESOPLZ 301
#define C_MESOPLZ2 302

void wt_create(HWND hWnd){
	wt.CheckBox(C_MESOPLZ, "Magica", 3 + 800, 40);
	wt.CheckBox(C_MESOPLZ2, "Magica2", 3 + 800, 60);

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
		}
		else{
			bMagica = false;
		}
		break;
	case C_MESOPLZ2:
		if(wt.CheckBoxStatus(wID)){
			bMagica2 = true;
		}
		else{
			bMagica2 = false;
		}
		break;
	default:
		break;
	}
}

void gui_wt(HINSTANCE hInstance){
	wt.Run(hInstance, "Magica", 160 + 800, 600/*60*/, wt_create, wt_control);
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
