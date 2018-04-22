#include"MSCRC.h"
#include"Function.h"

//v103.1
DWORD dwMSCRC1 = 0x0110EA69;//mov dl,[ecx]
DWORD dwMSCRC1_2 = 0x0110E84F;

DWORD dwThemidaCRC_Init = 0x01064330;

DWORD dwVMCRC1 = 0x005777CA;//mov eax,[eax]
DWORD dwVMCRC2 = 0x004DBCE4;//xor al,[edx]
DWORD dwVMCRC3 = 0x007BCF86;//mov al,[edx]
DWORD dwVMCRC4 = 0x0057DF55;//mov ax,[eax]

DWORD dwMSCRC1_Return = dwMSCRC1 + 0x05;
DWORD dwMSCRC1_2_Return = dwMSCRC1_2 + 0x06;

DWORD dwVMCRC1_Return = dwVMCRC1 + 0x07;
DWORD dwVMCRC2_Return = dwVMCRC2 + 0x07;
DWORD dwVMCRC3_Return = dwVMCRC3 + 0x06;
DWORD dwVMCRC4_Return = dwVMCRC4 + 0x07;
//__

BYTE *MS_Memory;
DWORD dwMS_Memory_Start = 0;
DWORD dwMS_Memory_End = 0;

void _declspec(naked) RangeCheckESI(){
	_asm{
		cmp ebx,[dwMS_Memory_Start]
		jb RC_Ending
		cmp ebx,[dwMS_Memory_End]
		ja RC_Ending
		sub ebx,[dwMS_Memory_Start]
		add ebx,MS_Memory
RC_Ending:
		ret
	}
}

void _declspec(naked) MSCRC1_Hook(){
	_asm{
		push ebx
		mov ebx,ecx
		call RangeCheckESI
		mov dl,[ebx]
		pop ebx
		add dl,0x01
		jmp dword ptr [dwMSCRC1_Return]
	}
}


void _declspec(naked) MSCRC1_2_Hook(){
	_asm{
		je MSCRC1_2_Ending
		//orgcode_start
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
		//orgcode_end
		push ebx
		mov ebx,ecx
		call RangeCheckESI
		mov dl,[ebx]
		pop ebx
		add dl,0x01
		jmp dword ptr [dwMSCRC1_Return]

MSCRC1_2_Ending:
		jmp dword ptr [dwMSCRC1_2_Return]
	}
}


void _declspec(naked) VMCRC1_Hook(){
	_asm{
		push ebx
		mov ebx,eax
		call RangeCheckESI
		mov eax,[ebx]
		pop ebx
		push 0x4864D117
		jmp dword ptr [dwVMCRC1_Return]
	}
}


void _declspec(naked) VMCRC2_Hook(){
	_asm{
		push ebx
		mov ebx,edx
		call RangeCheckESI
		xor al,[ebx]
		pop ebx
		mov word ptr [esp+0x04],si
		jmp dword ptr [dwVMCRC2_Return]
	}
}


void _declspec(naked) VMCRC3_Hook(){
	_asm{
		push ebx
		mov ebx,edx
		call RangeCheckESI
		mov al,[ebx]
		pop ebx
		push [esp+0x10]
		jmp dword ptr [dwVMCRC3_Return]
	}
}


void _declspec(naked) VMCRC4_Hook(){
	_asm{
		push ebx
		mov ebx,eax
		call RangeCheckESI
		mov ax,[ebx]
		pop ebx
		push [esp+0x08]
		jmp dword ptr [dwVMCRC4_Return]
	}
}

void MemoryBackup(){
	IMAGE_NT_HEADERS *nth = (IMAGE_NT_HEADERS *)((DWORD)GetModuleHandleA(NULL) + PIMAGE_DOS_HEADER(GetModuleHandleA(NULL))->e_lfanew);
	MS_Memory = (BYTE *)malloc(nth->OptionalHeader.SizeOfImage);
	RtlCopyMemory(MS_Memory, (DWORD *)nth->OptionalHeader.ImageBase, nth->OptionalHeader.SizeOfImage);
	dwMS_Memory_Start = (DWORD)nth->OptionalHeader.ImageBase;
	dwMS_Memory_End = dwMS_Memory_Start + (DWORD)nth->OptionalHeader.SizeOfImage;
}

void MSCRCBypass(){
	MemoryBackup();
	WriteJMP(dwMSCRC1_2, (DWORD)MSCRC1_2_Hook);
	//WriteJMP(dwMSCRC1, (DWORD)MSCRC1_Hook);
	
	/*
	WriteJMP(dwVMCRC1, (DWORD)VMCRC1_Hook);
	WriteJMP(dwVMCRC2, (DWORD)VMCRC2_Hook);
	WriteJMP(dwVMCRC3, (DWORD)VMCRC3_Hook);
	WriteJMP(dwVMCRC4, (DWORD)VMCRC4_Hook);
	*/
	
	WriteCODE(dwThemidaCRC_Init, "31 C0 C3");
}