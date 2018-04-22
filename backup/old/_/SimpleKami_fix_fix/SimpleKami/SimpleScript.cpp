#include"SimpleScript.h"

//==================== Address ====================
namespace SCR{
	DWORD dwTele_call = 0x004526E0;
	DWORD dwLoot_call_1 = 0x00542D90;
	DWORD dwLoot_call_2 = 0x005B8F00;
	DWORD dwChannel_call_1 = 0x00491E70;//56 8B 35 ?? ?? ?? ?? 85 F6 74 18 8B 46 04 8B 50 4C
	DWORD dwChannel_call_2 = 0x005E3ED0;//85 C9 74 05 E8 ?? ?? ?? ?? 8B 35 ?? ?? ?? ?? 83 BE ?? ?? ?? ?? 00 0F 85 ?? ?? ?? ?? 8B 86 ?? ?? ?? ?? 8B 48 69 51 83 C0 61 50 E8 start function

	//v98.2
	DWORD dwNDFA_1 = 0x00D3985B;
	DWORD dwNDFA_2 = 0x00D3986B;
	DWORD dwNDFA_3 = 0x00D3987F;
	DWORD dwNDFA_4 = 0x00D39920;
	DWORD dwNDFA_5 = 0x00D39929;
	DWORD dwNDFA_jne = 0x00D3999F;

	//v98.2
	DWORD dwKamiLoot = 0x005B90E1;
	DWORD dwKamiLoot_ret = dwKamiLoot + 5;

	//v98.2
	DWORD dwInstantDrop1 = 0x005B7E85;
	DWORD dwInstantDrop2 = 0x00518C87;

	//v98.2
	DWORD dwNoDelay = 0x00D00A11+6;
	DWORD dwUnlimitedMP = 0x008E3F4D;

	//v98.2
	DWORD dwHideDamage1 = 0x0049DD30;
	DWORD dwHideDamage2 = 0x0049BE00;

	//v98.2
	DWORD dwPhysical = 0x00D29C10;
	DWORD dwMagical = 0x007BB3F0;

	//v98.2
	DWORD dwHideEffect1 = 0x00C9B703;
	DWORD dwHideEffect2 = 0x007A3435;

	//DWORD dwBackground = 0x007465F0;//
	//DWORD dwCloud = 0x00742F40;//

	//v98.2
	DWORD dwPinTyper1 = 0x00735DF6 + 1;
	DWORD dwPinTyper2 = 0x00737517 + 1;
	DWORD dwPicTyper1 = 0x008FB1C6;
	DWORD dwPicTyper2 = 0x008FA7B6;
	DWORD dwPicTyper3 = 0x008FA7D2;
	DWORD dwPicTyper3_CCtrlEdit = 0x00568060;//bugged
	DWORD dwPicTyper3_CCtrlEditEx = 0x0056BC10;

	DWORD dwOptionList = 0x00DFFB57;
}



//==================== Pointer ====================
namespace SCR{
	DWORD dwChar_Base_Pointer = 0x0147197C;
	DWORD dwTele_Toggle_Offset = 0x8098;
	DWORD dwTele_X_Offset = 0x80BC;
	DWORD dwTele_Y_Offset = 0x80B0;
	DWORD dwFinal_Attack_Offset = 0x805C;

	DWORD dwMob_Base_Pointer = 0x014760C4;
	DWORD dwMob_Count_Offset = 0x10;
	DWORD dwMob_Struct_Offset = 0x28;
	DWORD dwMob_Next_Offset = 0x04;
	DWORD dwMob_1_Offset = 0x17C;
	DWORD dwMob_2_Offset = 0x24;
	DWORD dwMob_Current_Offset = 0x14;
	DWORD dwMob_Prev_Offset = 0x0C;
	DWORD dwMob_X_Offset = 0x58;
	DWORD dwMob_Y_Offset = 0x5C;
	DWORD dwMob_Invisible_X_Offset = 0x60;
	DWORD dwMob_Invisible_Y_Offset = 0x64;

	DWORD dwPeople_Base_Pointer = 0x014760C0;
	DWORD dwPeople_Count_Offset = 0x18;

	DWORD dwMacro_Skill_Base_Pointer = 0x01476434;

	DWORD dwChannel_Pointer = 0x01491B00;

	DWORD dwItem_Base_Pointer = 0x01476D74;
	DWORD dwItem_Count_Offset = 0x14;
}


//==================== Private Functions ====================
namespace SCR{
	void KamiLoot();
}

//==================== Scripts! ====================

