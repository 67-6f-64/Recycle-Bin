#include"RanarionGUI.h"
#include"HackShield.h"
#include"MemoryHacks.h"

bool IsButtonPressed = false;

void CreateSimpleControls(HWND hwnd){

	CreateCheckBoxControl(hwnd, 0x400, "HackShield Bypass for Windows 7/8.1 x64", 3, 3+15*0);
	CreateCheckBoxControl(hwnd, 0x401, "MSCRC Bypass", 3, 3+15*1);
	CreateCheckBoxControl(hwnd, 0x402, "Remove Themida CRCs", 3, 3+15*2);
	CreateCheckBoxControl(hwnd, 0x403, "VEH Debugger Detection Bypass", 3, 3+15*3);
	CreateCheckBoxControl(hwnd, 0x404, "Stop Sending ProcessList Packet", 3, 3+15*4);
	//CreateCheckBoxControl(hwnd, 0x405, "Kill ASPLunchr.exe/mkd25tray.exe/autoup.exe", 3, 3+15*5);
	CreateCheckBoxControl(hwnd, 0x406, "HSless", 3, 3+15*6);

	CheckDlgButton(hwnd, 0x400, BST_CHECKED);
	CheckDlgButton(hwnd, 0x401, BST_CHECKED);
	CheckDlgButton(hwnd, 0x402, BST_CHECKED);
	CheckDlgButton(hwnd, 0x403, BST_CHECKED);
	CheckDlgButton(hwnd, 0x404, BST_CHECKED);

	CreateMultiLineEditControl(hwnd, 0x200, "Aob based Bypass+ by AIRRIDE @CCPLZ\r\n----------------------------------------", 3, 3+15*7, 300-6, 14);

	CreatePushButtonControl(hwnd, 0x300, "Enable", 300-3-50, 3+15*6+12*14+20);
	CreatePushButtonControl(hwnd, 0x301, "Info", 3, 3+15*6+12*14+20);
}

void EnableHacks(HWND hwnd){
	if(IsDlgButtonChecked(hwnd, 0x400)){
		if(HackShieldBypass()) AddNewLine(hwnd, 0x200, "Success: HackShield Bypass");
		else AddNewLine(hwnd, 0x200, "Failure: HackShield Bypass");
	}

	if(IsDlgButtonChecked(hwnd, 0x401)){
		if(MSCRCBypass()) AddNewLine(hwnd, 0x200, "Success: MSCRC Bypass");
		else AddNewLine(hwnd, 0x200, "Failure: MSCRC Bypass");
	}

	if(IsDlgButtonChecked(hwnd, 0x402)){
		if(RemoveThemidaCRCs()) AddNewLine(hwnd, 0x200, "Success: Remove Themida CRCs");
		else AddNewLine(hwnd, 0x200, "Failure: Remove Themida CRCs");
	}

	if(IsDlgButtonChecked(hwnd, 0x403)){
		if(VEHBypass()) AddNewLine(hwnd, 0x200, "Success: VEH Bypass");
		else AddNewLine(hwnd, 0x200, "Failure: VEH Bypass");
	}
	
	if(IsDlgButtonChecked(hwnd, 0x404)){
		if(StopProcessListPacket()) AddNewLine(hwnd, 0x200, "Success: Stop ProcessList Packet");
		else AddNewLine(hwnd, 0x200, "Failure: Stop ProcessList Packet");
	}
	
	if(IsDlgButtonChecked(hwnd, 0x405)){
		if(KillProcesses()) AddNewLine(hwnd, 0x200, "Success: Kill HS\'s exe");
		else AddNewLine(hwnd, 0x200, "Failure: Kill HS\'s exe");
	}
	
	if(IsDlgButtonChecked(hwnd, 0x406)){
		if(HSless()) AddNewLine(hwnd, 0x200, "Success: HSless");
		else AddNewLine(hwnd, 0x200, "Failure: HSless");
	}
	AddNewLine(hwnd, 0x200, "----------------------------------------");
	AddNewLine(hwnd, 0x200, "You can close this window now");
	AddNewLine(hwnd, 0x200, "If you have any problem with this, do not post");
	AddNewLine(hwnd, 0x200, "before google or searching ccplz\'s thread");
	AddNewLine(hwnd, 0x200, "----------------------------------------");
	ScrollMultiLineEdit(hwnd, 0x200);
	SetDlgItemTextA(hwnd, 0x300, "Close");
	IsButtonPressed = true;
}



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
				EnableHacks(hwnd);
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

void StartDialog(HINSTANCE hInstance){
	CreateSimpleWindow(hInstance, "Ranarion v0.2", 300, 300, SimpleProc);

}