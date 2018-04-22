#include"NE2T.h"

typedef struct{
	LPBYTE Aob;
	LPBYTE Type;
	DWORD  Size;
}ScanArray;


bool CreateArrayOfBytes(int EditID){
	char Temp[2048];
	ScanArray *ScanArray;
	DWORD i,j;
	GetDlgItemText(hNE2TDialog, EditID, Temp, 11);

	j=0;
	for(i=0;i<2048;i++){
		switch(Temp[i]){
			case 0x20: break;
			case 0x3F: ScanArray->Type[j]=1;i++;i++;j++;break;
			default: _stscanf_s(&Temp[i], "%02X", &ScanArray->Aob[j]);i++;i++;j++;break;
		}

	}

	ScanArray->Size=j;




}