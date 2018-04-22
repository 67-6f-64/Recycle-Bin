#include<Windows.h>
#include"AIR_GUI.h"
#include"ASM.h"

/////////////////////////////////////////////
#define CHECK1 101
#define CHECK2 102
#define CHECK3 103
#define CHECK4 104
#define CHECK5 105

LRESULT CALLBACK WndProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
  switch(msg)  {
    case WM_CREATE:
		AIR::SetCheck(hwnd, CHECK1, "NoDelay", 3, 30, 200, 20);

		AIR::SetCheck(hwnd, CHECK2, "InstantFinalSlash", 3, 60, 200, 20);
		
		AIR::SetCheck(hwnd, CHECK3, "DamageEffectBlock", 3, 90, 200, 20);
		
		AIR::SetCheck(hwnd, CHECK4, "FullGodMode", 3, 120, 200, 20);

		AIR::SetCheck(hwnd, CHECK5, "InstantDrop", 3, 150, 200, 20);
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam)){
				
			case CHECK1:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					Asm::Write_code(0x00F9AE43 + 6, "01");
				}else{
					Asm::Write_code(0x00F9AE43 + 6, "00");
				}
				break;

			case CHECK2:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					//68 C4 05 D9 01 8B CE E8
					Asm::Write_code(0x00FB245C + 1, "B0 09 D9 01");
				}else{
					Asm::Write_code(0x00FB245C + 1, "C4 05 D9 01");
				}
				break;

			case CHECK3:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					//8B 44 24 18 85 C0 7E
					Asm::Write_code(0x00476D50, "C2 10 00");
					Asm::Write_code(0x004738C0, "C2 24 00");
				}else{
					Asm::Write_code(0x00476D50, "6A FF 68");
					Asm::Write_code(0x004738C0, "55 8D 6C");
				}
				break;

			case CHECK4:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					//E8 ?? ?? ?? ?? 53 8B CE E8 ?? ?? ?? ?? 33 FF 39 BE ?? ?? ?? ?? 74 0A call below
					Asm::Write_code(0x00FB0B70, "C2 04 00");
					//0F 88 ?? ?? ?? ?? 89 75 Current Function
					Asm::Write_code(0x00816730, "C2 04 00");
				}else{
					Asm::Write_code(0x00FB0B70, "6A FF 68");
					Asm::Write_code(0x00816730, "55 8D 6C");
				}
				break;

			case CHECK5:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					//0D ?? ?? ?? ?? 83 C4 ? E9 ?? ?? ?? ?? DD ?? ??
					Asm::Write_code(0x005D4525, "25");
					//89 86 ? ? ? ? E8 ? ? ? ? 89 ? ? ? ? ? 5E C2
					Asm::Write_code(0x005010BF, "90 90 90 90 90 90");
				}else{
					Asm::Write_code(0x005D4525, "0D");
					Asm::Write_code(0x005010BF, "89 86 20 21 00 00");
				}
				break;

  }
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		break;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}



void Dialog_Start(HINSTANCE hInstance){
	MSG msg;

	AIR::OpenWindow(hInstance, "Saych v318.1 JMS", 400, 300, WndProc1);

	//•Â‚¶‚È‚¢‚æ‚¤‚Éƒ‹[ƒv
	while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}



//////////////////////////////////////////////

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){return FALSE;}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Dialog_Start, hinstDLL, NULL, NULL);

	return TRUE;
}