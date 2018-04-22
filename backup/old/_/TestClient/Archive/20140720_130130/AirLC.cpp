#include"AirWindow.h"

//初期化
AirList::AirList(){
	hWndAL = NULL;
	HeaderCount = 0;
	ItemCount = 0;
	LastSelectedLine = -1;

	INITCOMMONCONTROLSEX icex;

    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
}

HWND AirList::GetHWND(){
	return hWndAL;
}

void AirList::GetItem(char *Data, int Line, int Index){
	ListView_GetItemText(hWndAL, Line, Index, Data, MaxString);
}

void AirList::ClearAll(){
	ListView_DeleteAllItems(hWndAL);
	ItemCount = 0;
	LastSelectedLine = -1;
}

void AirList::GetSelected(char *Data, int Index){
	int SelectedLine = ListView_GetNextItem(hWndAL, -1, LVNI_SELECTED);

	if((SelectedLine > ItemCount) || (SelectedLine == LastSelectedLine)){
		return;
	}

	LastSelectedLine = SelectedLine;

	ListView_GetItemText(hWndAL, SelectedLine, Index, Data, MaxString);
	return;
}

//アイテムの追加

void _stdcall AirList::_AddItem(char *_Format, DWORD *dwArgument){
	char temp[MaxHeader][MaxString] = {0};
	int i, j, k;
	LVITEMA item;

	item.mask = LVIF_TEXT;
	item.iItem = ItemCount;

	if(!hWndAL){
		return;
	}

	j=0;
	k=0;

	for(i=0; _Format[i]; i++){
		if(_Format[i] == '%'){
			switch(_Format[i+1]){
				case 'i':
					Air::INTtoChar(temp[j], dwArgument[k]);
					break;

				case 'b':
					Air::BYTEtoChar(temp[j], dwArgument[k]);
					break;

				case 'w':
					Air::WORDtoChar(temp[j], dwArgument[k]);
					break;

				case 'd':
					Air::DWORDtoChar(temp[j], dwArgument[k]);
					break;

				case 's':
					memcpy(temp[j], (char *)dwArgument[k], Air::GetStringLength((char *)dwArgument[k]));
					break;

				case 'a':
					Air::BYTEArraytoString(temp[j], dwArgument[k+1], (BYTE *)dwArgument[k]);
					k++;
					break;

				default:
					break;
			}
			j++;
			k++;
		}
	}

	for(item.iSubItem = 0; item.iSubItem < HeaderCount; item.iSubItem++){
		item.pszText = temp[item.iSubItem];

		if(!item.iSubItem){
			ListView_InsertItem(hWndAL, &item);
		}else{
			ListView_SetItem(hWndAL, &item);
		}


	}

	//自動スクロール
	ListView_EnsureVisible(hWndAL, ItemCount, true);

	ItemCount++;
	return;
}

void _stdcall AirList::AddItem(char *_Format, ...){
	_AddItem(_Format, (DWORD *)(&_Format+1));
}

//ヘッダーの追加
void AirList::AddHeader(LPSTR pszText, int cx){
	LVCOLUMNA col;

	ZeroMemory(&col, sizeof(col));

	col.mask = (LVCF_FMT|LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH);
	col.fmt = (LVCFMT_LEFT | LVCFMT_FIXED_WIDTH);
	col.pszText = pszText;
	col.cx = cx;
	col.iSubItem = HeaderCount+1;

	if(!hWndAL){
		return;
	}
	else{
		ListView_InsertColumn(hWndAL, HeaderCount+1, &col);
	}

	HeaderCount++;
	return;
}

//ListControlの作成
bool AirList::CreateListControl(HWND hWndParent, int X, int Y, int nWidth, int nHeight, bool bMS, bool bGL){
	DWORD dwExStyle;
	DWORD dwStyle = (LVS_REPORT | WS_CHILD | LVS_NOSORTHEADER | WS_BORDER | LVS_SINGLESEL| LVS_SHOWSELALWAYS );

	if(!hWndParent){
		return false;
	}

	hWndAL = CreateWindowExA(NULL, "SysListView32", NULL, dwStyle, X, Y, nWidth, nHeight, hWndParent, NULL, NULL, NULL);

	if(!hWndAL){
		return false;
	}

	//一行一括選択
	if(bMS){
		dwExStyle = ListView_GetExtendedListViewStyle(hWndAL) | LVS_EX_FULLROWSELECT;
		ListView_SetExtendedListViewStyle(hWndAL, dwExStyle);
	}

	//グリッド線
	if(bGL){
		dwExStyle = ListView_GetExtendedListViewStyle(hWndAL) | LVS_EX_GRIDLINES;
		ListView_SetExtendedListViewStyle(hWndAL, dwExStyle);
	}

	ShowWindow(hWndAL, SW_SHOW);

	return true;
}