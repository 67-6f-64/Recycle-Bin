#include<Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	CreateProcessA("C:\\Nexon\\MapleStory\\v334.0_U.exe", lpCmdLine, NULL, NULL, CREATE_SUSPENDED, TRUE, NULL, NULL, &si, &pi);
	MessageBoxA(NULL, NULL, NULL, NULL);
	return 0;
}
