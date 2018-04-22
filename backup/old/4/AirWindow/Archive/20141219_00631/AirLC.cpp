#include"AirWindow.h"

namespace Air{

}


AirList::AirList(){
	HeaderCount = 0;
	CurrentRow = 0;
	CurrentCol = 0;
	SelectedRow = -1;
	SelectedCol = -1;

	INITCOMMONCONTROLSEX icex;

    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
}
/*
void AirWindow::SetFont(int nIDDlgItem){
	SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_SETFONT, (WPARAM)Air::AWFont, NULL);
}

*/
void AirList::Init(HWND hWndParent, int X, int Y, int nWidth, int nHeight){
	DWORD dwExStyle;
	DWORD dwStyle = (LVS_REPORT | WS_CHILD | LVS_NOSORTHEADER | WS_BORDER | LVS_SINGLESEL| LVS_SHOWSELALWAYS );

	hWndAL = CreateWindowExA(NULL, "SysListView32", NULL, dwStyle, X, Y, nWidth, nHeight, hWndParent, NULL, NULL, NULL);
	SendMessageA(hWndAL, WM_SETFONT, (WPARAM)Air::AWFont, NULL);
	//1行選択
	dwExStyle = ListView_GetExtendedListViewStyle(hWndAL) | LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(hWndAL, dwExStyle);

	//グリッド線
	dwExStyle = ListView_GetExtendedListViewStyle(hWndAL) | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(hWndAL, dwExStyle);
	ShowWindow(hWndAL, SW_SHOW);
}


void AirList::AddHeader(LPSTR pszText, int cx){
	LVCOLUMNA col;

	ZeroMemory(&col, sizeof(col));

	col.mask = (LVCF_FMT|LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH);
	col.fmt = (LVCFMT_LEFT | LVCFMT_FIXED_WIDTH);
	col.pszText = pszText;
	col.cx = cx;
	col.iSubItem = HeaderCount+1;

	ListView_InsertColumn(hWndAL, HeaderCount+1, &col);

	HeaderCount++;
}


void AirList::Clear(){
	CurrentRow = 0;
	CurrentCol = 0;
	SelectedRow = -1;
	SelectedCol = -1;
	ListView_DeleteAllItems(hWndAL);
}

void AirList::NewLine(){
	CurrentRow++;
	CurrentCol = 0;
}


void AirList::AddItem(LPSTR pszText){
	LVITEMA item;

	if(CurrentCol >= HeaderCount){
		NewLine();
	}

	item.mask = LVIF_TEXT;
	item.iItem = CurrentRow;
	item.iSubItem = CurrentCol;
	item.pszText = pszText;

	if(!CurrentCol){
		ListView_InsertItem(hWndAL, &item);
	}else{
		ListView_SetItem(hWndAL, &item);
	}

	ListView_EnsureVisible(hWndAL, CurrentRow, true);

	CurrentCol++;
}

void _stdcall AirList::AddFormatItem(char *Format, ...){
	char *Buffer = (char *)malloc(MaxStringAL);
	Air::_AirPrint(Buffer, Format, (LPDWORD)&Format + 1);
	AddItem(Buffer);
}

HWND AirList::GetHWND(){
	return hWndAL;
}

LPSTR AirList::GetSelectedItem(int Index){
	int SelectRow = ListView_GetNextItem(hWndAL, -1, LVNI_SELECTED);
	LPSTR lpString = (LPSTR)malloc(MaxStringAL+1);

	if((SelectedRow == SelectRow) && (SelectedCol == Index)){
		return NULL;
	}

	if(SelectRow == -1){
		return NULL;
	}

	SelectedRow = SelectRow;
	SelectedCol = Index;
	ListView_GetItemText(hWndAL, SelectedRow, SelectedCol, lpString, MaxString);
	return lpString;
}

LPSTR AirList::_GetSelectedItem(int Index){
	int SelectRow = ListView_GetNextItem(hWndAL, -1, LVNI_SELECTED);
	LPSTR lpString = (LPSTR)malloc(MaxStringAL+1);

	if(SelectRow == -1){
		return NULL;
	}

	ListView_GetItemText(hWndAL, SelectRow, Index, lpString, MaxString);
	return lpString;
}