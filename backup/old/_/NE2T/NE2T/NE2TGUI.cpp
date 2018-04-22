#include"NE2T.h"

HWND hNE2TDialog;

void SetIcon(HINSTANCE hIns,HWND hwnd,int IconID){
	HICON hIcon;

	hIcon = (HICON)LoadImage(hIns, MAKEINTRESOURCE(IconID), IMAGE_ICON, 32, 32, 0);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}


void InitEditControl(){
	SetDlgItemText(hNE2TDialog, Edit_SA,"00400000");
	SetDlgItemText(hNE2TDialog, Edit_EA,"01200000");
	SetDlgItemText(hNE2TDialog, Edit_Addr,"00700000");
	SetDlgItemText(hNE2TDialog, Edit_Size,"8");
	SetDlgItemText(hNE2TDialog, Edit_Pointer,"0147C8D8");
	SetDlgItemText(hNE2TDialog, Edit_Offset,"1280");
}


DWORD ReadEditAddr(int EditID){
	char Temp[256];
	DWORD Addr;
	GetDlgItemText(hNE2TDialog, EditID, Temp, 11);
	//wsprintf(Temp, TEXT("%08X"), Addr);
	_stscanf_s(Temp, "%X", &Addr);
	//Addr=(DWORD)Temp;
	//wsprintf(Temp, TEXT("%08X"), Addr);
	//SetDlgItemText(hNE2TDialog, Edit_Memory,Temp);
	return Addr;
}

DWORD ReadEditSize(int EditID){
	char Temp[256];
	DWORD Size;
	GetDlgItemText(hNE2TDialog, EditID, Temp, 11);
	_stscanf_s(Temp, "%d", &Size);
	return Size;
}

void WriteByteArray(int EditID,DWORD Size){
	TCHAR uri[1024];
	DWORD i;
	for(i=0; i<Size; i++){
	wsprintf(&uri[i*3], TEXT("%02X "), Data[i]);
	}
	SetDlgItemText(hNE2TDialog, EditID,uri);
}

void WriteDWORD(int EditID){
	char Temp[64];
	wsprintf(Temp, TEXT("%d"), byte4);
	SetDlgItemText(hNE2TDialog, EditID, Temp);
}

bool F_PR=false;

void PointerReading(){
	F_PR=true;
	SendDlgItemMessage(hNE2TDialog, Edit_Pointer, EM_SETREADONLY, true, 0);
	SendDlgItemMessage(hNE2TDialog, Edit_Offset, EM_SETREADONLY, true, 0);

	DWORD Pointer=ReadEditAddr(Edit_Pointer);
	DWORD Offset=ReadEditAddr(Edit_Offset);

	while(F_PR==true){
	if(PointerReader(Pointer, Offset)) WriteDWORD(Edit_ResultP);
	Sleep(50);
	}
	SendDlgItemMessage(hNE2TDialog, Edit_Pointer, EM_SETREADONLY, false, 0);
	SendDlgItemMessage(hNE2TDialog, Edit_Offset, EM_SETREADONLY, false, 0);
}

INT_PTR CALLBACK NE2TDialog(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam){
		switch(uMsg){
				case WM_INITDIALOG:
					hNE2TDialog = hwnd;

					SetIcon(hNE2T, hNE2TDialog, Icon_NE2T);
					InitEditControl();
					//CheckDlgButton(hNE2TDialog, Check_Attach, BST_CHECKED);
					return true;


				case WM_COMMAND:
						switch(LOWORD(wParam)){
							case Check_Attach:
								if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
									CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ProcessChecker,NULL,NULL,NULL);
								}else{
									F_Attach=false;
								}
								return true;

							case Button_Read:
								if(MemoryReader(ReadEditAddr(Edit_Addr), ReadEditSize(Edit_Size))){
								WriteByteArray(Edit_Memory, ReadEditSize(Edit_Size));
								}
								return true;

							case Check_ReadP:
								if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
									CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)PointerReading,NULL,NULL,NULL);
								}else{
									F_PR=false;
								}
								return true;


						}
					return false;

				case WM_CLOSE:
					EndDialog(hwnd, true);
					return true;
		}
			return false;
}