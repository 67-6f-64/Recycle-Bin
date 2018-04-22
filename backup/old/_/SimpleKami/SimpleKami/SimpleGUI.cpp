#include"SimpleGUI.h"
#include"SimpleKami.h"
#include"SimpleScript.h"
#include"AirFilter.h"

//Control Item IDs
#define CHECK_KamiVac 101
#define CHECK_Extra 102
#define CHECK_ItemFilter 103
#define CHECK_MobFilter 104
#define STATIC_1 1
#define EDIT_X 201
#define EDIT_Y 202
#define EDIT_CCval 203
//

namespace GUI{
	LRESULT CALLBACK SimpleGUIProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void CreateControls(HWND hwnd);
	void KamiVacSettings(HWND hwnd, BOOL Flag);
}


void GUI::CreateControls(HWND hwnd){
	Airgui::SetStatic(hwnd, STATIC_1, "          X:", 5, 5);
	Airgui::SetStatic(hwnd, STATIC_1, "          Y:", 5, 5 + 20);
	Airgui::SetStatic(hwnd, STATIC_1, "CC Interval:", 5, 5 + 20 *2);
	Airgui::SetNumberEdit(hwnd, EDIT_X, "100", 5 + 6*12, 3, 50);
	Airgui::SetNumberEdit(hwnd, EDIT_Y, "20", 5 + 6*12, 3 + 20, 50);
	Airgui::SetNumberEdit(hwnd, EDIT_CCval, "0", 5 + 6*12, 3 + 20*2, 50);
	Airgui::SetStatic(hwnd, STATIC_1, "[s]", 5 + 6*12 + 50, 5 + 20*2);
	Airgui::SetCheck(hwnd, CHECK_KamiVac, "KamiVac", 3, 63);
	Airgui::SetCheck(hwnd, CHECK_Extra, "Extra", 250-(20 + 6*12), 3);
	Airgui::SetCheck(hwnd, CHECK_MobFilter, "Mob Filter", 250-(20 + 6*12), 3+ 20);
	Airgui::SetCheck(hwnd, CHECK_ItemFilter, "Item Filter", 250-(20 + 6*12), 3+ 20*2);
	Airgui::SetStatic(hwnd, STATIC_1, "Do not check KamiVac before login", 3, 83);
	Airgui::SetStatic(hwnd, STATIC_1, "Set SP in this position", 3, 95);
	Airgui::SetStatic(hwnd, STATIC_1, "You cannot loot item", 3, 107);
}


void GUI::KamiVacSettings(HWND hwnd, BOOL Flag){

	SendDlgItemMessageA(hwnd, EDIT_X, EM_SETREADONLY, Flag, 0);
	SendDlgItemMessageA(hwnd, EDIT_Y, EM_SETREADONLY, Flag, 0);
	SendDlgItemMessageA(hwnd, EDIT_CCval, EM_SETREADONLY, Flag, 0);

	if(Flag == TRUE){
		Kami::RangeX = GetDlgItemInt(hwnd, EDIT_X, NULL, NULL);
		Kami::RangeY = GetDlgItemInt(hwnd, EDIT_Y, NULL, NULL);
		Kami::CCval = GetDlgItemInt(hwnd, EDIT_CCval, NULL, NULL);


		if(Kami::CCval < 10 && Kami::CCval > 0){
			SetDlgItemInt(hwnd, EDIT_CCval, 10, NULL);
			Kami::CCval = 10;//Default
		}

	}else{
		Kami::RangeX = 0;
		Kami::RangeY = 0;
		Kami::CCval = 0;
	}

		Kami::Action = 0;
		Kami::CC = FALSE;
		Kami::Breath = FALSE;
		Kami::oldCCval = -1;

}


LRESULT CALLBACK GUI::SimpleGUIProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

  switch(msg)  {
    case WM_CREATE:
		CreateControls(hwnd);
		SCR::AlwaysEnable();
		SCR::OtherScript(TRUE);
		CheckDlgButton(hwnd, CHECK_Extra, BST_CHECKED);
		AIF::LoadItemList();
		AIF::ItemFilter(TRUE);
		CheckDlgButton(hwnd, CHECK_ItemFilter, BST_CHECKED);
		AIF::LoadItemList2();
		AIF::ItemFilter2(TRUE);
		CheckDlgButton(hwnd, CHECK_MobFilter, BST_CHECKED);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Kami::CCTimer, NULL ,NULL ,NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Kami::BreathTimer, NULL ,NULL ,NULL);
		break;


	case WM_PAINT:
		break;


	case WM_COMMAND:
		switch(LOWORD(wParam)){
				
			case CHECK_KamiVac:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					//checked
					KamiVacSettings(hwnd, TRUE);
					Kami::WRYYY(TRUE);
				}else{
					//unchecked
					KamiVacSettings(hwnd, FALSE);
					Kami::WRYYY(FALSE);
				}
				break;
				

			case CHECK_Extra:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					//checked
					SCR::OtherScript(TRUE);
				}else{
					//unchecked
					SCR::OtherScript(FALSE);
				}
				break;
				

			case CHECK_ItemFilter:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					AIF::LoadItemList();
					AIF::ItemFilter(TRUE);
				}else{
					AIF::ItemFilter(FALSE);
				}
				break;


			case CHECK_MobFilter:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					AIF::LoadItemList2();
					AIF::ItemFilter2(TRUE);
				}else{
					AIF::ItemFilter2(FALSE);
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


void GUI::DialogInit(HINSTANCE hInstance){
	MSG msg;

	Airgui::CreateNewDialog(hInstance, "Simple fedge v97.1", 250, 120, SimpleGUIProc);

	//•Â‚¶‚È‚¢‚æ‚¤‚Éƒ‹[ƒv
	while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}