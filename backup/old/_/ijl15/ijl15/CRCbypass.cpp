#include"Asm.h"
#include"CRCbypass.h"

	unsigned char *Memory;
	DWORD Addr_Start = 0x00401000;
	DWORD Addr_End   = 0x0127F000;

	//8B 4D 18 8B 09
	DWORD CRC_Main = 0x010F9AA9;//v101.1
	DWORD CRC_Main_ret = CRC_Main + 5;

	DWORD CRC_Main2 = 0x00FD230F;//v100.1
	DWORD CRC_Main2_ret = 0x00FD2529;

	DWORD CRC_2nd = 0x01ECAF3F;//v101.1
	DWORD CRC_2nd_ret = CRC_2nd + 6;

	DWORD CRC_3rd = 0x01DD6CCE;//v101.1
	DWORD CRC_3rd_ret = 0x01DC6C22;

	DWORD CRC_2nd_1 = CRC_2nd;//v101.1
	DWORD CRC_2nd_1_Dump = 0x83590102;

	DWORD CRC_2nd_2 = CRC_2nd + 4;//v101.1
	DWORD CRC_2nd_2_Dump = 0x1C8904EC;
	
	DWORD CRC_E1 = 0x0057797A;//v101.1
	DWORD CRC_E1_ret = 0x007B69BE;

	DWORD CRC_E2 = 0x007AEEAC;//v101.1
	DWORD CRC_E2_ret = CRC_E2 + 6;
	
	DWORD CRC_E4 = 0x007BB91F;//v101.1
	DWORD CRC_E4_ret = CRC_E4 + 5;
	
	DWORD CRC_E5 = 0x005771FA;//101.1
	DWORD CRC_E5_ret = CRC_E5 + 5;


	void *CRC;


void _declspec(naked) CRC_Main_Hook(){
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

void _declspec(naked) CRC_Main2_Hook(){
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
		jb Ending2
		cmp ecx,[Addr_End]
		ja Ending2
		sub ecx,[Addr_Start]
		add ecx,Memory
Ending2:
		jmp CRC_Main2_ret
	}
}

void _declspec(naked) CRC_2nd_Hook(){
	_asm{
		cmp ecx,[Addr_Start]
		jb Ending_2nd
		cmp ecx,[Addr_End]
		ja Ending_2nd
		sub ecx,[Addr_Start]
		add ecx,Memory
Ending_2nd:
		add al,[ecx]
		pop ecx
		sub esp,0x04
		jmp CRC_2nd_ret
	}
}


void _declspec(naked) CRC_3rd_Hook(){
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


void _declspec(naked) CRC_3rd_Hook2(){
	_asm{
		push [edx]
		push edx
		cmp edx,CRC_2nd-0x100
		jb Justice
		cmp edx,CRC_2nd+0x100
		ja Justice
		sub edx,CRC_2nd
		add edx,0x200
		add edx,CRC
		mov edx,[edx]
		mov dword ptr [esp+0x04],edx
Justice:
		pop edx
		jmp CRC_3rd_ret
	}
}

void _declspec(naked) CRC_E1_Hook(){
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


void _declspec(naked) CRC_E2_Hook(){
	_asm{
		cmp eax,[Addr_Start]
		jb Ending_E2
		cmp eax,[Addr_End]
		ja Ending_E2
		sub eax,[Addr_Start]
		add eax,Memory
Ending_E2:
		mov eax,[eax]
		push esi
		mov dword ptr [ebp],eax
		jmp CRC_E2_ret
	}
}


void _declspec(naked) CRC_E4_Hook(){
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
		pushfd
		push ebx
		jmp CRC_E4_ret
	}
}



void _declspec(naked) CRC_E5_Hook(){
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
		mov [esp],dl
		jmp CRC_E5_ret
	}
}



void CRCbypass(HANDLE hThread){
	DWORD old;

	while(*(DWORD *)(CRC_2nd_2) != CRC_2nd_2_Dump){
		Sleep(20);
	}

	SuspendThread(hThread);//STOP

	VirtualProtect((DWORD *)Addr_Start, (Addr_End - Addr_Start), PAGE_READWRITE, &old);
	Memory = (unsigned char *)malloc(Addr_End - Addr_Start + 0x0001000);
	memcpy(Memory, (DWORD *)Addr_Start, Addr_End - Addr_Start + 0x0001000);


	Asm::Write_Hook("jmp", CRC_Main, (DWORD)CRC_Main_Hook);

	Asm::Write_Hook("jmp", CRC_2nd, (DWORD)CRC_2nd_Hook);
	
	Asm::Write_Hook("jmp", CRC_3rd, (DWORD)CRC_3rd_Hook);

	Asm::Write_Hook("jmp", CRC_E1, (DWORD)CRC_E1_Hook);
	
	Asm::Write_Hook("jmp", CRC_E2, (DWORD)CRC_E2_Hook);
	
	Asm::Write_Hook("jmp", CRC_E4, (DWORD)CRC_E4_Hook);
	
	Asm::Write_Hook("jmp", CRC_E5, (DWORD)CRC_E5_Hook);
	

}
