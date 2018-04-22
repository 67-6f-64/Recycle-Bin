#include<Windows.h>
#include<Richedit.h>
#include<CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

#if defined _M_IX86
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include"gui.h"

void gui::Run(HINSTANCE hInstance, LPSTR lpWindowName, int nWidth, int nHeight, void (*create)(HWND), void (*control)(WORD), void (*notify)(HWND)){
	WNDCLASSEXA wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEXA));

	InitCommonControls();

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc =  WindowProc;
	wc.lpszClassName = lpWindowName;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClassExA(&wc)){
		return;
	}

	hInstanceAW = hInstance;
	createAW = create;
	controlAW = control;
	notifyAW = notify;

	CreateWindowExA(NULL, lpWindowName, lpWindowName, (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, this);

	if(hWndAW){
		AdjustWindow(nWidth, nHeight);

		MSG lpMsg;
		while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
			TranslateMessage(&lpMsg);
			DispatchMessageA(&lpMsg);
		}

		SendMessageA(hWndAW, WM_CLOSE, 0, 0);
		UnregisterClassA(lpWindowName, hInstance);
	}
}

void gui::AdjustWindow(int nWidth, int nHeight){
	RECT W_RECT, C_RECT;
	//枠を含めたウィンドウの角の座標を取得
	GetWindowRect(hWndAW, &W_RECT);
	//枠を含めない(クライアント領域)の角の座標を取得
	GetClientRect(hWndAW, &C_RECT);
	//指定サイズに変更
	SetWindowPos(hWndAW, HWND_TOP, NULL, NULL, (nWidth + (W_RECT.right-W_RECT.left)-(C_RECT.right-C_RECT.left)), (nHeight + (W_RECT.bottom-W_RECT.top)-(C_RECT.bottom-C_RECT.top)), SWP_NOMOVE);
	ShowWindow(hWndAW, SW_SHOW);
}

HCURSOR CursorAW = LoadCursorA(NULL, IDC_ARROW);
void gui::FixCursor(HWND hwnd, UINT uMsg){
		if(GetFocus() != hwnd){
		switch(uMsg){
			case WM_MOUSEMOVE://マウスがメインウィンドウ上を移動
				SetCursor(CursorAW);//カーソルを通常のアイコンに変更
				break;

			case WM_LBUTTONUP://メインウィンドウ上でクリックがされた
				SetFocus(hwnd);
				break;

			default:
				break;
		}
	}
}

LRESULT CALLBACK gui::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		switch(uMsg){
			case WM_CREATE:
				SetWindowLongA(hwnd, GWL_USERDATA, (LONG)(((CREATESTRUCT *)(lParam))->lpCreateParams));
				((gui *)(((CREATESTRUCT *)(lParam))->lpCreateParams))->SetHWND(hwnd);
				if(((gui *)(((CREATESTRUCT *)(lParam))->lpCreateParams))->createAW){
					((gui *)(((CREATESTRUCT *)(lParam))->lpCreateParams))->createAW(hwnd);
				}
				break;

			case WM_COMMAND:
				if(((gui *)(GetWindowLongA(hwnd, GWL_USERDATA)))->controlAW){
					((gui *)(GetWindowLongA(hwnd, GWL_USERDATA)))->controlAW(LOWORD(wParam));
				}
				break;

			case WM_NOTIFY:
				if(((gui *)(GetWindowLongA(hwnd, GWL_USERDATA)))->notifyAW){
					((gui *)(GetWindowLongA(hwnd, GWL_USERDATA)))->notifyAW(GetFocus());
				}
				break;

			case WM_DESTROY:
					PostQuitMessage(0);
				break;

			default:
				FixCursor(hwnd, uMsg);
				break;
		}

	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void gui::SetHWND(HWND hWnd){
	hWndAW = hWnd;
}

//SubWindow
HFONT FontAW = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "MS Gothic");
void gui::SetFont(int nIDDlgItem){
	SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_SETFONT, (WPARAM)FontAW, NULL);
}

int gui::GetWidth(LPSTR lpWindowName){
	int i = 0, iCount = 0, iMaxCount = 0;
	while(lpWindowName[i]){
		if(lpWindowName[i++] == '\n'){
			if(iMaxCount < iCount){
				iMaxCount = iCount;
			}
			iCount = 0;
		}else{
			iCount++;
		}
	}
	if(iMaxCount < iCount){
		iMaxCount = iCount;
	}
	return iMaxCount*6;
}


int gui::GetHeight(LPSTR lpWindowName){
	int i = 0, iLine = 1;
	while(lpWindowName[i]){
		if(lpWindowName[i++] == '\n'){
			iLine++;
		}
	}
	return iLine*12;
}

void gui::StaticText(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, int nWidth){
	int nHeight = GetHeight(lpWindowName);
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT);

	if(nWidth == AUTORESIZE){
		nWidth = GetWidth(lpWindowName);
	}

	if(CreateWindowExA(NULL, "STATIC", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		SetFont(nIDDlgItem);
	}
}

