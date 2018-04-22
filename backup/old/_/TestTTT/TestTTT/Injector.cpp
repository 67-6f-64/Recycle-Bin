#include"Injector.h"

AirWindow IW;
AirList IL;

#define ButtonInject 0x100
#define EditInject 0x200


void DLLInjection(){
	LoadLibraryA(IW.GetString(EditInject));
}


void LoadFiles(){
	WIN32_FIND_DATA filedata;
	HANDLE hFile;
	char FilePath[0x100] ={0};

	hFile = FindFirstFileA("DLL/*.dll", &filedata);

	if(hFile == INVALID_HANDLE_VALUE){
		return;
	}
	
	do{
		Air::CreateFormatString(FilePath, "DLL/%s", filedata.cFileName);
		IL.AddItem("%s", FilePath);
	}
	while(FindNextFileA(hFile, &filedata));
}


void GuiCreationI(HWND hWnd){
	IL.CreateListControl(hWnd, 3, 3, 394, 294-20);
	IL.AddHeader("DLL File Name", 360);
	IW.CreateButton(ButtonInject, "Inject", 397-50, 297-18);
	IW.CreateEditBox(EditInject, NULL, 3, 297-18, 340);
	LoadFiles();
}

void GuiControlI(WORD wID){
	switch(wID){
		case ButtonInject:
			DLLInjection();
			break;

		default:
			break;
	}
}


void GuiNotifyI(HWND hWnd){
	char Buffer[MaxString] = {0};

	if(hWnd == IL.GetHWND()){
		IL.GetSelected(Buffer, 0);

		if(Buffer[0]){
			IW.SetString(EditInject, Buffer);
		}
	}
}


void Injector(HINSTANCE hInstance){
	IW.CreateSimpleWindow(hInstance, "Air Injector (test)", 400, 300, GuiCreationI, GuiControlI, GuiNotifyI, true);
	Air::WaitForQuit();
}