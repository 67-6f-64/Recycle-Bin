#include"AirPL.h"
#include<stdio.h>
DWORD dwSendHook_Ret;

void _declspec(naked) SendHook(){
	_asm{
		pushad
		mov ebx,[ebp+0x04]
		cmp byte ptr [ebx],0xC3//ret
		jne Label1
		mov ebx,[ebp+0x0C]//return2
Label1:
		mov eax,[ebp+0x08]//SPacket
		push ebx//Return
		push [eax+0x08]//Size
		push [eax+0x04]//Packet
		call SendLog
		test eax,eax
		popad
		jne SendBlock
		jmp dword ptr [dwSendHook_Ret]
SendBlock:
		leave
		ret 0x0004
	}
}

DWORD dwRecvPointer_Org;
DWORD dwRecvReturn;

void _declspec(naked) RecvHook(){
	_asm{
		mov eax,[dwRecvReturn]
		cmp dword ptr [esp],eax
		jne RH_Ending
		mov eax,esp
		pushad
		mov ebx,[eax+0x34]//RPacket
		push [eax+0x30]//Return
		push [ebx+0x0C]//Size
		push [ebx+0x08]//Packet
		call RecvLog
		popad
RH_Ending:
		jmp dword ptr [dwRecvPointer_Org]
	}
}

//fag
char HexArray[] ="0123456789ABCDEF0123456789abcdef";

int StringtoByteArray(BYTE *bCode, char *Code){
	int iSize = 0, i, j;

	for(i=0; Code[i]; i++){
		if(Code[i] == '*'){
			Code[i] = HexArray[rand()%0x10];
		}
		for(j=0; HexArray[j]; j++){
			if(Code[i] == HexArray[j]){
				if(iSize%2 == 0){
					bCode[iSize/2] = (j%0x10)*0x10;
				}
				else{
					bCode[iSize/2] += j%0x10;
				}
				iSize++;
				break;
			}
		}
	}

	if(iSize%2){
		return 0;
	}

	return (iSize/2);
}
//fag

DWORD dwFlagSend = 0;
DWORD PacketSize = 0;
BYTE Packet[2048];

void _stdcall SendPacket(char *PacketString){
	WORD Header;
	PacketSize = 0;
	Header = StringToWORD(PacketString);
	if(Header == 0xFFFF){
		return;
	}
	PacketSize = 2;
	*(WORD *)&Packet[0] = Header;

	PacketSize += StringtoByteArray((BYTE *)&Packet[2], &PacketString[5]);

	dwFlagSend = 1;
}

void _stdcall NullPacket(WORD Header){
	ZeroMemory(&Packet[0], 102);
	*(WORD *)&Packet[0] = Header;
	PacketSize = 102;
	dwFlagSend = 1;
}

DWORD dwDMPointer;
DWORD DMRetAddr;
DWORD CreatePacket;//0x0097E5D0;
DWORD FreePacket;//0x0094CDB0;

void _declspec(naked) PacketInjector(){
	_asm{
		mov eax,[esp]
		cmp dword ptr [DMRetAddr],eax
		jne PI_End
		cmp dword ptr [dwFlagSend],1
		jne PI_End
		mov dword ptr [dwFlagSend],0
		cmp dword ptr [PacketSize],2
		jb PI_End
		pushad
		/*
		push 0x00
		push [PacketSize]
		lea eax,[Packet]
		push eax
		push 0x00
		*/
		sub esp,0x10
		mov ecx,esp
		cmp dword ptr [CreatePacket],0
		je Label2
		push 0x00
		call dword ptr [CreatePacket]
Label2:
		mov ecx,esp
		lea eax,[Packet]
		mov dword ptr [ecx+0x04],eax
		mov eax,[PacketSize]
		mov dword ptr [ecx+0x08],eax
		push PI_RET
		push ecx
		push [FakeRet]
		mov ecx,[SendObject]
		mov ecx,[ecx]
		jmp dword ptr [SendFunction]
PI_RET:
		//mov ecx,esp
		//call dword ptr [FreePacket]
		add esp,0x10
		popad
PI_End:
		jmp dword ptr [dwDMPointer]
	}
}

AirMemory AM;

DWORD FaggotArray[2000] = {0};
DWORD SendFunction, SendObject, FakeRet;
DWORD MS_Start, MS_End, MS;
DWORD FaggotJMP = 0;

