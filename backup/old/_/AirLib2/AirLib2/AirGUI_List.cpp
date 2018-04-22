#include"AirGUI_List.h"

ListView::ListView(){
	hwndLV = 0;
	HeaderCount = 0;
	ListCount = 0;
	Select = 0;
	Selected = 1337;
	INITCOMMONCONTROLSEX icex;
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
}

void ListView::Clear(){
	ListView_DeleteAllItems(hwndLV);
	ListCount = 0;
	Select = 0;
	Selected = 1337;
}
HWND ListView::GetHWND(){
	return hwndLV;
}

bool ListView::GetSelectedItem(int ItemNum, HWND hDlg, int nIDDlgItem){
	Select = ListView_GetNextItem(hwndLV, -1, LVNI_SELECTED);

	if(Select == -1 || Select == Selected){
		return false;
	}

	Selected = Select;

	ZeroMemory(lpTemporary, Temporary_String_Max_Length);
	ListView_GetItemText(hwndLV, Select, ItemNum, lpTemporary, Temporary_String_Max_Length);
	SetDlgItemText(hDlg, nIDDlgItem, lpTemporary);
	return true;
}

bool _stdcall ListView::_AddItem(LPSTR *pszText){
	int i;

	LVITEMA item;
	item.mask = LVIF_TEXT;
	item.iItem = ListCount;

	if(!hwndLV){
		return false;
	}

	for(i=0; i<HeaderCount; i++){
		item.iSubItem = i;
		item.pszText = pszText[i];

		if(!i){
			ListView_InsertItem(hwndLV, &item);
		}else{
			ListView_SetItem(hwndLV, &item);
		}

	}

	//自動スクロール
	ListView_EnsureVisible(hwndLV, ListCount, true);

	ListCount++;

	return true;
}

bool ListView::CreateHeader(LPSTR pszText, int cx){
	LVCOLUMNA col;
	ZeroMemory(&col, sizeof(col));

	col.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT|LVCFMT_FIXED_WIDTH;
	col.pszText = pszText;
	col.cx = cx;
	col.iSubItem = HeaderCount+1;

	if(!hwndLV){
		return false;
	}

	ListView_InsertColumn(hwndLV, HeaderCount+1, &col);

	HeaderCount++;

	return true;
}

HWND ListView::CreateSimpleListView(HWND hWndParent, int X, int Y, int nWidth, int nHeight, bool bMultiSelect, bool bGridLine){
	DWORD dwExStyle;
	DWORD dwStyle = (LVS_REPORT | WS_CHILD | LVS_NOSORTHEADER | WS_BORDER | LVS_SINGLESEL);

	if(!hWndParent){
		return false;
	}

	hwndLV = CreateWindowExA(NULL, "SysListView32", NULL, dwStyle, X, Y, nWidth, nHeight, hWndParent, NULL, NULL, NULL);

	if(!hwndLV){
		return false;
	}

	//一行一括選択
	if(bMultiSelect){
		//SendMessageA(hwndLV, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
		dwExStyle = ListView_GetExtendedListViewStyle(hwndLV) | LVS_EX_FULLROWSELECT;
		ListView_SetExtendedListViewStyle(hwndLV, dwExStyle);
	}

	//グリッド線
	if(bGridLine){
		dwExStyle = ListView_GetExtendedListViewStyle(hwndLV) | LVS_EX_GRIDLINES;
		ListView_SetExtendedListViewStyle(hwndLV, dwExStyle);
	}

	ShowWindow(hwndLV, SW_SHOW);

	return hwndLV;
}