#include"blabla.h"

BOOL FullAccess(DWORD dwMemory, DWORD dwSize){
	DWORD old;

	return VirtualProtect((DWORD *)dwMemory, dwSize, PAGE_EXECUTE_READWRITE, &old);
}


DWORD Alloc(DWORD dwSize){
	DWORD dwMemory = (DWORD)new BYTE[dwSize];
	int i;

	if(!FullAccess(dwMemory, dwSize)){
		return 0;//coudn't change memory protect
	}

	for(i=0; i<dwSize; i++){
		*(BYTE *)(dwMemory+i) = 0x90;
	}

	return dwMemory;
}


void Write1(LPDWORD dwMemory, BYTE bCode, bool bNext){
	*(BYTE *)(*dwMemory)  = bCode;
	if(bNext){
		*dwMemory+=1;
	}
}

void Write2(LPDWORD dwMemory, WORD wCode, bool bNext){
	*(WORD *)(*dwMemory)  = wCode;
	if(bNext){
		*dwMemory += 2;
	}
}

void Write4(LPDWORD dwMemory, DWORD dwCode, bool bNext){
	*(DWORD *)(*dwMemory)  = dwCode;
	if(bNext){
		*dwMemory += 4;
	}
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

int WriteCode(LPDWORD dwMemory, LPCSTR lpString, bool bNext){
	LPSTR lpTemp;
	LPBYTE lpArray;
	int i, RealSize;

	for(i=0; lpString[i]; i++){}

	lpTemp = (LPSTR)malloc(i);
	ZeroMemory(lpTemp, i);

	RealSize = StringCleaner(lpTemp, lpString);

	if(RealSize%2){
		free(lpTemp);
		return 0;
	}

	lpArray = (LPBYTE)malloc(RealSize/2);
	ZeroMemory(lpArray, RealSize/2);

	RealSize = ChartoByte(lpArray, lpTemp);

	for(i=0; i<RealSize; i++){
		*(BYTE *)(*dwMemory+i) = lpArray[i];
	}

	if(bNext){
		*dwMemory += i;
	}

	free(lpTemp);
	free(lpArray);
	return 0;
}