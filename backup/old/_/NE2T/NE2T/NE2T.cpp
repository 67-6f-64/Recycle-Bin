#include"NE2T.h"

HINSTANCE hNE2T;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow){

	hNE2T=hInstance;

	DialogBox(hNE2T, MAKEINTRESOURCE(Dialog_NE2T), NULL, NE2TDialog);

	return 0;
}