void FindJMP(){
	DWORD GetData = AM.AobScan("51 53 56 57 8B F9 8B 57 08 8B 02 89 44 24 0C 8A 42 04");
	DWORD i, temp;

	/*
	for(i=MS_Start; i<MS_End; i++){
		if(*(BYTE *)(i) == 0xE9){
			temp = i + *(DWORD *)(i+1) + 5;
			if(temp == GetData){
				FaggotJMP = i;
				break;
			}
		}
	}
	
	
	for(i=SendObject; i<MS_End; i++){
		if(*(BYTE *)(i) == 0xE8){
			temp = i + *(DWORD *)(i+1) + 5;
			if(temp == FaggotJMP){
				AM.MemoryWriter(i, "B8 01 00 00 00");
			}
		}
	}
	
	*/

	for(i=0x00500000; ;i++){
		if(*(BYTE *)(i) == 0xC3){
			FakeRet = i;
			break;
		}
	}
//	*(DWORD *)((DWORD)&FaggotArray+0x24) = 1;
//	*(DWORD *)((DWORD)&FaggotArray+0x6B8) = 1;
}

void WriteHooks(){
	DWORD DetectionList[128] = {0};//
	DWORD i, dwHookAddr, RecvPointer;
	DWORD temp;

	AM.Init();
	AM.GetDumpInfo(&MS_Start, &MS_End, &MS);
	SendFunction = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 ?? ?? ?? 64 A3 00 00 00 00 ?? ?? ?? 6A 00 E9");
	dwRecvReturn = AM.AobScan("8B 7C 24 2C 8B CF C7 44 24 24 00 00 00 00 E8");

	if(dwRecvReturn){
		RecvPointer = *(DWORD *)(dwRecvReturn - 4);
		AM.PointerHook(RecvPointer, RecvHook, &dwRecvPointer_Org);
	}

	dwHookAddr = AM.AutoVMHook(SendFunction, SendHook, &dwSendHook_Ret, RecvPointer);

	if(!SendFunction){
		MessageBoxA(NULL, "ERROR:SendHook", "AirPL", NULL);
	}

	if(!dwRecvReturn){
		MessageBoxA(NULL, "ERROR:RecvHook", "AirPL", NULL);
	}

	//temp = AM.AobScan("8B 0D ?? ?? ?? ?? 8D 44 ?? ??  ?? E8 ?? ?? ?? ?? 8B ?? ?? ?? 6A 00 E8");
	temp = AM.AobScan("8B 0D ?? ?? ?? ?? 8D 54 24 08 52 E8 ?? ?? ?? ?? 8D 4C 24 0C");
	if(temp){
		SendObject = *(DWORD *)(temp +2);
	}
	else{
		MessageBoxA(NULL, "ERROR:SendObject", "AirPL", NULL);
	}

	temp = AM.AobScan("FF 15 ?? ?? ?? ?? 8D 55 ?? 52 8B 8D ?? ?? ?? ?? E8");//JMS

	if(!temp){
		temp = AM.AobScan("FF 15 ?? ?? ?? ?? 8D 95 ?? ?? ?? ?? 52 8B 8D ?? ?? ?? ?? E8");//EMS
	}

	if(temp){
		AM.PointerHook(*(DWORD *)(temp+2), PacketInjector, &dwDMPointer);
		DMRetAddr = temp+6;
	}
	else{
		MessageBoxA(NULL, "ERROR:SendInject", "AirPL", NULL);
	}

	CreatePacket = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 ?? 8D ?? ?? ?? 64 A3 00 00 00 00 8B F1 89 ?? ?? ?? 8D ?? ?? ?? ?? 8D ?? ?? 68 00 01 00 00 C7 01 00 00 00 00 E8");
	
	if(!CreatePacket){
		MessageBoxA(NULL, "ERROR:SendInject(EMS)", "AirPL", NULL);
	}

	//bypass send detection for EMS
	/*
	for(i=0; ; i++){
		DetectionList[i] = AM.AobScan("64 A1 18 00 00 00", i+1);
		if(DetectionList[i] == 0){
			break;
		}
	}

	for(i=0; DetectionList[i]; i++){
		if(DetectionList[i] > RecvPointer){
			//AM.MemoryWriter(DetectionList[i], "B8 00 D0 FD FF 90");
			*(BYTE *)(DetectionList[i]) = 0xB8;
			*(DWORD *)(DetectionList[i] + 1) = (DWORD)&FaggotArray;
			*(BYTE *)(DetectionList[i] + 5) = 0x90;
		}
	}
	*/
	
	FindJMP();
}