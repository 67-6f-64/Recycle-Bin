#include<Windows.h>
#include"gui.h"

gui wt;

void wt_create(HWND hWnd){
	wt.StaticText(S_MESO, "Meso:", 3 + 800, 3);
	wt.StaticText(S_MESOVALUE, "0000000000", 33 + 800, 3);
	wt.StaticText(S_MESSAGE, "’âŽ~’†(‚¾‚ÆŽv‚¤)", 3 + 800, 20);
	wt.Button(B_DELIVERY, "ŠJ‚­", 120 + 800, 3);
	wt.CheckBox(C_MESOPLZ, "Magica Rush", 3 + 800, 40);
	/*
	SetWindowLongA(Client->hWnd, GWL_STYLE, WS_CHILD | WS_POPUP);
	ShowWindow(Client->hWnd, SW_SHOW);
	SetParent(Client->hWnd, hWnd);
	SetWindowPos(Client->hWnd, HWND_TOP, 0, 0, 800, 600, NULL);
	*/
}

void wt_control(WORD wID){
	switch(wID){
	case B_DELIVERY:
		break;
	case C_MESOPLZ:
		if(wt.CheckBoxStatus(wID)){
			//gui_update_action(ACTION_GETREWARD);
		}
		else{
			//gui_update_action(ACTION_DEFAULT);
		}
		break;
	default:
		break;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	wt.Run(hInstance, "Magica", 160 + 800, 600/*60*/, wt_create, wt_control);
	return 0;
}