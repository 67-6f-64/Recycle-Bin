#include"guigui.h"

DWORD SendFunction = 0x00478020;//v101.1


BOOL SendPacket(char Packet_STR[]){
	int Packet_Size;
	BYTE Packet[4096];

	Packet_Size = Asm::ChartoByte(Packet_STR, Packet);

	if(Packet_Size == -1 || Packet_Size < 2){
		return FALSE;
	}

	//created by airride^^
	_asm{
		lea eax,[Packet]
		mov ebx,[Packet_Size]
		push 0x00
		push ebx
		push eax
		push 0x00
		push esp
		call SendFunction
	}

	return TRUE;
}



BOOL GetPacketAndSend(HWND hwnd, int editid){
	char Packet_STR[1024*3];
	GetDlgItemTextA(hwnd, editid, Packet_STR, 1024*3);
	SendPacket(Packet_STR);
	return TRUE;
}


INT_PTR CALLBACK PacketDialog(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam){
	switch(uMsg){
		case WM_INITDIALOG:
			break;
		
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDC_BUTTON1:
					GetPacketAndSend(hwnd, IDC_EDIT1);
					break;
				case IDC_BUTTON2:
					GetPacketAndSend(hwnd, IDC_EDIT2);
					break;
				case IDC_BUTTON3:
					GetPacketAndSend(hwnd, IDC_EDIT3);
					break;
				case IDC_BUTTON4:
					GetPacketAndSend(hwnd, IDC_EDIT1);
					Sleep(20);
					GetPacketAndSend(hwnd, IDC_EDIT2);
					Sleep(20);
					GetPacketAndSend(hwnd, IDC_EDIT3);
					Sleep(20);
					break;
			}
			break;

	case WM_CLOSE:
		EndDialog(hwnd, true);
		break;
	}

	return false;
}



void PacketDialog(HINSTANCE hDLL){
	DialogBoxA(hDLL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, PacketDialog);
}