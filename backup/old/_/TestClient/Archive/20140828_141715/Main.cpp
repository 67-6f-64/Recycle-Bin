#include<Windows.h>
#include"AirWindow.h"

AirWindow MW;
AirList AL;

void OutOfParent(HWND hWnd){
	char Data[128] = {0};

	if(hWnd == AL.GetHWND()){
		AL.GetSelected(Data, 2);
		if(Data[0]){
			Air::DebugMessage(Data);
		}
	}
}

void GuiCR(HWND hwnd){
	MW.CreateButton(0x100, "COPY", 3, 300-20); 
	AL.CreateListControl(hwnd, 0, 0, 400, 300-20);
	AL.AddHeader("Header1", 100);
	AL.AddHeader("Header2", 100);
	AL.AddHeader("Header3", 100);
	AL.AddHeader("Header4", 100);
	AL.AddItem("%d%s%a", 0x1234, "‚ ‚¢‚¤‚¦‚¨" ,"\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD", 10);
	AL.AddItem("%d%s%a", 0x1234, "‚ ‚¢‚¤‚¦‚¨" ,"\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD", 10);
	AL.AddItem("%d%s%a", 0x1234, "‚ ‚¢‚¤‚¦‚¨" ,"\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD", 10);
	AL.AddItem("%d%s%a", 0x1234, "‚ ‚¢‚¤‚¦‚¨" ,"\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD", 10);
	AL.AddItem("%d%s%a", 0x1234, "‚ ‚¢‚¤‚¦‚¨" ,"\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD", 10);
	AL.AddItem("%d%s%a", 0x1234, "‚ ‚¢‚¤‚¦‚¨" ,"\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD\xAB\xCD", 10);

	char Data[128];

	AL.GetItem(Data, 0, 1);
	Air::DebugMessage(Data);
	
}


void GuiCO(WORD wID){
	switch(wID){
		default:
			break;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	Air::DebugWindow(hInstance);
	Air::DebugMessage("%s %d", "‚Ä‚·‚Æ", 0x124);
	Air::DebugMessage("%a ‚ ‚¢‚¤‚¦‚¨ %d", "\x12\x23\x45\x45", 3, 0x123);
	MW.CreateSimpleWindow(hInstance, "TestClient", 400, 300, GuiCR, GuiCO, OutOfParent, true);

	Air::WaitForQuit();

	return 0;
}
