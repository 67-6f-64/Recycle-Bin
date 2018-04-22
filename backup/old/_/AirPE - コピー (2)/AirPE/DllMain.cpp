#include<Windows.h>
#include"AirWindow.h"
#include"AirF2.h"
#include"Parser.h"
#pragma comment(lib, "AirWindow.lib")
#include"Singeki.h"

#define ButtonInject 0x100
#define ButtonClear 0x101
#define EditInject 0x200
#define EditReturn 0x201
#define CheckSendHook 0x300
#define CheckRecvHook 0x301
#define CheckFormatView 0x302
#define ButtonSingeki 0x400
#define ButtonCCPLZ 0x401

AirWindow AW;
AirList AL;

bool bSendHook = true;
bool bRecvHook = true;
bool bFormatView = false;

#define MaxSize 2048

void WriteHook();

////////////////////////////////////////////////////////

void _stdcall DecodeFinish(){
	RFinish();
	if(bFormatView){
		if((orgRSize > 1) && (orgRSize < MaxSize) && (orgRSize == RPacketSize)){
			AL.AddItem("%s%d%i%s", "FIn", orgRReturn, orgRSize, RBuffer);
		}
		else{
			AL.AddItem("%s%d%i%s", "FIn", orgRReturn, orgRSize, "This packet was not fully decoded or the packet is too long to write here :wut:");
		}
	}
	RCurrentPointer = 0;
	RPacketSize = 0;
}

void _stdcall PacketHook(DWORD dwType, DWORD dwReturn, WORD dwSize, BYTE Packet[]){

	if(dwType == 0){
		if(bSendHook){
			if(dwSize < MaxSize){
				AL.AddItem("%s%d%i%a", "Out", dwReturn, dwSize, Packet, dwSize);

			}
			else{
				AL.AddItem("%s%d%i%s", "Out", dwReturn, dwSize, "This packet is too long to write here");
			}
		}
		if(bFormatView && (dwSize < MaxSize)){
			SFinish();
			if(dwSize == SPacketSize){
				AL.AddItem("%s%d%i%s", "FOut", dwReturn, dwSize, SBuffer);
			}
			else{
				AL.AddItem("%s%d%i%s", "FOut", dwReturn, dwSize, "Some values of this packet were added directly :wut:");
			}
		}
	}
	else{
		if(bRecvHook){
			if((dwSize > 5) && (dwSize < MaxSize)){
				AL.AddItem("%s%d%i%a", "In", dwReturn, dwSize-4, &Packet[4], dwSize-4);
			}
			else{
				AL.AddItem("%s%d%i%s", "In", dwReturn, dwSize-4, "This packet is too long to write here");
			}
		}

		if(bFormatView && (dwSize < MaxSize) && (dwSize > 5)){
			orgRSize = dwSize - 4;
			orgRReturn = dwReturn;
			RCurrentPointer = 0;
			RPacketSize = 0;
			RInit();
		}
	}

}
//55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 ?? ?? ?? 64 A3 00 00 00 00 ?? ?? ?? 6A 00 E9
DWORD dwSendHookAddr = 0x01F08161;
DWORD dwSendHookRet = 0x0092FCB7;
DWORD SendFunction = 0x0048F600;
DWORD dwCommonReturn = SendFunction + 0x10;

//8B 7C 24 2C 8B CF C7 44 24 24 00 00 00 00 E8
DWORD dwInterlockedIncrement;
DWORD dwRecvHookPointer = 0x017E2234;
DWORD dwRecvHookRet = 0x005F0861;

//51 53 56 57 8B F9 8B 57 08 8B 02 89 44 24 0C 8A 42 04 84 C0 75 02 2nd
DWORD UpdateKey = 0x01CE256F;
DWORD UpdateKey_Ret = 0x01C3643E;
DWORD GetKey = 0x01D8CD3F;
DWORD GetKey_Ret = 0x01EEE815;

void _declspec(naked) SendHook(){
	_asm{
		pushad
		mov ebx,[ebp+0x04]
		cmp ebx,[dwCommonReturn]
		jne Label1
		mov ebx,[ebp+0x0C]
Label1:
		mov eax,[ebp+0x08]//Packet Struct
		push [eax+0x04]//Packet
		push [eax+0x08]//Size
		push ebx//Return
		push 0x00
		call PacketHook
		popad
		jmp dword ptr [dwSendHookRet]
	}
}

void _declspec(naked) RecvPointerHook(){
	_asm{
		mov eax,[dwRecvHookRet]
		cmp dword ptr [esp],eax
		jne EndingP
		mov eax,esp
		pushad
		mov ebx,[eax+0x34]//Packet Struct
		cmp word ptr [ebx+0x0C],0x06
		jb EndingL
		push [ebx+0x08]//Packet
		push [ebx+0x0C]//Size
		push [eax+0x30]//Return
		push 0x01
		call PacketHook
EndingL:
		popad
EndingP:
		jmp dword ptr [dwInterlockedIncrement]
	}
}


void _declspec(naked) _UpdateKey(){
	_asm{
		xor eax,eax
		xor ecx,ecx
		jmp dword ptr [UpdateKey_Ret]
	}
}

