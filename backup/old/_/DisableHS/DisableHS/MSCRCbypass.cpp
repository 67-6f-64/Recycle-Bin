#include"MSCRCbypass.h"
namespace CRC{
	unsigned char *Memory;
	DWORD Addr_Start = 0x00401000;
	DWORD Addr_End = 0x00FC0000;

	DWORD CRC_Main = 0x00E89089;
	DWORD CRC_Main_ret = CRC_Main + 5;

	DWORD CRC_2nd = 0x018EB0B0;
	DWORD CRC_2nd_ret = CRC_2nd + 5;

	DWORD CRC_3rd = 0x017FEA58;
	DWORD CRC_3rd_ret = 0x017F4CED;
	DWORD CRC_2nd_1 = CRC_2nd - 2;
	DWORD CRC_2nd_1_Dump = 0x0102D101;
	DWORD CRC_2nd_2 = CRC_2nd + 2;
	DWORD CRC_2nd_2_Dump = 0x83240C8B;

	DWORD CRC_F2 = 0x00CD0ACF;
	DWORD CRC_F2_ret = CRC_F2 + 6;

	DWORD CRC_F3 = 0x00CD0A2D;
	DWORD CRC_F3_ret = CRC_F3 + 6;

	DWORD CRC_F4 = 0x0092133A;
	DWORD CRC_F4_ret = CRC_F4 + 8;

	DWORD CRC_F1 = 0x005208F3;
	DWORD CRC_F1_ret = 0x00921325;
	
	void CRC_Main_Hook();
	void CRC_2nd_Hook();
	void CRC_3rd_Hook();
	void CRC_F1_Hook();
	void CRC_F2_Hook();
	void CRC_F3_Hook();
	void CRC_F4_Hook();
}

void _declspec(naked) CRC::CRC_Main_Hook(){
	_asm{
		cmp ecx,[Addr_Start]
		jb Ending
		cmp ecx,[Addr_End]
		ja Ending
		sub ecx,[Addr_Start]
		add ecx,Memory
Ending:
		mov dl,[ecx]
		add dl,0x01
		jmp CRC_Main_ret
	}
}

void _declspec(naked) CRC::CRC_2nd_Hook(){
	_asm{
		cmp ecx,[Addr_Start]
		jb Ending_2nd
		cmp ecx,[Addr_End]
		ja Ending_2nd
		sub ecx,[Addr_Start]
		add ecx,Memory
Ending_2nd:
		add al,[ecx]
		mov ecx,[esp]
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

void _declspec(naked) CRC::CRC_F2_Hook(){
	_asm{
		push edx
		cmp edx,[Addr_Start]
		jb Ending_F2
		cmp edx,[Addr_End]
		ja Ending_F2
		sub edx,[Addr_Start]
		add edx,Memory
Ending_F2:
		xor al,[edx]
		pop edx
		pushfd
		mov [esp],ah
		jmp CRC_F2_ret
	}
}

void _declspec(naked) CRC::CRC_F3_Hook(){
	_asm{
		cmp eax,[Addr_Start]
		jb Ending_F3
		cmp eax,[Addr_End]
		ja Ending_F3
		sub eax,[Addr_Start]
		add eax,Memory
Ending_F3:
		mov eax,[eax]
		lea esp,[esp+0x28]
		jmp CRC_F3_ret
	}
}

void _declspec(naked) CRC::CRC_F4_Hook(){
	_asm{
		push ecx
		mov ecx,eax
		cmp ecx,[Addr_Start]
		jb Ending_F4
		cmp ecx,[Addr_End]
		ja Ending_F4
		sub ecx,[Addr_Start]
		add ecx,Memory
Ending_F4:
		mov ax,[ecx]
		pop ecx
		push ebx
		mov [esp+0x04],ah
		jmp CRC_F4_ret
	}
}

void _declspec(naked) CRC::CRC_F1_Hook(){
	_asm{
		push edx
		cmp edx,[Addr_Start]
		jb Ending_F1
		cmp edx,[Addr_End]
		ja Ending_F1
		sub edx,[Addr_Start]
		add edx,Memory
Ending_F1:
		mov al,[edx]
		pop edx
		jmp CRC_F1_ret
	}
}


void CRC::CRCbypass(){
	DWORD old;
	VirtualProtect((DWORD *)Addr_Start, (Addr_End - Addr_Start), PAGE_READWRITE, &old);

	Memory = (unsigned char *)malloc(Addr_End - Addr_Start + 0x0001000);
	memcpy(Memory, (DWORD *)Addr_Start, Addr_End - Addr_Start + 0x0001000);
	
	Asm::Write_jmp(CRC_Main, (DWORD)CRC_Main_Hook);
	Asm::Write_jmp(CRC_2nd, (DWORD)CRC_2nd_Hook);
	Asm::Write_jmp(CRC_3rd, (DWORD)CRC_3rd_Hook);
	Asm::Write_jmp(CRC_F2, (DWORD)CRC_F2_Hook);
	Asm::Write_jmp(CRC_F3, (DWORD)CRC_F3_Hook);
	Asm::Write_jmp(CRC_F4, (DWORD)CRC_F4_Hook);
	Asm::Write_jmp(CRC_F1, (DWORD)CRC_F1_Hook);

}
