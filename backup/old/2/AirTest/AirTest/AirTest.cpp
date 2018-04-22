#include"AirTest.h"

void Create(HWND hWnd);
void Control(WORD wID);

AirWindow AW;
void Init(HINSTANCE hInstance){
	AW.Init(hInstance, "AirTest", 400, 300, true, Create, Control);
}

#define EDIT_MAPID 101
#define BUTTON_TELEPORT 201
#define BUTTON_DISCONNECT 202

void Create(HWND hWnd){
	AW.EditBox(EDIT_MAPID, "910000000", 3, 3, 80, 0, true);
	AW.Button(BUTTON_TELEPORT, "Teleport", 90, 3);
	AW.Button(BUTTON_DISCONNECT, "Disconnect", 90, 23);
}

void Control(WORD wID){
	switch(wID){
	case BUTTON_TELEPORT:
		MapTeleport(AW.GetInt(EDIT_MAPID));
		break;
	case BUTTON_DISCONNECT:
		Disconnect();
		break;
	default:
		break;
	}
}