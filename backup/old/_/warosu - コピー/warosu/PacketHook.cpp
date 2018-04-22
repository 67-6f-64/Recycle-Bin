#include"PacketHook.h"

namespace PH{
	void SendHook();
	void RecvHook();
	void RecvPointerHook();
	void ThreadCheck();
}
//55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 ?? ?? ?? 64 A3 00 00 00 00 ?? ?? ?? 6A 00 E9
//8B 7C 24 2C 8B CF C7 44 24 24 00 00 00 00 E8

namespace PH{
	DWORD SendHook_Addr = 0x01F01200;
	DWORD SendHook_ret = 0x01F342C4;
	DWORD RecvHook_ret = 0x005CDDD1;
	DWORD RecvHook_Pointer = 0x01660070;
	DWORD Recv_Pointer_Memory;
	char Type_S[4] = "OUT";
	char Type_R[4] = "IN";
}


//void AddPacketList(char Type[], DWORD retAddr, int Size, BYTE Packet[]);

void _declspec(naked) PH::SendHook(){
	_asm{
		pushad
		cmp dword ptr [PE::SHook],0
		je Ending_S
		mov ecx,[ebp+0x08]//Struct
		push [ecx+0x04]//SPacket
		push [ecx+0x08]//Size
		push [ebp+0x04]//retAddr
		lea ebx,[Type_S]
		push ebx
		call PE::AddPacketList
Ending_S:
		popad
		jmp SendHook_ret
	}
}


void _declspec(naked) PH::RecvHook(){
	_asm{
		mov ecx,[esp+0x28]//return Address
		mov edi,[esp+0x2C]//RPacket Struct ( original code)
		pushad
		cmp dword ptr [PE::RHook],0
		je Ending_R
		mov eax,[edi+0x08]
		add eax,4
		push eax//SPacket
		mov edx,[edi+0x0C]
		sub edx,4
		push edx//Size
		push ecx//retAddr
		lea ebx,[Type_R]
		push ebx
		call PE::AddPacketList
Ending_R:
		popad
		jmp RecvHook_ret
	}
}


void _declspec(naked) PH::RecvPointerHook(){
	_asm{
		mov eax,[RecvHook_ret]
		cmp dword ptr [esp],eax
		jne Ending_RP
		mov eax,RecvHook
		mov dword ptr [esp],eax
Ending_RP:
		jmp Recv_Pointer_Memory
	}
}

void _declspec(naked) PH::ThreadCheck(){
	_asm{
		mov eax,0x7EFDD000
		mov eax,[eax+0x24]
		mov ecx,eax//using ecx
		ret
	}
}


BOOL PH::PacketHook(){
	Asm::Write_Hook("call", SendHook_Addr, (DWORD)SendHook);

	Recv_Pointer_Memory = Asm::Write_Pointer_Hook(RecvHook_Pointer, (DWORD)RecvPointerHook);
	
	//8D 0C F5 00 00 00 00 66 0B D1 0F B7 CA 2nd result of starting function(0x007... jumped by the address(0x004...
	
	//64 A1 18 00 00 00


	//hello
	//Asm::Write_code(0x0052A850, "31 C0 31 C9 C3");
	//Asm::Write_code(0x0046CFC0, "31 C0 31 C9 C3");
	//world
	//Asm::Write_code(0x01A4212C, "31 C0 90 90 90 90");//6B8
	//Asm::Write_code(0x018DFA81, "31 C0 90 90 90 90");//6B8
	//Asm::Write_code(0x017ADFEF, "31 C0 90");//24

	return TRUE;
}
