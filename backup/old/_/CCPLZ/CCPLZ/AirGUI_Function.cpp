#include"AirGUI_Function.h"

//一行追記
void Air::AddString(HWND hDlg, int nIDDlgItem, LPCSTR lpNewString, bool bScroll){
	int InjectPoint = SendDlgItemMessageA(hDlg, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	//文字列追加
	SendDlgItemMessageA(hDlg, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
	SendDlgItemMessageA(hDlg, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)lpNewString);

	//改行追加
	if(bScroll){
		InjectPoint = SendDlgItemMessageA(hDlg, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);
		SendDlgItemMessageA(hDlg, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
		SendDlgItemMessageA(hDlg, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)"\r\n");
	}

	//最下層までスクロール
	if(bScroll){
		SendDlgItemMessageA(hDlg, nIDDlgItem, EM_SCROLL, SB_BOTTOM, NULL);
	}

}

//文字列書き込み
void Air::SetString(HWND hDlg,int nIDDlgItem, LPCSTR lpString){
	SetDlgItemTextA(hDlg, nIDDlgItem, lpString);
}

//読み取り専用化
void Air::ReadOnly(HWND hDlg, int nIDDlgItem, bool bFlag){
	SendDlgItemMessageA(hDlg, nIDDlgItem, EM_SETREADONLY, bFlag, NULL);
}

//文字列取得
LPSTR Air::GetString(HWND hDlg, int nIDDlgItem){
	int StringSize = SendDlgItemMessageA(hDlg, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	if(StringSize < 1){
		return NULL;
	}

	LPSTR lpStrting = (LPSTR)malloc(StringSize+1);

	GetDlgItemTextA(hDlg, nIDDlgItem, lpStrting, StringSize+1);

	return lpStrting;
}

//ワロス
#define MAX_STRING 0x100

namespace Air{
	char HexArray[] = "0123456789ABCDEF";
	void DWORDtoChar(char *temp, DWORD dw);
	void WORDtoChar(char *temp, WORD w);
	void INTtoChar(char *temp, DWORD d);
	void _stdcall _AddFormatString(HWND hWndParent,int nIDDlgItem, char *_Format, DWORD *dwArgument);
}

void Air::DWORDtoChar(char *temp, DWORD dw){
	int i;

	temp[0] = '0';
	temp[1] = 'x';

	for(i=3; i>=0; i--){
		temp[i*2+2] = HexArray[(BYTE)(dw >> (8*(3-i)))/0x10];
		temp[i*2+3] = HexArray[(BYTE)(dw >> (8*(3-i)))%0x10];
	}

}

void Air::WORDtoChar(char *temp, WORD w){
	int i;
	temp[0] = '0';
	temp[1] = 'x';
	for(i=1; i>=0; i--){
		temp[i*2+2] = HexArray[(BYTE)(w >> (8*(1-i)))/0x10];
		temp[i*2+3] = HexArray[(BYTE)(w >> (8*(1-i)))%0x10];
	}

}

void Air::INTtoChar(char *temp, DWORD d){
	int i, j, digit = 0;
	DWORD _d;
	bool IsNegative = (d < 0x80000000) ? false : true;

	if(!IsNegative){
		_d=d;
	}else{
		_d=~d+1;
		temp[0] = '-';
	}

	for(i=1; (_d/i); i*=10){
		digit++;
	}
	j=i/10;

	for(i=0; i<digit; i++){
		temp[i+IsNegative] = HexArray[_d/j];
		_d -=j*(_d/j);
		j/=10;
	}

}

void _stdcall Air::_AddFormatString(HWND hWndParent,int nIDDlgItem, char *_Format, DWORD *dwArgument){
	int i, j, k;
	char temp[MAX_STRING] = {0};

	for(i=j=k=0; _Format[i]; i++){
		if(_Format[i]=='%'){

			AddString(hWndParent, nIDDlgItem, temp, false);
			ZeroMemory(temp, MAX_STRING);
			j=0;

			switch(_Format[i+1]){
				case 's'://String
					AddString(hWndParent, nIDDlgItem, (char *)dwArgument[k], false);
					break;

				case 'd'://DWORD
					DWORDtoChar(temp, dwArgument[k]);
					AddString(hWndParent, nIDDlgItem, temp, false);
					ZeroMemory(temp, MAX_STRING);
					break;

				case 'w'://WORD
					WORDtoChar(temp, dwArgument[k]);
					Air::AddString(hWndParent, nIDDlgItem, temp, false);
					ZeroMemory(temp, MAX_STRING);
					break;

				case 'i'://int
					INTtoChar(temp, dwArgument[k]);
					AddString(hWndParent, nIDDlgItem, temp, false);
					ZeroMemory(temp, MAX_STRING);
					break;

				default:
					temp[j] = _Format[i];
					i--;
					j++;
					k--;
					break;
			}
			i++;
			k++;
		}
		else{
			temp[j] = _Format[i];
			j++;
		}
	}

	AddString(hWndParent, nIDDlgItem, temp, true);
}

void _stdcall Air::AddFormatString(HWND hWndParent,int nIDDlgItem, char *_Format, ...){

	_asm{
		pushad
		lea esi,[_Format]
		mov eax,esi
		add eax,4
		push eax
		push [esi]
		push [nIDDlgItem]
		push [hWndParent]
		call _AddFormatString
		popad
	}
}