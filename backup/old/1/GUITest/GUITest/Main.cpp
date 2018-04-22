#include<Windows.h>
#include"gui.h"

gui pe;
list log;

void gui_create(HWND hWnd){
	pe.StaticText(1, "Static Text 2222\nStatic Text 11", 3, 3);
	pe.CheckBox(2, "Check Box1", 3, 3 + 30);
	pe.EditBox(3, "Edit Box1", 3, 3 + 60, 100);
	pe.Button(4, "Button", 3, 3 + 90);
	log.Set(hWnd, 3, 120);
	log.AddHeader("Header1", 100);
	log.AddHeader("Header2", 100);
	log.AddHeader("Header3", 100);
	log.AddItem(0, "item1");
	log.AddItem(1, "item2");
	log.AddItem(2, "item3");
	//log.Clear();
	log.AddItem(0, "item4");
	log.AddItem(1, "item5");
	log.AddItem(2, "item6");
}

void gui_control(WORD wID){
	switch(wID){
	case 1:
		break;
	case 2:
		if(pe.CheckBoxStatus(wID)){
			pe.ReadOnly(3);
		}else{
			pe.ReadOnly(3, false);
		}
		break;
	case 3:
		break;
	default:
		break;
	}
}

void gui_nofity(HWND hWnd){
	if(hWnd == log.GetHWND()){
		LPSTR lps1 = log.GetSelectedItem(0);
		LPSTR lps2 = log.GetSelectedItem(1);
		if(lps1){
			MessageBoxA(NULL, lps1, lps2, NULL);
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	pe.Run(hInstance, "PE Test", 800, 600, gui_create, gui_control, gui_nofity);
	return 0;
}