void _declspec(naked) _GetKey(){
	_asm{
		xor eax,eax
		xor ecx,ecx
		jmp dword ptr [GetKey_Ret]
	}
}

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

void _stdcall SendPacket(){
	char *StrPacket = AW.GetString(EditInject);
	if(!StrPacket){
		return;
	}
	BYTE Packet[1024] = {0};
	DWORD dwSize = StringtoByteArray(Packet, StrPacket);

	if(dwSize > 1024 || dwSize == 0){
		return;
	}

	_asm{
		push 0x00
		push [dwSize]
		lea eax,[Packet]
		push eax
		push 0x00
		push esp
		call dword ptr [SendFunction]
		add esp,0x04
		add esp,0x10
	}

}


void GuiCreationI(HWND hWnd){
	AL.CreateListControl(hWnd, 3, 3, 794, 594-40);
	AL.AddHeader("Type", 40);
	AL.AddHeader("Return", 100);
	AL.AddHeader("Size", 60);
	AL.AddHeader("Packet", 560);

	AW.CreateButton(ButtonInject, "Send", 630, 579);
	AW.CreateEditBox(EditInject, NULL, 3, 579, 600);
	AW.CreateEditBox(EditReturn, NULL, 3, 559, 100);
	AW.CreateCheckBox(CheckSendHook, "SendHook", 200, 559, bSendHook);
	AW.CreateCheckBox(CheckRecvHook, "RecvHook", 300, 559, bRecvHook);
	AW.CreateButton(ButtonClear, "Clear", 400, 559);
	AW.CreateCheckBox(CheckFormatView, "FormatView", 500, 559, bFormatView);

	
	AW.CreateButton(ButtonSingeki, "Singeki", 600, 559);
	AW.CreateButton(ButtonCCPLZ, "CCPLZ", 700, 559);
	WriteHook();
}


void GuiControlI(WORD wID){
	switch(wID){
		case ButtonInject:
			SendPacket();
			break;

		case ButtonClear:
			AL.ClearAll();
			break;

		case CheckSendHook:
			bSendHook = AW.CheckBoxStatus(wID);
			break;

		case CheckRecvHook:
			bRecvHook = AW.CheckBoxStatus(wID);
			break;

		case CheckFormatView:
			bFormatView = AW.CheckBoxStatus(wID);
			break;
			
		case ButtonSingeki:
			Singeki();
			break;

		case ButtonCCPLZ:
			CCPLZ();
			break;

		default:
			break;
	}
}


void GuiNotifyI(HWND hWnd){
	char Buffer[MaxString] = {0};
	char Buffer2[16] = {0};

	if(hWnd == AL.GetHWND()){
		AL.GetSelected(Buffer, 3);
		AL.GetSelected(Buffer2, 1);

		if(Buffer[0]){
			AW.SetString(EditInject, Buffer);
			AW.SetString(EditReturn, Buffer2);
		}
	}
}

void WriteJMP(DWORD dwPrev, DWORD dwNext){
	DWORD old;

	VirtualProtect((DWORD *)dwPrev, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE *)(dwPrev) = 0xE9;
	*(DWORD *)(dwPrev+1) = (DWORD)dwNext -  dwPrev -5;
}
void WriteCALL(DWORD dwPrev, DWORD dwNext){
	DWORD old;

	VirtualProtect((DWORD *)dwPrev, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE *)(dwPrev) = 0xE8;
	*(DWORD *)(dwPrev+1) = (DWORD)dwNext -  dwPrev -5;
}


void WriteHook(){
	DWORD old;

	VirtualProtect((DWORD *)dwSendHookAddr, 5, PAGE_EXECUTE_READWRITE, &old);

	*(BYTE *)(dwSendHookAddr) = 0xE9;
	*(DWORD *)(dwSendHookAddr+1) = (DWORD)SendHook - dwSendHookAddr - 5;


	VirtualProtect((DWORD *)dwRecvHookPointer, 4, PAGE_EXECUTE_READWRITE, &old);
	dwInterlockedIncrement = *(DWORD *)dwRecvHookPointer;

	*(DWORD *)(dwRecvHookPointer) = (DWORD)RecvPointerHook;
	
	VirtualProtect((DWORD *)UpdateKey, 5, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)GetKey, 5, PAGE_EXECUTE_READWRITE, &old);
	
	*(BYTE *)(UpdateKey) = 0xE9;
	*(DWORD *)(UpdateKey+1) = (DWORD)_UpdateKey - UpdateKey -5;
	*(BYTE *)(GetKey) = 0xE8;
	*(DWORD *)(GetKey+1) = (DWORD)_GetKey - GetKey -5;

	EncodeHook();
	DecodeHook();
}

void AirPE(HINSTANCE hInstance){
	AW.CreateSimpleWindow(hInstance, "AirPE", 800, 600, GuiCreationI, GuiControlI, GuiNotifyI, true);
	Air::WaitForQuit();
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}
	srand((DWORD)GetCurrentProcessId());
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AirPE, NULL, NULL, NULL);

	return TRUE;
}