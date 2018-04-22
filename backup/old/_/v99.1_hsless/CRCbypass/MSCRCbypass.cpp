#include"MSCRCbypass.h"

namespace CRC{
	unsigned char *Memory;
	DWORD Addr_Start = 0x00401000;
	DWORD Addr_End   = 0x01200000;
	
	//55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9
	DWORD VEH_DEBUGGER_DETECTION = 0x0053F8D0;//v99.1

	//8B 4D 18 8B 09
	DWORD CRC_Main = 0x00FD16B9;//v99.1
	DWORD CRC_Main_ret = CRC_Main + 5;

	DWORD CRC_Main_2 = 0x00FD149F;//v99.1
	DWORD CRC_Main_2_ret = 0x00FD16B9;


	DWORD CRC_2nd = 0x02072632;//v99.1
	DWORD CRC_2nd_ret = CRC_2nd + 5;

	DWORD CRC_3rd = 0x01F82324;//v99.1
	DWORD CRC_3rd_ret = 0x01F73E3F;

	DWORD CRC_2nd_1 = CRC_2nd - 3;//v99.1
	DWORD CRC_2nd_1_Dump = *(DWORD *)CRC_2nd_1;

	DWORD CRC_2nd_2 = CRC_2nd + 1;//v99.1
	DWORD CRC_2nd_2_Dump = *(DWORD *)CRC_2nd_2;

	
	DWORD CRC_E1 = 0x0053F836;//v99.1
	DWORD CRC_E1_ret = 0x0053FAE1;
	//DWORD CRC_E1_call = 0x00530E6D;

	//8B 00 E8
	DWORD CRC_E2 = 0x0053F6F7;
	DWORD CRC_E2_ret = CRC_E2 + 7;
	//DWORD CRC_E2_call = 0x0093C7A8;

	//8B 00 E8
	DWORD CRC_E3 = 0x0052A702;//removed
	DWORD CRC_E3_ret = CRC_E3 + 7;
	DWORD CRC_E3_call = 0x0052AB29;
	
	DWORD CRC_E4 = 0x0053FA5E;//v99.1
	DWORD CRC_E4_ret = CRC_E4 + 10;
	
	DWORD CRC_E5 = 0x004CCE32;//98.2
	DWORD CRC_E5_ret = CRC_E5 + 6;

	
	void CRC_Main_Hook();
	void CRC_Main_Hook2();
	void CRC_2nd_Hook();
	void CRC_3rd_Hook();
	void CRC_E1_Hook();
	void CRC_E2_Hook();
	void CRC_E3_Hook();
	void CRC_E4_Hook();
	void CRC_E5_Hook();
}

void _declspec(naked) CRC::CRC_Main_Hook(){
	_asm{
		push ecx
		cmp ecx,[Addr_Start]
		jb Ending
		cmp ecx,[Addr_End]
		ja Ending
		sub ecx,[Addr_Start]
		add ecx,Memory
Ending:
		mov dl,[ecx]
		pop ecx
		add dl,0x01
		jmp CRC_Main_ret
	}
}

void _declspec(naked) CRC::CRC_Main_Hook2(){
	_asm{
		xor eax,eax
		add eax,edx
		mov edx,[ebp+0x18]
		sub eax,0x08
		mov eax,[edx]
		shr eax,0x08
		xor ecx,ecx
		mov ecx,eax
		shl ecx,0x08
		mov ecx,[ebp+0x08]
		add ecx,[ebp-0x38]
		xor edx,edx
		mov ebx,[ebp+0x08]
		cmp ecx,[Addr_Start]
		jb Ending
		cmp ecx,[Addr_End]
		ja Ending
		sub ecx,[Addr_Start]
		add ecx,Memory
Ending:
		jmp CRC_Main_2_ret
	}
}

void _declspec(naked) CRC::CRC_2nd_Hook(){
	_asm{
		push ebx
		cmp ebx,[Addr_Start]
		jb Ending_2nd
		cmp ebx,[Addr_End]
		ja Ending_2nd
		sub ebx,[Addr_Start]
		add ebx,Memory
Ending_2nd:
		add al,[ebx]
		pop ebx
		mov ebx,[esp]
		jmp CRC_2nd_ret
	}
}


