#include"CRCbypass.h"
#include"lib/Asm.h"
#include<stdio.h>
#include<time.h>
namespace CRC{
	unsigned char *Memory;
	DWORD Addr_Start = 0x00401000;
	DWORD Addr_End   = 0x01200000;

	DWORD CRC_Main = 0x00FD1479;//v99.2
	DWORD CRC_Main_ret = CRC_Main + 5;


	DWORD CRC_2nd = 0x02074F0E;//v99.2
	DWORD CRC_2nd_ret = CRC_2nd + 5;

	DWORD CRC_3rd = 0x01F7F942;//v99.2
	DWORD CRC_3rd_ret = 0x01F72E64;

	DWORD CRC_2nd_1 = CRC_2nd - 2;//v99.2
	DWORD CRC_2nd_1_Dump = *(DWORD *)CRC_2nd_1;

	DWORD CRC_2nd_2 = CRC_2nd + 2;//v99.2
	DWORD CRC_2nd_2_Dump = *(DWORD *)CRC_2nd_2;

	
	DWORD CRC_E1 = 0x004CBEBC;//v99.2
	DWORD CRC_E1_ret = 0x0053EE70;

	DWORD CRC_E2 = 0x0053EF97;//v99.2
	DWORD CRC_E2_ret = CRC_E2 + 7;

	
	DWORD CRC_E4 = 0x0053EFD2;//v99.2
	DWORD CRC_E4_ret = CRC_E4 + 10;
	
	DWORD CRC_E5 = 0x004CC274;//99.2
	DWORD CRC_E5_ret = CRC_E5 + 6;

	DWORD dwVEH_Detection = 0x0053EF50;

	DWORD dwLauncher = 0x00901760;
	
	DWORD dwMutex_Prev = 0x00DF0018;
	DWORD dwMutex_Next = 0x00DF0079;

	DWORD dwDual_Client = 0x00713271;
	DWORD dwDual_Client_ret = dwDual_Client + 5;
	
	DWORD dwHeart_Beat_Response = 0x00E7EA60;
	DWORD dwStart_Service = 0x00E7E5C0;
	//DWORD dwAdBlock = 0x00DC2540;
	
	DWORD dwTitle_Hook = 0x00DF6974;
	DWORD dwTitle_Hook_ret = dwTitle_Hook + 6;

	char Title[0x100]="HSlessStory";
	//55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 18 F2 FF FF 
	DWORD dwHS_Update_Skip = 0x00EA0BF9;
}

namespace CRC{
	void CRC_Main_Hook();
	void CRC_2nd_Hook();
	void CRC_3rd_Hook();
	void CRC_E1_Hook();
	void CRC_E2_Hook();
	void CRC_E4_Hook();
	void CRC_E5_Hook();
	void DualClientbypass();
	void _stdcall Modification_10Bytes(BYTE garbage[]);
	void Title_Hook();
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
		pop ecx
		push ecx
		mov cl,0xCD
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
		push [CRC_E1_ret]
		//jmp CRC_E1_call
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
		push [CRC_E2_ret]
		//jmp CRC_E2_call
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
		push 0x9AA3AD73
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
		push [esp]
		jmp CRC_E5_ret
	}
}


void _stdcall CRC::Modification_10Bytes(BYTE garbage[]){
	int i;
	srand(time(NULL));//èâä˙âª
	for(i=0; i<10; i++){
		garbage[i] = rand()%0x100;
	}
}

void _declspec(naked) CRC::DualClientbypass(){
	_asm{
		pushad
		push eax
		call Modification_10Bytes
		popad
		push eax
		lea ecx,[esp+0x2C]
		jmp dwDual_Client_ret
	}
}

void _declspec(naked) CRC::Title_Hook(){
	_asm{
		push eax
		mov ecx,[ebp-0x54]
		neg ecx
		lea eax,[Title]
		mov dword ptr [esp+0x04],eax
		jmp dwTitle_Hook_ret
	}
}



void CRC::CRCbypass(int mode){
	HMODULE KERNELBASE = GetModuleHandleA("KERNELBASE.dll");
	Asm::Write_code((DWORD)GetProcAddress(KERNELBASE, "CreateMutexExW")+0x70+1, "00");

	return;
	DWORD old;


	while(*(DWORD *)CRC_2nd_2 != 0xC483240C){
		Sleep(10);
	}

	CRC_2nd_1_Dump = *(DWORD *)CRC_2nd_1;
	CRC_2nd_2_Dump = *(DWORD *)CRC_2nd_2;

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


	//VEH Debug Detection
	Asm::Write_code(dwVEH_Detection, "31 C0 C3");
	
	//Launcher Skipper
	Asm::Write_code(dwLauncher, "31 C0 40 C3");

	//Dual Client!
	Asm::Write_Hook("jmp", dwDual_Client, (DWORD)DualClientbypass);

	//Mutex Killer
	Asm::Write_Hook("jmp", dwMutex_Prev, dwMutex_Next);
	

	if(mode == IDYES){
		//HSless
		Asm::Write_code(dwHS_Update_Skip, "8B C3 C2 10 00");
		Asm::Write_code(dwStart_Service, "C2 04 00");
		Asm::Write_code(dwHeart_Beat_Response, "C2 04 00");
		Asm::Write_Hook("jmp", dwTitle_Hook, (DWORD)Title_Hook, 1);
	}else if(mode == IDNO){
		LoadLibraryA("DLL/DeadShield.dll");
	}

}
