#include"AirASM.h"

BOOL FullAccess(DWORD dwAddress, DWORD dwSize){
	DWORD old;
	return VirtualProtect((DWORD *)dwAddress, dwSize, PAGE_EXECUTE_READWRITE, &old);
}


bool Hook(char *Type, DWORD dwPrev, DWORD dwNext){

	if(!FullAccess(dwPrev, 5)){
		return false;
	}

	switch(*(DWORD *)Type){
		case 0x00706D6A:
			*(BYTE *)(dwPrev) = 0xE9;
			break;
		default:
			return false;
	}

	*(DWORD *)(dwPrev + 1) = dwNext-dwPrev-5;

	return true;
}


LPCSTR lpHexArray = "0123456789ABCDEF";

int StringCleaner(LPSTR lpTemp, LPCSTR lpString){
	int i, RealSize;

	for(i=RealSize=0; lpString[i]; i++){
		if((lpString[i] >= '0' && lpString[i] <='9') || (lpString[i] >= 'A' && lpString[i] <='F')){
			lpTemp[RealSize] = lpString[i];
			RealSize++;
		}
		else if(lpString[i] >= 'a' && lpString[i] <='f'){
			lpTemp[RealSize] = (BYTE)lpString[i]-0x20;
			RealSize++;
		}
		else if(lpString[i] == '*'){
			lpTemp[RealSize] = lpHexArray[rand()%0x10];
			RealSize++;
		}
	}

	return RealSize;
}

int ChartoByte(LPBYTE lpByte, LPCSTR lpString){
	int i, RealSize;

	for(i=RealSize=0; lpString[i]; i+=2){
		if(lpString[i] <= '9'){
			lpByte[RealSize] = ((BYTE)lpString[i]-0x30)*0x10;
		}
		else{
			lpByte[RealSize] = ((BYTE)lpString[i]-0x37)*0x10;
		}
		if(lpString[i+1] <= '9'){
			lpByte[RealSize] += (BYTE)lpString[i+1]-0x30;
		}
		else{
			lpByte[RealSize] += (BYTE)lpString[i+1]-0x37;
		}
		RealSize++;
	}

	return RealSize;
}

bool HexWriter(DWORD dwAddress, LPCSTR lpString){
	LPSTR lpTemp;
	LPBYTE lpArray;
	int i, RealSize;

	for(i=0; lpString[i]; i++){}

	lpTemp = (LPSTR)malloc(i);
	ZeroMemory(lpTemp, i);

	RealSize = StringCleaner(lpTemp, lpString);

	if(RealSize%2){
		free(lpTemp);
		return false;
	}

	lpArray = (LPBYTE)malloc(RealSize/2);
	ZeroMemory(lpArray, RealSize/2);

	RealSize = ChartoByte(lpArray, lpTemp);

	for(i=0; i<RealSize; i++){
		*(BYTE *)(dwAddress+i) = lpArray[i];
	}


	free(lpTemp);
	free(lpArray);

	return true;
}