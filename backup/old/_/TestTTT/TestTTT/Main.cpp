#include<Windows.h>
#include"AirWindow.h"
#include"Injector.h"
#include"Menu.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	Menu(hInstance);
	Injector(hInstance);
	return 0;
}