void SCR::AlwaysEnable(){
		Asm::Write_code(dwPinTyper1, "84", 0);
		Asm::Write_code(dwPinTyper2, "84", 0);
		Asm::Write_code(dwPicTyper1, "90 E9", 0);
		Asm::Write_code(dwPicTyper2, "00", 0);
		Asm::Write_Hook("call", dwPicTyper3, dwPicTyper3_CCtrlEdit, 0);
		Asm::Write_code(dwInstantDrop1, "25");
		Asm::Write_code(dwInstantDrop2, "EB 04");
		Asm::Write_code(dwPhysical, "C2 04 00", 0);
		Asm::Write_code(dwMagical, "C2 04 00", 0);
		Asm::Write_code(dwOptionList, "EB", 0);
}

void SCR::OtherScript(BOOL Flag){
	if(Flag == TRUE){
		//Asm::Write_code(dwBackground, "C3", 0);
		//Asm::Write_code(dwCloud, "C2 08 00", 0);
		Asm::Write_code(dwHideDamage1, "C2 0C 00", 0);
		Asm::Write_code(dwHideDamage2, "C2 1C 00", 0);
		Asm::Write_code(dwHideEffect1, "90 E9", 0);
		Asm::Write_code(dwHideEffect2, "90 E9", 0);
	}else{
		//Asm::Write_code(dwBackground, "6A", 0);
		//Asm::Write_code(dwCloud, "55 8D 6C", 0);
		Asm::Write_code(dwHideDamage1, "6A FF 68", 0);
		Asm::Write_code(dwHideDamage2, "55 8D 6C", 0);
		Asm::Write_code(dwHideEffect1, "0F 85", 0);
		Asm::Write_code(dwHideEffect2, "0F 84", 0);
	}
}

void SCR::AllScript(BOOL Flag){
	if(Flag == TRUE){
		Asm::Write_code(dwNDFA_1, NULL, 6);
		Asm::Write_code(dwNDFA_2, NULL, 2);
		Asm::Write_code(dwNDFA_3, NULL, 6);
		Asm::Write_code(dwNDFA_4, NULL, 2);
		Asm::Write_code(dwNDFA_5, NULL, 7);

		Asm::Write_Hook("jmp", dwKamiLoot, (DWORD)KamiLoot);
		Asm::Write_code(0x00D22105, "6A 01", 1);//Enable Floating

		Asm::Write_code(dwNoDelay, "01", 0);
		Asm::Write_code(dwUnlimitedMP, "90 90", 0);


	}else{
		Asm::Write_Hook("jne",dwNDFA_1, dwNDFA_jne);
		Asm::Write_code(dwNDFA_2, "75 18", 0);
		Asm::Write_Hook("jne",dwNDFA_3, dwNDFA_jne);
		Asm::Write_code(dwNDFA_4, "77 7D", 0);
		Asm::Write_code(dwNDFA_5, "FF 24 8D B4 99 D3 00");

		Asm::Write_code(dwKamiLoot, "8D 54 24 34 52", 0);
		Asm::Write_code(0x00D22105, "33 DB 53");//Disable Floating

		Asm::Write_code(dwNoDelay, "00", 0);
		Asm::Write_code(dwUnlimitedMP, "7D 06", 0);

	}
}


void _declspec(naked) SCR::KamiLoot(){
	_asm{
		pushad
		push eax//Y
		push ebp//X
		call TeleportXY
		popad
		lea edx,[esp+0x34]
		push edx
		jmp dwKamiLoot_ret
	}
}

//==================== following functions will work forever ====================

namespace SCR{//This is used for saving character XY coordinates
	int XYtemp[16]={0};
}


int _stdcall SCR::GetItemCount(){
	int ItemCount = 0;

	_asm{
		mov esi,dwItem_Base_Pointer
		mov esi,[esi]
		test esi,esi
		je GetItemCount_Ending
		add esi,dwItem_Count_Offset
		mov esi,[esi]
		mov dword ptr [ItemCount],esi
GetItemCount_Ending:
	}

	return ItemCount;
}


void _stdcall SCR::PickUp(){
	_asm{
		mov ecx,dwChar_Base_Pointer
		mov ecx,[ecx]
		test ecx,ecx
		je Pickup_Ending
		add ecx,4
		lea eax,[XYtemp]
		push eax
		call dwLoot_call_1
		mov ecx,dwItem_Base_Pointer
		mov ecx,[ecx]
		test ecx,ecx
		je Pickup_Ending
		push eax
		call dwLoot_call_2
		
Pickup_Ending:
	}
}


void _stdcall SCR::ChannelChange(){
	_asm{
		mov ebx,dwChannel_Pointer
		mov ebx,[ebx]
		inc ebx
		cmp ebx,14
		jb NextChannel
		xor ebx,ebx
NextChannel:
		push ebx
		call dwChannel_call_1
		mov ecx,eax
		call dwChannel_call_2
	}
}


