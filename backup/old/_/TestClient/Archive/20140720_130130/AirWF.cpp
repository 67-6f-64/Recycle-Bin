#include"AirWindow.h"

//��s�ǋL
void AirWindow::AddString(int nIDDlgItem, LPCSTR lpNewString, bool bScroll){
	int InjectPoint = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	//������ǉ�
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)lpNewString);

	//���s�ǉ�
	if(bScroll){
		InjectPoint = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);
		SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
		SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)"\r\n");
	}

	//�ŉ��w�܂ŃX�N���[��
	if(bScroll){
		SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SCROLL, SB_BOTTOM, NULL);
	}

}

//�����񏑂�����
void AirWindow::SetString(int nIDDlgItem, LPCSTR lpString){
	SetDlgItemTextA(hWndAW, nIDDlgItem, lpString);
}

//�ǂݎ���p��
void AirWindow::ReadOnly(int nIDDlgItem, bool bFlag){
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETREADONLY, bFlag, NULL);
}

//������擾
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
	char temp[MaxString] = {0};
	Air::_CreateFormatString(temp, _Format, dwArgument);
	AddString(nIDDlgItem, temp, true);
}


void _stdcall AirWindow::AddFormatString(int nIDDlgItem, char *_Format, ...){
	_AddFormatString(nIDDlgItem, _Format, (DWORD *)(&_Format+1));
}