void gui::CheckBox(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, bool bCheck, int nWidth){
	int nHeight = GetHeight(lpWindowName) + 1;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX);

	if(nWidth == AUTORESIZE){
		nWidth = GetWidth(lpWindowName) + 15;
	}

	if(CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		SetFont(nIDDlgItem);
		CheckDlgButton(hWndAW, nIDDlgItem, bCheck);
	}
}

void gui::EditBox(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, int nWidth, int nLine, bool IsOnlyNumber){
	int nHeight = GetHeight(lpWindowName) + 4;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER);

	if(nWidth == AUTORESIZE){
		nWidth = GetWidth(lpWindowName) + 6;
	}

	if(nLine > 1){
		dwStyle |= (ES_MULTILINE | WS_VSCROLL);
		nHeight += (nLine-1) * 12;
	}

	if(IsOnlyNumber){
		dwStyle |= ES_NUMBER;
	}

	if(CreateWindowExA(NULL, "EDIT", lpWindowName, dwStyle, X, (Y+2), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		SetFont(nIDDlgItem);
	}
}

void gui::Button(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, int nWidth){
	int nHeight = GetHeight(lpWindowName) + 6;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON);

	if(nWidth == AUTORESIZE){
		nWidth = GetWidth(lpWindowName) + 12;
	}

	if(CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+1), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		SetFont(nIDDlgItem);
	}
}

bool gui::CheckBoxStatus(int nIDDlgItem){
	return IsDlgButtonChecked(hWndAW, nIDDlgItem);
}

void gui::ReadOnly(int nIDDlgItem, bool bFlag){
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETREADONLY, bFlag, NULL);
}

LPSTR gui::GetString(int nIDDlgItem){
	int StringSize = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);
	LPSTR lpStrting = (LPSTR)malloc(StringSize+1);

	if(StringSize < 1){
		return NULL;
	}

	GetDlgItemTextA(hWndAW, nIDDlgItem, lpStrting, StringSize+1);

	return lpStrting;
}

//List Control
list::list(){
	HeaderCount = 0;
	CurrentLine = -1;
	SelectedLine = -1;
	SelectedItem = -1;
}
void list::Set(HWND hWndParent, int X, int Y, int nWidth, int nHeight){
	DWORD dwExStyle;
	DWORD dwStyle = (LVS_REPORT | WS_CHILD | LVS_NOSORTHEADER | WS_BORDER | LVS_SINGLESEL| LVS_SHOWSELALWAYS );

	hWndAL = CreateWindowExA(NULL, "SysListView32", NULL, dwStyle, X, Y, nWidth, nHeight, hWndParent, NULL, NULL, NULL);
	SendMessageA(hWndAL, WM_SETFONT, (WPARAM)FontAW, NULL);

	//1行選択
	dwExStyle = ListView_GetExtendedListViewStyle(hWndAL) | LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(hWndAL, dwExStyle);

	//グリッド線
	dwExStyle = ListView_GetExtendedListViewStyle(hWndAL) | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(hWndAL, dwExStyle);
	ShowWindow(hWndAL, SW_SHOW);
}

void list::AddHeader(LPSTR pszText, int cx){
	LVCOLUMNA col;

	ZeroMemory(&col, sizeof(col));

	col.mask = (LVCF_FMT|LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH);
	col.fmt = (LVCFMT_LEFT | LVCFMT_FIXED_WIDTH);
	col.pszText = pszText;
	col.cx = cx;
	col.iSubItem = HeaderCount + 1;

	ListView_InsertColumn(hWndAL, HeaderCount+1, &col);

	HeaderCount++;
}

void list::AddItem(int nItem, LPSTR pszText){
	LVITEMA item;

	if(nItem ==  0){
		CurrentLine++;
	}

	item.mask = LVIF_TEXT;
	item.iItem = CurrentLine;
	item.iSubItem = nItem;
	item.pszText = pszText;

	if(!nItem){
		ListView_InsertItem(hWndAL, &item);
	}else{
		ListView_SetItem(hWndAL, &item);
	}

	ListView_EnsureVisible(hWndAL, CurrentLine, true);
}

void list::Clear(){
	ListView_DeleteAllItems(hWndAL);
	CurrentLine = -1;
	SelectedLine = -1;
	SelectedItem = -1;
}

LPSTR list::GetSelectedItem(int Index){
	int SelectLine = ListView_GetNextItem(hWndAL, -1, LVNI_SELECTED);
	LPSTR lpString = (LPSTR)malloc(MaxStringAL + 1);

	if((SelectedLine == SelectLine) && (SelectedItem == Index)){
		return NULL;
	}

	if(SelectLine == -1){
		return NULL;
	}

	SelectedLine = SelectLine;
	SelectedItem = Index;
	ListView_GetItemText(hWndAL, SelectedLine, SelectedItem, lpString, MaxStringAL);
	return lpString;
}

HWND list::GetHWND(){
	return hWndAL;
}