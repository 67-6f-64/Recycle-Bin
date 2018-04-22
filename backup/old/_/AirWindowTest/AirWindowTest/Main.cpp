#include<Windows.h>
#include"AirWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	AirWindow AW;

	AW.CreateSimpleWindow(hInstance, "AirWindow", 400, 300, NULL, NULL, NULL, true);

	Air::WaitForQuit();

	return 0;
}