void _declspec(naked) CRC::CRC_3rd_Hook(){
	_asm{
		cmp edx,[CRC_2nd_1]
		je CRC_2nd_1_Load
		cmp edx,[CRC_2nd_2]
		je CRC_2nd_2_Load
		jmp Ending_3rd
CRC_2nd_1_Load:
		push [CRC_2nd_1_Dump]
		jmp CRC_3rd_ret
CRC_2nd_2_Load:
		push [CRC_2nd_2_Dump]
		jmp CRC_3rd_ret
Ending_3rd:
		push [edx]
		jmp CRC_3rd_ret
	}
}


void _declspec(naked) CRC::CRC_E1_Hook(){
	_asm{
		push edx
		cmp edx,[Addr_Start]
		jb Ending_E1
		cmp edx,[Addr_End]
		ja Ending_E1
		sub edx,[Addr_Start]
		add edx,Memory
Ending_E1:
		xor al,[edx]
		pop edx
		jmp CRC_E1_ret
	}
}


void _declspec(naked) CRC::CRC_E2_Hook(){
	_asm{
		cmp eax,[Addr_Start]
		jb Ending_E2
		cmp eax,[Addr_End]
		ja Ending_E2
		sub eax,[Addr_Start]
		add eax,Memory
Ending_E2:
		mov eax,[eax]
		push 0x4864D117
		jmp CRC_E2_ret
	}
}


void _declspec(naked) CRC::CRC_E3_Hook(){
	_asm{
		cmp eax,[Addr_Start]
		jb Ending_E3
		cmp eax,[Addr_End]
		ja Ending_E3
		sub eax,[Addr_Start]
		add eax,Memory
Ending_E3:
		mov eax,[eax]
		push [CRC_E3_ret]
		jmp CRC_E3_call
	}
}


void _declspec(naked) CRC::CRC_E4_Hook(){
	_asm{
		push ebx
		mov ebx,eax
		cmp ebx,[Addr_Start]
		jb Ending_E4
		cmp ebx,[Addr_End]
		ja Ending_E4
		sub ebx,[Addr_Start]
		add ebx,Memory
Ending_E4:
		mov ax,[ebx]
		pop ebx
		mov dword ptr [esp],0xA359DA6E
		jmp CRC_E4_ret
	}
}



void _declspec(naked) CRC::CRC_E5_Hook(){
	_asm{
		push edx
		cmp edx,[Addr_Start]
		jb Ending_E5
		cmp edx,[Addr_End]
		ja Ending_E5
		sub edx,[Addr_Start]
		add edx,Memory
Ending_E5:
		mov al,[edx]
		pop edx
		mov byte ptr [esp+0x04],ah
		jmp CRC_E5_ret
	}
}


void CRC::CRCbypass(){
	DWORD old;

	VirtualProtect((DWORD *)Addr_Start, (Addr_End - Addr_Start), PAGE_READWRITE, &old);

	Memory = (unsigned char *)malloc(Addr_End - Addr_Start + 0x0001000);
	memcpy(Memory, (DWORD *)Addr_Start, Addr_End - Addr_Start + 0x0001000);
	
	//Asm::Write_Hook("jne", CRC_Main_2, (DWORD)CRC_Main_Hook2);

	Asm::Write_Hook("jmp", CRC_Main, (DWORD)CRC_Main_Hook);

	Asm::Write_Hook("jmp", CRC_2nd, (DWORD)CRC_2nd_Hook);
	
	Asm::Write_Hook("jmp", CRC_3rd, (DWORD)CRC_3rd_Hook);
	
	Asm::Write_Hook("jmp", CRC_E1, (DWORD)CRC_E1_Hook);
	
	Asm::Write_Hook("jmp", CRC_E2, (DWORD)CRC_E2_Hook);

	Asm::Write_Hook("jmp", CRC_E3, (DWORD)CRC_E3_Hook);
	
	Asm::Write_Hook("jmp", CRC_E4, (DWORD)CRC_E4_Hook);
	
	Asm::Write_Hook("jmp", CRC_E5, (DWORD)CRC_E5_Hook);
	
	Asm::Write_code(VEH_DEBUGGER_DETECTION, "31 C0 C3");


	Asm::Write_code(0x00F840A0, "C2 04 00");

	Asm::Write_code(0x00F84540, "C2 04 00");



}