int _stdcall SCR::GetCurrentChannel(){
	int Channel;

	_asm{
		mov ebx,dwChannel_Pointer
		mov ebx,[ebx]
		mov dword ptr [Channel],ebx
	}
	return Channel;
}


int _stdcall SCR::GetPeopleCount(){
	int People = 0;

	_asm{
		mov ebx,dwPeople_Base_Pointer
		mov ebx,[ebx]
		test ebx,ebx
		je GetPeopleCount_Ending
		add ebx,dwPeople_Count_Offset
		mov ebx,[ebx]
		mov dword ptr [People],ebx

GetPeopleCount_Ending:

	}
	return People;
}


MobData _stdcall SCR::GetMobXY(){
	MobData Mob = {0};
	Mob.Flag = FALSE;

	_asm{
		mov esi,dwMob_Base_Pointer
		mov esi,[esi]
		test esi,esi
		je GetMobXY_False
		mov eax,esi
		add eax,dwMob_Count_Offset
		mov eax,[eax]//Count
		test eax,eax
		je GetMobXY_False
		add esi,dwMob_Struct_Offset
		mov esi,[esi]//Mob Struct
		test esi,esi
		je GetMobXY_False

//GetMobXY_Mob1st
		mov edx,esi
		add edx,dwMob_Next_Offset
		mov edx,[edx]
		test edx,edx
		je GetMobXY_False
		add edx,dwMob_1_Offset
		mov edx,[edx]
		test edx,edx
		je GetMobXY_False
		add edx,dwMob_2_Offset
		mov edx,[edx]
		test edx,edx
		je GetMobXY_False
		mov eax,edx
		add eax,dwMob_Invisible_X_Offset
		mov eax,[eax]
		mov ebx,edx
		add ebx,dwMob_Invisible_Y_Offset
		mov ebx,[ebx]
		or eax,ebx
		test eax,eax
		jne GetMobXY_True

		//Prev
		sub esi,dwMob_Prev_Offset
		mov esi,[esi]
		test esi,esi
		je GetMobXY_False

GetMobXY_NextMob://2~
		mov edx,esi
		add edx,dwMob_Current_Offset
		mov edx,[edx]
		test edx,edx
		je GetMobXY_False
		add edx,dwMob_1_Offset
		mov edx,[edx]
		test edx,edx
		je GetMobXY_False
		add edx,dwMob_2_Offset
		mov edx,[edx]
		test edx,edx
		je GetMobXY_False
		mov eax,edx
		add eax,dwMob_Invisible_X_Offset
		mov eax,[eax]
		mov ebx,edx
		add ebx,dwMob_Invisible_Y_Offset
		mov ebx,[ebx]
		or eax,ebx
		test eax,eax
		jne GetMobXY_True
		add esi,dwMob_Next_Offset
		mov esi,[esi]
		test esi,esi
		je GetMobXY_False
		jmp GetMobXY_NextMob

GetMobXY_True:
		mov eax,edx
		add eax,dwMob_X_Offset
		mov eax,[eax]
		mov ebx,edx
		add ebx,dwMob_Y_Offset
		mov ebx,[ebx]
		mov dword ptr [Mob.X],eax
		mov dword ptr [Mob.Y],ebx
		inc [Mob.Flag]//TRUE
GetMobXY_False:

	}

	return Mob;
}


void _stdcall SCR::TeleportXY(int TeleX, int TeleY){
	_asm{
		mov esi,dwChar_Base_Pointer
		mov esi,[esi]
		test esi,esi
		je TeleportXY_Ending
		push 0x01
		mov ecx,esi
		add ecx,dwTele_Toggle_Offset
		call dwTele_call
		push [TeleX]
		mov ecx,esi
		add ecx,dwTele_X_Offset
		call dwTele_call
		push [TeleY]
		mov ecx,esi
		add ecx,dwTele_Y_Offset
		call dwTele_call
TeleportXY_Ending:
	}
}


void _stdcall SCR::UseFinalAttack(){
	_asm{
		mov esi,dwChar_Base_Pointer
		mov esi,[esi]
		test esi,esi
		je UseFinalAttack_Ending
		mov edi,dwMacro_Skill_Base_Pointer
		mov edi,[edi]
		test edi,edi
		je UseFinalAttack_Ending
		mov edi,[edi+0x20]
		test edi,edi
		je UseFinalAttack_Ending
		mov edi,[edi]
		add esi,dwFinal_Attack_Offset
		mov dword ptr [esi],edi
UseFinalAttack_Ending:
	}
}