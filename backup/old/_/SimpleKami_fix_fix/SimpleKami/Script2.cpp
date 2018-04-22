#include"Script2.h"

namespace SCR2{
	DWORD dwGet_Unknown_Pointer = 0x00D58D70;
	DWORD dwDirectly_Teleport = 0x00DACD70;
	DWORD dwChar_Base_Pointer = 0x0147197C;

	DWORD dwMouse_Hook = 0x00B92E92;
	DWORD dwMouse_Hook_ret = dwMouse_Hook + 6;
}

namespace SCR2{
	void _stdcall NewTeleportXY(int teleX, int teleY);
	void Mouse_Hook();
}


void _stdcall SCR2::NewTeleportXY(int teleX, int teleY){
	_asm{
		mov esi,dwChar_Base_Pointer
		mov esi,[esi]
		test esi,esi
		je NewTeleportXY_Ending
		lea ecx,[esi+0x04]
		call dwGet_Unknown_Pointer
		test eax,eax
		je NewTeleportXY_Ending
		push [teleY]
		push [teleX]
		push 0x00
		mov ecx,eax
		call dwDirectly_Teleport
NewTeleportXY_Ending:
	}
}

void _declspec(naked) SCR2::Mouse_Hook(){
	_asm{
		mov esi,[eax+0x978]
		pushad
		test esi,esi
		je Mouse_Hook_Ending
		cmp [eax+0x9DC],0x0C
		jne Mouse_Hook_Ending
		push [esi+0x90]//Y
		push [esi+0x8C]//X
		call NewTeleportXY
Mouse_Hook_Ending:
		popad
		jmp dwMouse_Hook_ret
	}
}


void SCR2::SSMouseFly(BOOL Flag){
	if(Flag == TRUE){
		Asm::Write_Hook("jmp", dwMouse_Hook, (DWORD)Mouse_Hook);
	}else{
		Asm::Write_code(dwMouse_Hook, "8B B0 78 09 00 00");
	}
}