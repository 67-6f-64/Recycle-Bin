#include"AirWindow.h"

char HexArray[] = "0123456789ABCDEF";

void AirWindow::DWORDtoChar(char *temp, DWORD dw){
	int i;

	temp[0] = '0';
	temp[1] = 'x';

	for(i=3; i>=0; i--){
		temp[i*2+2] = HexArray[(BYTE)(dw >> (8*(3-i)))/0x10];
		temp[i*2+3] = HexArray[(BYTE)(dw >> (8*(3-i)))%0x10];
	}
	temp[10] = 0;
}

void AirWindow::WORDtoChar(char *temp, WORD w){
	int i;
	temp[0] = '0';
	temp[1] = 'x';
	for(i=1; i>=0; i--){
		temp[i*2+2] = HexArray[(BYTE)(w >> (8*(1-i)))/0x10];
		temp[i*2+3] = HexArray[(BYTE)(w >> (8*(1-i)))%0x10];
	}
	temp[6] = 0;
}

void AirWindow::BYTEtoChar(char *temp, BYTE b){
	temp[0] = '0';
	temp[1] = 'x';
	temp[2] = HexArray[(BYTE)b/0x10];
	temp[3] = HexArray[(BYTE)b%0x10];
	temp[4] = 0;
}


void AirWindow::BYTEArraytoString(char *temp, DWORD dwSize, BYTE Array[]){
	int i;
	for(i=0; i<dwSize; i++){
		temp[i*3] = HexArray[Array[i]/0x10];
		temp[i*3+1] = HexArray[Array[i]%0x10];
		temp[i*3+2] = ' ';
	}
	temp[i*3-1] = 0;
}


void AirWindow::INTtoChar(char *temp, DWORD d){
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
	temp[i+IsNegative] = 0;
}

int AirWindow::GetStringLength(char *_String){
	int i;
	for(i=0; _String[i]; i++){};
	return i;
}

void _stdcall AirWindow::_CreateFormatString(char *_Output, char *_Format, DWORD *dwArgument){
	int i, j, k, last = 0, iSize;
	char temp[0x100] = {0};
	for(i=j=k=0; _Format[i]; i++){
		if(_Format[i]=='%'){
			memcpy(&_Output[last], temp, j);
			last += j;
			_Output[last] = 0;
			ZeroMemory(temp, 0x100);
			j=0;

			switch(_Format[i+1]){
				case 's'://String
					iSize = GetStringLength((char *)dwArgument[k]);
					memcpy(&_Output[last], (char *)dwArgument[k], iSize);
					last += iSize;
					break;

				case 'd'://DWORD
					DWORDtoChar(temp, dwArgument[k]);
					iSize = GetStringLength(temp);
					memcpy(&_Output[last], temp, iSize);
					last += iSize;
					_Output[last] = 0;
					ZeroMemory(temp, 0x100);
					break;

				case 'w'://WORD
					WORDtoChar(temp, dwArgument[k]);
					iSize = GetStringLength(temp);
					memcpy(&_Output[last], temp, iSize);
					last += iSize;
					_Output[last] = 0;
					ZeroMemory(temp, 0x100);
					break;

				case 'b'://WORD
					BYTEtoChar(temp, dwArgument[k]);
					iSize = GetStringLength(temp);
					memcpy(&_Output[last], temp, iSize);
					last += iSize;
					_Output[last] = 0;
					ZeroMemory(temp, 0x100);
					break;

				case 'i'://int
					INTtoChar(temp, dwArgument[k]);
					iSize = GetStringLength(temp);
					memcpy(&_Output[last], temp, iSize);
					last += iSize;
					_Output[last] = 0;
					ZeroMemory(temp, 0x100);
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
	iSize = GetStringLength(temp);
	memcpy(&_Output[last], temp, iSize);
	last += iSize;
	_Output[last] = 0;
	ZeroMemory(temp, 0x100);
}

void _stdcall AirWindow::CreateFormatString(char *_Output, char *_Format, ...){
	_CreateFormatString(_Output, _Format, (DWORD *)(&_Format+1));
}

//sub
//一行追記
void AirWindow::AddString(int nIDDlgItem, LPCSTR lpNewString, bool bScroll){
	int InjectPoint = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	//文字列追加
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)lpNewString);

	//改行追加
	if(bScroll){
		InjectPoint = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);
		SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
		SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)"\r\n");
	}

	//最下層までスクロール
	if(bScroll){
		SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SCROLL, SB_BOTTOM, NULL);
	}

}

//文字列書き込み
void AirWindow::SetString(int nIDDlgItem, LPCSTR lpString){
	SetDlgItemTextA(hWndAW, nIDDlgItem, lpString);
}

//読み取り専用化
void AirWindow::ReadOnly(int nIDDlgItem, bool bFlag){
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETREADONLY, bFlag, NULL);
}

//文字列取得
LPSTR AirWindow::GetString(int nIDDlgItem){
	int StringSize = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	if(StringSize < 1){
		return NULL;
	}

	LPSTR lpStrting = (LPSTR)malloc(StringSize+1);

	GetDlgItemTextA(hWndAW, nIDDlgItem, lpStrting, StringSize+1);

	return lpStrting;
}


void _stdcall AirWindow::_AddFormatString(int nIDDlgItem, char *_Format, DWORD *dwArgument){
	char temp[0x100] = {0};
	_CreateFormatString(temp, _Format, dwArgument);
	AddString(nIDDlgItem, temp, true);
}


void _stdcall AirWindow::AddFormatString(int nIDDlgItem, char *_Format, ...){
	_AddFormatString(nIDDlgItem, _Format, (DWORD *)(&_Format+1));
}