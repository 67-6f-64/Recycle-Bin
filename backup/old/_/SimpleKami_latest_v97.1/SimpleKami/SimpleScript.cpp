#include"SimpleScript.h"

//==================== Address ====================
namespace SCR{
	DWORD dwTele_call = 0x0044F460;
	DWORD dwLoot_call_1 = 0x005334A0;
	DWORD dwLoot_call_2 = 0x005A7AE0;
	DWORD dwChannel_call_1 = 0x0048B880;
	DWORD dwChannel_call_2 = 0x005D1AB0;

	DWORD dwNDFA_1 = 0x00CFD74B;
	DWORD dwNDFA_2 = 0x00CFD75B;
	DWORD dwNDFA_3 = 0x00CFD76F;
	DWORD dwNDFA_4 = 0x00CFD810;
	DWORD dwNDFA_5 = 0x00CFD819;
	DWORD dwNDFA_jne = 0x00CFD88F;

	DWORD dwKamiLoot = 0x005A7CBF;
	DWORD dwKamiLoot_ret = dwKamiLoot + 6;

	DWORD dwInstantDrop1 = 0x005A6AC5;
	DWORD dwInstantDrop2 = 0x0050C4B7;

	DWORD dwNoDelay = 0x00CC9A77;
	DWORD dwUnlimitedMP = 0x008C1EE4;

	DWORD dwHideDamage1 = 0x00496790;
	DWORD dwHideDamage2 = 0x00494870;

	DWORD dwPhysical = 0x00CED310;
	DWORD dwMagical = 0x0079D410;

	DWORD dwHideEffect1 = 0x00C6B943;
	DWORD dwHideEffect2 = 0x00785DA5;

	DWORD dwBackground = 0x007465F0;
	DWORD dwCloud = 0x00742F40;

	DWORD dwPinTyper1 = 0x00719C46 + 1;
	DWORD dwPinTyper2 = 0x0071B367 + 1;
	DWORD dwPicTyper1 = 0x008D7786;
	DWORD dwPicTyper2 = 0x008D6D76;
	DWORD dwPicTyper3 = 0x008D6D92;
	DWORD dwPicTyper3_CCtrlEdit = 0x00558300;
	DWORD dwPicTyper3_CCtrlEditEx = 0x0055BEB0;
}



//==================== Pointer ====================
namespace SCR{
	DWORD dwChar_Base_Pointer = 0x013FF0CC;
	DWORD dwTele_Toggle_Offset = 0x7B04;
	DWORD dwTele_X_Offset = 0x7B28;
	DWORD dwTele_Y_Offset = 0x7B1C;
	DWORD dwFinal_Attack_Offset = 0x7AD4;

	DWORD dwMob_Base_Pointer = 0x014037DC;
	DWORD dwMob_Count_Offset = 0x10;
	DWORD dwMob_Struct_Offset = 0x28;
	DWORD dwMob_Next_Offset = 0x04;
	DWORD dwMob_1_Offset = 0x178;
	DWORD dwMob_2_Offset = 0x24;
	DWORD dwMob_Current_Offset = 0x14;
	DWORD dwMob_Prev_Offset = 0x0C;
	DWORD dwMob_X_Offset = 0x58;
	DWORD dwMob_Y_Offset = 0x5C;
	DWORD dwMob_Invisible_X_Offset = 0x60;
	DWORD dwMob_Invisible_Y_Offset = 0x64;

	DWORD dwPeople_Base_Pointer = 0x014037D8;
	DWORD dwPeople_Count_Offset = 0x18;

	DWORD dwMacro_Skill_Base_Pointer = 0x01403B34;

	DWORD dwChannel_Pointer = 0x0141D468;

	DWORD dwItem_Base_Pointer = 0x01408C1C;
	DWORD dwItem_Count_Offset = 0x14;
}


//==================== Private Functions ====================
namespace SCR{
	void ThreadCheck();
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
}

void SCR::OtherScript(BOOL Flag){
	if(Flag == TRUE){
		Asm::Write_code(dwBackground, "C3", 0);
		Asm::Write_code(dwCloud, "C2 08 00", 0);
		Asm::Write_code(dwHideDamage1, "C2 0C 00", 0);
		Asm::Write_code(dwHideDamage2, "C2 18 00", 0);
		Asm::Write_code(dwHideEffect1, "90 E9", 0);
		Asm::Write_code(dwHideEffect2, "90 E9", 0);
		/*
		Asm::Write_code(dwPinTyper1, "84", 0);
		Asm::Write_code(dwPinTyper2, "84", 0);
		Asm::Write_code(dwPicTyper1, "90 E9", 0);
		Asm::Write_code(dwPicTyper2, "00", 0);
		Asm::Write_Hook("call", dwPicTyper3, dwPicTyper3_CCtrlEdit, 0);
		*/
	}else{
		Asm::Write_code(dwBackground, "6A", 0);
		Asm::Write_code(dwCloud, "55 8D 6C", 0);
		Asm::Write_code(dwHideDamage1, "6A FF 68", 0);
		Asm::Write_code(dwHideDamage2, "55 8D 6C", 0);
		Asm::Write_code(dwHideEffect1, "0F 85", 0);
		Asm::Write_code(dwHideEffect2, "0F 84", 0);
		/*
		Asm::Write_code(dwPinTyper1, "85", 0);
		Asm::Write_code(dwPinTyper2, "85", 0);
		Asm::Write_code(dwPicTyper1, "90 E9", 0);
		Asm::Write_code(dwPicTyper2, "01", 0);
		Asm::Write_Hook("call", dwPicTyper3, dwPicTyper3_CCtrlEditEx, 0);
		*/
	}
}

void SCR::AllScript(BOOL Flag){
	if(Flag == TRUE){
		Asm::Write_code(dwNDFA_1, NULL, 6);
		Asm::Write_code(dwNDFA_2, NULL, 2);
		Asm::Write_code(dwNDFA_3, NULL, 6);
		Asm::Write_code(dwNDFA_4, NULL, 2);
		Asm::Write_code(dwNDFA_5, NULL, 7);

		Asm::Write_Hook("jmp", dwKamiLoot, (DWORD)KamiLoot, 1);
		Asm::Write_code(0x00CE71EC + 1, "01");//Enable Floating

		Asm::Write_code(dwNoDelay, "01", 0);
		Asm::Write_code(dwUnlimitedMP, "90 90", 0);


	}else{
		Asm::Write_Hook("jne",dwNDFA_1, dwNDFA_jne);
		Asm::Write_code(dwNDFA_2, "75 18", 0);
		Asm::Write_Hook("jne",dwNDFA_3, dwNDFA_jne);
		Asm::Write_code(dwNDFA_4, "77 7D", 0);
		Asm::Write_code(dwNDFA_5, "FF 24 8D A4 D8 CF 00");

		Asm::Write_code(dwKamiLoot, "50 55 8D 54 24 34", 0);
		Asm::Write_code(0x00CE71EC + 1, "00");//Disable Floating

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
		push eax
		push ebp
		lea edx,[esp+0x34]
		jmp dwKamiLoot_ret
	}
}


void _declspec(naked) SCR::ThreadCheck(){
	_asm{
		mov eax,0x7EFDD000//MainThread
		mov eax,[eax+0x24]
		ret
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