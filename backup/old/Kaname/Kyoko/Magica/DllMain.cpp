#include<Windows.h>
#include"gui.h"
#include"Magica.h"


//func
HWND hMagica = 0;
void Emebed(){
	SetWindowLongA(Client->hWnd, GWL_STYLE, WS_CHILD | WS_POPUP);
	ShowWindow(Client->hWnd, SW_SHOW);
	SetParent(Client->hWnd, hMagica);
	SetWindowPos(Client->hWnd, HWND_TOP, 0, 0, 800, 600, NULL);
}
//

gui wt;

#define C_MESOPLZ 301
#define C_MESOPLZ2 302
#define B_CHANCE 101
void wt_create(HWND hWnd){
	hMagica = hWnd;
	wt.CheckBox(C_MESOPLZ, "Magica", 3 + 800, 40);
	wt.CheckBox(C_MESOPLZ2, "Magica2", 3 + 800, 60);
	//wt.Button(B_CHANCE, "Chance", 3 + 800, 20);
	Emebed();
}

void wt_control(WORD wID){
	switch(wID){
	case C_MESOPLZ:
		NewLogin = 0;
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
	case B_CHANCE:
		Emebed();
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
