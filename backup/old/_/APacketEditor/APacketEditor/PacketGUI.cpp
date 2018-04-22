#include"PacketGUI.h"
#include"PacketHook.h"
#include"SendPacket.h"

ListView LV;
DWORD IsAdvanceLog = 0;
DWORD pReturn;
WORD pHeader;
int pSizeMin;
int pSizeMax;

void AdvanceLog(){

}


bool IsSendHook = true;
bool IsRecvHook = true;

bool _stdcall AddPacket(bool bType, DWORD dwReturn, DWORD dwSize, BYTE Packet[]){
	LPSTR pszText[4] = {(LPSTR)malloc(0x10), (LPSTR)malloc(0x10), (LPSTR)malloc(0x10), (LPSTR)malloc(dwSize*3)};

	if(bType && IsSendHook){
		pszText[0] = "Send";
	}
	else if(!bType && IsRecvHook){
		pszText[0] = "Recv";
	}
	else{
		return true;
	}

	Air::DWORDtoChar(pszText[1], dwReturn);
	Air::INTtoChar(pszText[2], dwSize);
	Air::BYTEArraytoString(pszText[3], dwSize, Packet);
	LV._AddItem(pszText);
	return true;
}

void CreatePacketControls(HWND hWndParent){
	Air::CreateCheckBox(hWndParent, 0x100, "SendHook", 3+700+3, 3+20*0, true);
	Air::CreateCheckBox(hWndParent, 0x101, "RecvHook", 3+700+3, 3+20*1, true);
	Air::CreateCheckBox(hWndParent, 0x102, "BlockList", 3+700+3, 3+20*2);
	Air::CreateCheckBox(hWndParent, 0x103, "Spammer", 3+700+3, 3+20*3);
	Air::CreateCheckBox(hWndParent, 0x104, "Scripter", 3+700+3, 3+20*4);
	Air::CreateEdit(hWndParent, 0x200, NULL, 3, 600-3-20, 700);
	Air::CreateButton(hWndParent, 0x300, "Send", 3+700+3+10, 600-3-20, 50);
	Air::CreateEdit(hWndParent, 0x201, NULL, 3+700+3+10, 600-3-20*2, 70);
	Air::CreateButton(hWndParent, 0x301, "Clear", 3+700+3+10, 600-3-20*4, 50);

	LV.CreateSimpleListView(hWndParent, 3, 3, 700, 600-40);
	LV.CreateHeader("Type", 40);
	LV.CreateHeader("Return", 90);
	LV.CreateHeader("Size", 50);
	LV.CreateHeader("Packet Data", 480);
}

void PacketCommands(HWND hwnd, WORD wID){
	switch(wID){
		case 0x300:
			//Pa::SendPacketString(Air::GetString(hwnd, 0x200));
			vSendPacket(Air::GetString(hwnd, 0x200));
			break;

		case 0x301:
			LV.Clear();
			break;

		case 0x100:
			IsSendHook = IsDlgButtonChecked(hwnd, wID);
			break;

		case 0x101:
			IsRecvHook = IsDlgButtonChecked(hwnd, wID);
			break;

		case 0x103:
			ShowWindow(Air::hWndList[1], IsDlgButtonChecked(hwnd, wID));
			break;

		default:
			break;
	}
}

LRESULT CALLBACK PacketProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreatePacketControls(hwnd);
			break;

		case WM_COMMAND:
			PacketCommands(hwnd, LOWORD(wParam));
			break;

		case WM_NOTIFY:
				if(LOWORD(wParam) == 0){
					LV.GetSelectedItem(3, hwnd, 0x200);
					LV.GetSelectedItem(1, hwnd, 0x201);
				}
			break;

		default:
			break;
	}

	return Air::_DefWindowProcA(hwnd, msg, wParam, lParam);
}

void CreateSpammerControls(HWND hWndParent){
	Air::CreateEdit(hWndParent, 0x200, NULL, 3, 3, 400-6, false, 20);
	Air::CreateStaticText(hWndParent, 0x300, "Loop", 3, 300-3-20*2);
	Air::CreateEdit(hWndParent, 0x201, "10", 3+30, 300-3-20*2, 50, true);
	Air::CreateStaticText(hWndParent, 0x301, "Delay", 3+100, 300-3-20*2);
	Air::CreateEdit(hWndParent, 0x202, "50", 3+100+40, 300-3-20*2, 50, true);
	Air::CreateCheckBox(hWndParent, 0x100, "Spam!", 3+300, 300-3-20);
	WriteHook();
}

LRESULT CALLBACK SpammerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateSpammerControls(hwnd);
			break;

		case WM_COMMAND:
			//Commands(hwnd, LOWORD(wParam));
			break;

		default:
			break;
	}

	return Air::_DefWindowProcA(hwnd, msg, wParam, lParam);
}


void APacketEditor(HINSTANCE hInstance){
	Air::SetMainInstance(hInstance);
	Air::CreateSimpleWindow(hInstance, "A Packet Editor for EMS v102.1", 800, 600, PacketProc);
	Air::CreateSimpleWindow(hInstance, "Multi Line Spammer", 400, 300, SpammerProc, false);//1
	Air::WaitForQuit();
}