#include<Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	LoadLibraryA("CE.dll");
	SuspendThread(GetCurrentThread());
	return 0;
}
