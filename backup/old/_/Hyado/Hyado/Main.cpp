#include<Windows.h>
#include"Hyado.h"
#include"Mera.h"

void CreateSimpleControls(HWND hwnd){
	/*
	CreateStaticControl(hwnd, 0x100, "VAICAVJH", 3, 3);
	CreateEditControl(hwnd, 0x200, "", 3, 300-16-3, 200);
	CreatePushButtonControl(hwnd, 0x300, "Testasdf", 3+200+5, 300-16-3-2);
	CreateCheckBoxControl(hwnd, 0x400, "Testwwww", 3+100, 3+100);
	CreateMultiLineEditControl(hwnd, 0x210, "AAA\r\nBBB\r\nCCC\r\nDDD\r\nEEE\r\nFFF\r\nGGG", 3, 3+20, 200, 5);
	*/

	CreateCheckBoxControl(hwnd, 0x400, "HackShield Bypass for Windows 7/8.1 x64", 3, 3+15*0);
	CreateCheckBoxControl(hwnd, 0x401, "MSCRC Bypass", 3, 3+15*1);
	CreateCheckBoxControl(hwnd, 0x402, "Remove Themida CRCs", 3, 3+15*2);
	CreateCheckBoxControl(hwnd, 0x403, "VEH Debugger Detection Bypass", 3, 3+15*3);
	CreateCheckBoxControl(hwnd, 0x404, "Stop Sending ProcessList Packet", 3, 3+15*4);
	CreateCheckBoxControl(hwnd, 0x405, "Kill ASPLunchr.exe/mkd25tray.exe/autoup.exe", 3, 3+15*5);
	CreateCheckBoxControl(hwnd, 0x406, "HSless", 3, 3+15*6);

	CheckDlgButton(hwnd, 0x400, BST_CHECKED);
	CheckDlgButton(hwnd, 0x401, BST_CHECKED);
	CheckDlgButton(hwnd, 0x402, BST_CHECKED);
	CheckDlgButton(hwnd, 0x403, BST_CHECKED);
	CheckDlgButton(hwnd, 0x404, BST_CHECKED);

	CreateMultiLineEditControl(hwnd, 0x200, "Aob based Bypass+ by AIRRIDE @CCPLZ\r\nblog: http://kaedeserver.blog.fc2.com/\r\n----------------------------------------", 3, 3+15*7, 300-6, 14);

	CreatePushButtonControl(hwnd, 0x300, "Enable", 300-3-50, 3+15*6+12*14+20);
	CreatePushButtonControl(hwnd, 0x301, "Info", 3, 3+15*6+12*14+20);
}

bool IsButtonPressed = false;

void SimpleControlActions(HWND hwnd, WORD wUID){
	switch(wUID){

		case 0x400:
			if(IsDlgButtonChecked(hwnd, 0x401)){
				CheckDlgButton(hwnd, 0x406, !IsDlgButtonChecked(hwnd, wUID));
			}
			break;

		case 0x401:
			if(!IsDlgButtonChecked(hwnd, wUID)){
				CheckDlgButton(hwnd, 0x401, BST_UNCHECKED);
				CheckDlgButton(hwnd, 0x402, BST_UNCHECKED);
				CheckDlgButton(hwnd, 0x403, BST_UNCHECKED);
				CheckDlgButton(hwnd, 0x404, BST_UNCHECKED);
				CheckDlgButton(hwnd, 0x405, BST_UNCHECKED);
				CheckDlgButton(hwnd, 0x406, BST_UNCHECKED);
			}
			break;

		case 0x402:
		case 0x403:
		case 0x404:
		case 0x405:
			if(!IsDlgButtonChecked(hwnd, 0x401)) CheckDlgButton(hwnd, wUID, BST_UNCHECKED);
			break;

		case 0x406:
			if(!IsDlgButtonChecked(hwnd, 0x401)) CheckDlgButton(hwnd, wUID, BST_UNCHECKED);
			else
			CheckDlgButton(hwnd, 0x400, !IsDlgButtonChecked(hwnd, wUID));
			break;

		case 0x300:
			if(!IsButtonPressed){
				AddNewLine(hwnd, 0x200, "Success: HackShield Bypass");
				AddNewLine(hwnd, 0x200, "Success: MSCRC Bypass");
				AddNewLine(hwnd, 0x200, "Success: Removed Themida CRCs");
				AddNewLine(hwnd, 0x200, "Success: VEH Bypass");
				AddNewLine(hwnd, 0x200, "Success: Stopped ProcessList Packet");
				AddNewLine(hwnd, 0x200, "Success: Kill HS\'s exe");
				AddNewLine(hwnd, 0x200, "Success: HSless");
				AddNewLine(hwnd, 0x200, "----------------------------------------");
				AddNewLine(hwnd, 0x200, "You can close this window now");
				AddNewLine(hwnd, 0x200, "If you have any problem with this, do not post");
				AddNewLine(hwnd, 0x200, "before google or searching ccplz\'s thread");
				AddNewLine(hwnd, 0x200, "----------------------------------------");
				ScrollMultiLineEdit(hwnd, 0x200);
				SetDlgItemTextA(hwnd, 0x300, "Close");
				IsButtonPressed = true;
			}else{
				PostQuitMessage(0);
			}
			break;

		case 0x301:
			AddNewLine(hwnd, 0x200, "What is Themida CRCs?\r\nThemida CRCs are called CRC2nd/3rd");
			AddNewLine(hwnd, 0x200, "What is ASP...blabla?\r\nthese exe often cause errors\r\nand no one needs it");
			AddNewLine(hwnd, 0x200, "What is HSless?\r\nRemove HS from MS perfectly\r\nbut it causes 10 minutes d/c");
			AddNewLine(hwnd, 0x200, "Who is Taku?\r\nHe is DupeMaster");
			AddNewLine(hwnd, 0x200, "----------------------------------------");
			ScrollMultiLineEdit(hwnd, 0x200);
			break;

		default:
			break;
	}
}

LRESULT CALLBACK SimpleProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateSimpleControls(hwnd);
			break;

		case WM_COMMAND:
			SimpleControlActions(hwnd, LOWORD(wParam));
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
			
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
			CursorAnimationFixer(hwnd, msg);
			break;

		default:
			break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

//for list

LRESULT CALLBACK SimpleProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateSimpleControls(hwnd);
			break;

		case WM_COMMAND:
			SimpleControlActions(hwnd, LOWORD(wParam));
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
			
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
			CursorAnimationFixer(hwnd, msg);
			break;

		default:
			break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}


void ListList(HINSTANCE hInstance){
	CreateSimpleWindow(hInstance, "Ranarion2", 300, 300, SimpleProc);
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ListList, hInstance, NULL ,NULL);
	CreateSimpleWindow(hInstance, "Ranarion", 300, 300, SimpleProc);

	return 0;
}