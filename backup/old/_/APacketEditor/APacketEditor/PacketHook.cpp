#include"PacketHook.h"
#include"PacketGUI.h"
#include"AirASM.h"
#include"Address.h"
/*
Recv:
0057F2A5 - call dword ptr [0128E468] : [WS2_32.recv]
0057F2AB - test eax,eax
...
0057F390 - call 0057F100
...
0057F1CC - call 0057ED20
*/

void _declspec(naked) SendHook(){
	_asm{
		pushad
		mov esi,[ebp+0x08]
		mov eax,[ebp+0x04]
		cmp eax,[FaggotReturn]
		jne Hook1
		mov eax,[ebp+0x0C]
Hook1:
		push [esi+0x04]
		push [esi+0x08]
		push eax
		push 1
		call AddPacket
Ending1:
		popad
		jmp dword ptr [SendHookReturn]
	}
}


void _declspec(naked) RecvPointerHook(){
	_asm{
		mov eax,[esp]
		cmp eax,[RecvHookEsp]
		jne Ending2
		mov eax,RecvHook
		mov dword ptr [esp],eax
Ending2:
		jmp dword ptr [RecvHookPointer_Org]
RecvHook:
		mov ecx,[esp+0x28]//return Address
		mov edi,[esp+0x2C]//RPacket Struct ( original code)
		pushad
		push [edi+0x08]
		sub dword ptr [esp],0x04
		push [edi+0x0C]
		sub dword ptr [esp],0x04
		cmp dword ptr [esp],2
		jb Ending2_1
		push ecx
		push 0
		call AddPacket
Ending2_1:
		popad
		jmp dword ptr [RecvHookEsp]
	}
}

void WriteHook(){
	Hook("jmp", SendHookAddr, (DWORD)SendHook);
	*(DWORD *)(RecvHookPointer) = (DWORD)RecvPointerHook;
	//hello
	HexWriter(0x017EB0D5, "31 C0 31 C9 90");
	HexWriter(0x01845BCB, "31 C0 31 C9 90");
	HexWriter(0x019DD101, "31 C0 31 C9 90");
	HexWriter(0x01AA3CF1, "31 C0 31 C9 90");
	HexWriter(0x01AD4623, "31 C0 31 C9 90");
	//world
	HexWriter(0x017303CE, "31 C0 90 90 90 90");//6B8
	HexWriter(0x01845BF8, "31 C0 90 90 90 90");//6B8
	HexWriter(0x01A26170, "31 C0 90");//24
}

