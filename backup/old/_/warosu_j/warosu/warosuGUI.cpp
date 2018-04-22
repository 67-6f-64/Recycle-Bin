#include"warosuGUI.h"

namespace PE{
	INT_PTR CALLBACK PacketDialog(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);
	void InitPacketDialog(HWND hwnd);
	void InitPacketList();
	BOOL GetPacket(HWND hwnd);
	void RemoveALL();
	BOOL SendPacket(HWND hwnd);
}

namespace PE{
	HWND PacketList;
	int PacketCount = 0;
	int Selected = -1;
	BOOL SHook = FALSE;
	BOOL RHook = FALSE;
}

void PE::RemoveALL(){
	ListView_DeleteAllItems(PacketList);
	PacketCount = 0;
	Selected = -1;
}


BOOL PE::GetPacket(HWND hwnd){
	int Select;

	Select = ListView_GetNextItem(PacketList, -1, LVNI_SELECTED);

	if(Select == -1 || Select == Selected){
		return FALSE;
	}

	Selected = Select;

	char Packet_STR[MAX_StrPACKET];
	char retAddr_STR[16];

	ListView_GetItemText(PacketList, Select, 3, Packet_STR, MAX_StrPACKET);
	SetDlgItemText(hwnd, EDIT_Packet, Packet_STR);

	ListView_GetItemText(PacketList, Select, 1, retAddr_STR, 16);
	SetDlgItemText(hwnd, EDIT_retAddr, retAddr_STR);
	return TRUE;
}



void  _stdcall PE::AddPacketList(char Type[], DWORD retAddr, int Size, BYTE Packet[]){
	char Packet_STR[MAX_StrPACKET], retAddr_STR[16], Size_STR[16];;
	int Count = PacketCount;
	PacketCount++;

	LVITEM item;
	item.mask = LVIF_TEXT;
	
	sprintf(Size_STR, "%d", Size);
	sprintf(retAddr_STR, "%08X", retAddr);
	
	Asm::BytetoChar(Packet, Size, Packet_STR);

	item.pszText = Type;
	item.iItem = Count;
	item.iSubItem = 0;
	ListView_InsertItem(PacketList , &item);

	item.pszText = retAddr_STR;
	item.iItem = Count;
	item.iSubItem = 1;
	ListView_SetItem(PacketList, &item);

	item.pszText = Size_STR;
	item.iItem = Count;
	item.iSubItem = 2;
	ListView_SetItem(PacketList, &item);

	item.pszText = Packet_STR;//_Packet;
	item.iItem = Count;
	item.iSubItem = 3;
	ListView_SetItem(PacketList, &item);


	//Scroll to Bottom
	ListView_EnsureVisible(PacketList, Count, true);
}




void PE::InitPacketList(){	
	LVCOLUMN col;
	col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;

	col.cx = 40;
	col.pszText = TEXT("Type");
	col.iSubItem = 0;
	ListView_InsertColumn(PacketList , 0, &col);

	col.cx = 70;
	col.pszText = TEXT("retAddr");
	col.iSubItem = 1;
	ListView_InsertColumn(PacketList, 1, &col);

	col.cx = 40;
	col.pszText = TEXT("Size");
	col.iSubItem = 2;
	ListView_InsertColumn(PacketList , 2, &col);

	col.cx = 800;
	col.pszText = TEXT("Packets");
	col.iSubItem = 3;
	ListView_InsertColumn(PacketList, 3, &col);
}



void PE::InitPacketDialog(HWND hwnd){

	//CheckDlgButton(hwnd, CHECK_SHook, BST_CHECKED);
	//SHook = TRUE;

	//CheckDlgButton(hwnd, CHECK_RHook, BST_CHECKED);
	//RHook = TRUE;

	PacketList = GetDlgItem(hwnd, LIST_Packet);

	SendMessage(PacketList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	InitCommonControls();

	InitPacketList();
}

BOOL PE::SendPacket(HWND hwnd){
	char Packet_STR[1024*3];
	GetDlgItemTextA(hwnd, EDIT_Packet, Packet_STR, 1024*3);
	PS::SendPacket(Packet_STR);
	return TRUE;
}


INT_PTR CALLBACK PE::PacketDialog(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam){
	//BYTE garbage[128] ="\x90\x90\x12\x12\x13\x00";
	switch(uMsg){
		case WM_INITDIALOG:
			InitPacketDialog(hwnd);
			break;

		
		case WM_COMMAND:
			switch(LOWORD(wParam)){

				case BUTTON_Remove:
					RemoveALL();
					break;

				case BUTTON_Send:
					SendPacket(hwnd);
					break;

				case CHECK_SHook:
							if(IsDlgButtonChecked(hwnd, LOWORD(wParam)) == BST_CHECKED){
								SHook = TRUE;
							}else{
								SHook = FALSE;
							}
							break;


				case CHECK_RHook:
							if(IsDlgButtonChecked(hwnd, LOWORD(wParam)) == BST_CHECKED){
								RHook = TRUE;
							}else{
								RHook = FALSE;
							}
							break;

			}
			break;
		
	case WM_NOTIFY:
			if(LOWORD(wParam) == LIST_Packet){
				GetPacket(hwnd);
			}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, true);
		break;
	}

	return false;
}



void PE::PacketDialog(HINSTANCE hinstDLL){
	DialogBoxA(hinstDLL, MAKEINTRESOURCE(DIALOG_Main), NULL, PacketDialog);
}