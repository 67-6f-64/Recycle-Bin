#include"ListView.h"

//初期化
Air::ListView::ListView(){
	INITCOMMONCONTROLSEX icex;
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
	HeaderCount = 1;//1個目は使わない
	ItemCount = 0;
	hwndLV = NULL;
	hMainInstance = NULL;
	hMain = NULL;
}


//追記?
bool Air::ListView::AddList(LPSTR pszText[]){
	int i;
	LVITEMA item;

	item.mask = LVIF_TEXT;
	item.iItem = ItemCount;

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
	ListView_EnsureVisible(hwndLV, ItemCount, true);

	ItemCount++;

	return true;
}

//ヘッダー追加
bool Air::ListView::CreateHeader(LPSTR pszText, int cx){
	LVCOLUMNA col;
	ZeroMemory(&col, sizeof(col));

	col.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT|LVCFMT_FIXED_WIDTH;
	col.pszText = pszText;
	col.cx = cx;
	col.iSubItem = HeaderCount;

	if(!hwndLV){
		return false;
	}

	ListView_InsertColumn(hwndLV, HeaderCount, &col);

	HeaderCount++;
	return true;
}

//作成
bool Air::ListView::Init(HWND hWndParent, int X, int Y, int nWidth, int nHeight, bool bMultiSelect, bool bGridLine){
	DWORD dwExStyle;
	DWORD dwStyle = (LVS_REPORT | WS_CHILD | LVS_NOSORTHEADER | WS_BORDER);

	if(hwndLV){
		return false;
	}

	hwndLV = CreateWindowExA(NULL, "SysListView32", NULL, dwStyle, X, Y, nWidth, nHeight, hWndParent, NULL, NULL, NULL);

	if(!hwndLV){
		return false;
	}

	hMain = hWndParent;
	hMainInstance = (HINSTANCE)GetWindowLongA(hWndParent, GWL_HINSTANCE);

	//一行一括選択
	if(bMultiSelect){
		SendMessageA(hwndLV, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	}

	//グリッド線
	if(bGridLine){
		dwExStyle = ListView_GetExtendedListViewStyle(hwndLV) | LVS_EX_GRIDLINES;
		ListView_SetExtendedListViewStyle(hwndLV, dwExStyle);
	}

	ShowWindow(hwndLV, SW_SHOW);

	return true;
}

//すべて削除
bool Air::ListView::Clear(){

	if(!hwndLV){
		return false;
	}

	ListView_DeleteAllItems(hwndLV);
	ItemCount = 0;

	return true;
}

//アイテム取得
LPSTR Air::ListView::GetList(int nOffset, int uLine){
	int nLine;
	LPSTR lpString = (LPSTR)malloc(0x100);

	if(!hwndLV){
		return NULL;
	}

	if(uLine == -1){
		ListView_GetNextItem(hwndLV, -1, LVNI_SELECTED);
	}else{
		nLine = uLine;
	}

	ListView_GetItemText(hwndLV, nLine, nOffset, lpString, 0x100);

	return lpString;
}