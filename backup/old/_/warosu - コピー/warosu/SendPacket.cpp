#include"SendPacket.h"

namespace PS{
	DWORD SendFunction = 0x00487140;//v327.0
}

BOOL PS::SendPacket(char Packet_STR[]){
	int Packet_Size;
	BYTE Packet[4096];

	Packet_Size = Asm::ChartoByte(Packet_STR, Packet);

	if(Packet_Size == -1 || Packet_Size < 2){
		return FALSE;
	}

	//created by airride^^
	_asm{
		lea eax,[Packet]
		mov ebx,[Packet_Size]
		push 0x00
		push ebx
		push eax
		push 0x00
		push esp
		call SendFunction
		add esp,0x04
		add esp,0x10
	}

	return TRUE;
}





bool SendPacketString(LPSTR lpString){
	LPCSTR lpRandom = "0123456789ABCDEF";
	LPSTR lpTemp;
	LPBYTE lpArray;
	int i,j;

	//サイズ取得
	for(i=0; lpString[i]; i++){}
	//メモリ確保
	lpTemp = (LPSTR)malloc(i);
	ZeroMemory(lpTemp, i);

	for(i=j=0; lpString[i]; i++){
		if((lpString[i] >= '0' && lpString[i] <='9') || (lpString[i] >= 'A' && lpString[i] <='F')){
			lpTemp[j] = lpString[i];
			j++;
		}
		else if(lpString[i] >= 'a' && lpString[i] <='f'){
			lpTemp[j] = (BYTE)lpString[i]-0x20;
			j++;
		}
		else if(lpString[i] == '*'){
			lpTemp[j] = lpRandom[rand()%0x10];
			j++;
		}
	}

	if(j%2){
		free(lpTemp);
		return false;//error
	}

	lpArray = (LPBYTE)malloc(j/2);
	ZeroMemory(lpArray, j/2);

	for(i=j=0; lpTemp[i]; i+=2){
		if(lpTemp[i] <= '9'){
			lpArray[j] = ((BYTE)lpTemp[i]-0x30)*0x10;
		}
		else{
			lpArray[j] = ((BYTE)lpTemp[i]-0x37)*0x10;
		}
		if(lpTemp[i+1] <= '9'){
			lpArray[j] += (BYTE)lpTemp[i+1]-0x30;
		}
		else{
			lpArray[j] += (BYTE)lpTemp[i+1]-0x37;
		}
		j++;
	}

	_asm{
		pushad
		push 0x00000000
		movzx eax,[j]
		push eax
		mov eax,[lpArray]
		push eax
		push 0x00000000
		push esp
		mov eax,0x00478020
		call eax
		popad
	}

	free(lpTemp);
	free(lpArray);
	return true;
}
