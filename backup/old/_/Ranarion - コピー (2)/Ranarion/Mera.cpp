#include"Mera.h"

//ゴシックフォントは横幅が固定なので扱いやすいと思います！
#define DefaultEditControlStyle WS_CHILD|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL|WS_BORDER
#define DefaultStaticControlStyle WS_CHILD|WS_VISIBLE|ES_LEFT
#define DefaultPushButtonControlStyle WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON
#define DefaultCheckBoxControlStyle WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX
#define DefaultMultiLineEditControlStyle WS_CHILD|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL|WS_BORDER|ES_MULTILINE|WS_VSCROLL
#define DefaultNumberEditControlStyle WS_CHILD|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL|WS_BORDER|ES_NUMBER

#define DefaultEditControlHeight 16
#define DefaultStaticControlWidth(vn) vn*6
#define DefaultStaticControlHeight 13
#define DefaultPushButtonWidth(vn) vn*6+10
#define DefaultPushButtonHeight 20
#define DefaultCheckBoxWidth(vn) vn*6+15
#define DefaultCheckBoxHeight 13
#define DefaultMultiLineHeight(vi) (vi-1)*12+16

#define DefaultFontName "ＭＳ ゴシック"
#define DefaultFontSize 12

HCURSOR DefaultCursor = LoadCursorA(NULL, IDC_ARROW);
HFONT DefaultFont = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, DefaultFontName);


void CursorAnimationFixer(HWND hwnd, UINT msg){
	//フォーカスがメインウィンドウならリターン
	if(GetFocus()==hwnd) return;
	//マウスアニメーション関連の修正処理(リソースエディタと同等)
	switch(msg){
		case WM_MOUSEMOVE:
			SetCursor(DefaultCursor);//ノーマルのカーソルに変更
			break;
		case WM_LBUTTONUP://キーフォーカスをメインウィンドウへ
			SetFocus(hwnd);
			break;
		default:
			break;
	}
}

void SetFont(HWND hwnd){
	SendMessageA(hwnd, WM_SETFONT, (WPARAM)DefaultFont, MAKELPARAM(NULL, NULL));//フォント変更
}

int GetCharSize(char *Value){
	int i;

	for(i=0; Value[i]!=NULL; i++){}

	return i;
}

//HINSTANCE使うかわからないので使う場合は修正してください
void CreateEditControl(HWND hwnd, int Index, char *Value, int X, int Y, int Width){
	SetFont(CreateWindowExA(NULL, "EDIT", Value, DefaultEditControlStyle, X, Y, Width, DefaultEditControlHeight, hwnd, (HMENU)Index, NULL, NULL));//フォント変更しつつコントロールを作成
}

void CreateStaticControl(HWND hwnd, int Index, char *Value, int X, int Y){
	SetFont(CreateWindowExA(NULL, "STATIC", Value, DefaultStaticControlStyle, X, Y, DefaultStaticControlWidth(GetCharSize(Value)), DefaultStaticControlHeight, hwnd, (HMENU)Index, NULL, NULL));//フォント変更しつつコントロールを作成
}

void CreatePushButtonControl(HWND hwnd, int Index, char *Value, int X, int Y){
	SetFont(CreateWindowExA(NULL, "BUTTON", Value, DefaultPushButtonControlStyle, X, Y, DefaultPushButtonWidth(GetCharSize(Value)), DefaultPushButtonHeight, hwnd, (HMENU)Index, NULL, NULL));//フォント変更しつつコントロールを作成
}

void CreateCheckBoxControl(HWND hwnd, int Index, char *Value, int X, int Y){
	SetFont(CreateWindowExA(NULL, "BUTTON", Value, DefaultCheckBoxControlStyle, X, Y, DefaultCheckBoxWidth(GetCharSize(Value)), DefaultCheckBoxHeight, hwnd, (HMENU)Index, NULL, NULL));//フォント変更しつつコントロールを作成
}

void CreateMultiLineEditControl(HWND hwnd, int Index, char *Value, int X, int Y, int Width, int VisibleLine){
	SetFont(CreateWindowExA(NULL, "EDIT", Value, DefaultMultiLineEditControlStyle, X, Y, Width, DefaultMultiLineHeight(VisibleLine), hwnd, (HMENU)Index, NULL, NULL));//フォント変更しつつコントロールを作成
}

void CreateNumberEditControl(HWND hwnd, int Index, char *Value, int X, int Y, int Width){
	SetFont(CreateWindowExA(NULL, "EDIT", Value, DefaultNumberEditControlStyle, X, Y, Width, DefaultEditControlHeight, hwnd, (HMENU)Index, NULL, NULL));//フォント変更しつつコントロールを作成
}

void ScrollMultiLineEdit(HWND hwnd, int nIDDlgItem, WPARAM wParam){
	SendMessageA(GetDlgItem(hwnd, nIDDlgItem), EM_SCROLL, SB_BOTTOM, NULL);
}

void AddNewLine(HWND hwnd, int nIDDlgItem, char *NewValue){
	int Elength;

	Elength = SendMessageA(GetDlgItem(hwnd, nIDDlgItem), WM_GETTEXTLENGTH, NULL, NULL);
	SendMessageA(GetDlgItem(hwnd, nIDDlgItem), EM_SETSEL, Elength, Elength);
	SendMessageA(GetDlgItem(hwnd, nIDDlgItem), EM_REPLACESEL, NULL, (LPARAM)"\r\n");
	Elength = SendMessageA(GetDlgItem(hwnd, nIDDlgItem), WM_GETTEXTLENGTH, NULL, NULL);
	SendMessageA(GetDlgItem(hwnd, nIDDlgItem), EM_SETSEL, Elength, Elength);
	SendMessageA(GetDlgItem(hwnd, nIDDlgItem), EM_REPLACESEL, NULL, (LPARAM)NewValue);
}