#include"Menu.h"

AirWindow MW;

#define CheckHSCRC 0x300
#define CheckMSCRC 0x301
#define CheckHSLess 0x302
#define ButtonEnable 0x100


void GuiCreationM(HWND hWnd){
	MW.CreateCheckBox(CheckHSCRC, "HSCRC Bypass", 3 , 3, true);
	MW.CreateCheckBox(CheckMSCRC, "MSCRC Bypass", 3 , 3+20, true);
	MW.CreateCheckBox(CheckHSLess, "HSLess", 3 , 3+20*2, false);
	MW.CreateButton(ButtonEnable, "Enable", 3, 3+20*3, 50);
}

void GuiControlM(WORD wID){
	switch(wID){
		case ButtonEnable:
			MW.CloseSimpleWindow();
			Sleep(1000);
			PostQuitMessage(0);//for ending waitforquit
			break;

		default:
			break;
	}
}

void Menu(HINSTANCE hInstance){
	MW.CreateSimpleWindow(hInstance, "Air Bypass", 200, 150, GuiCreationM, GuiControlM, NULL, true);
	Air::WaitForQuit();
}