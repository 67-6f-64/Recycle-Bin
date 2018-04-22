#include"AirWindow.h"

#define DebugEdit 0x300

namespace Air{
	bool DebugFlag = false;
	AirWindow DW;
	void GuiCreationD(HWND hwnd);
	void _stdcall _DebugMessage(char *_Format, DWORD *dwArgument);
}

void _stdcall Air::_DebugMessage(char *_Format, DWORD *dwArgument){
	char temp[0x100] = {0};
	Air::_CreateFormatString(temp, _Format, dwArgument);
	DW.AddString(DebugEdit, temp, true);
}

void Air::DebugMessage(char *_Format, ...){
	_DebugMessage(_Format, (DWORD *)(&_Format+1));
}

void Air::GuiCreationD(HWND hwnd){
	DW.CreateEditBox(DebugEdit, NULL, 3, 3, 400-6, false, 20);
	DW.ReadOnly(DebugEdit, true);
	DW.AddString(DebugEdit, "********** AirLibrary's Debug Window **********");
}

void Air::DebugWindow(HINSTANCE hInstance){
	DW.CreateSimpleWindow(hInstance, "Debug Window", 400, 300, GuiCreationD, NULL, NULL, NULL, false, 90);
	DebugFlag = true;
}


bool Air::IsDebug(){
	return DebugFlag;
}
