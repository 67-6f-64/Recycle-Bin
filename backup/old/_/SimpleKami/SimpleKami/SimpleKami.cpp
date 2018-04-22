#include"SimpleKami.h"
#include"SimpleScript.h"

namespace Kami{
	//Public
	int RangeX = 0;
	int RangeY = 0;
	int CCval = 0;

	//Private
	int Action = 0;
	BOOL Breath = FALSE;
	BOOL CC = FALSE;
	int Channel;
	int CCTime = 0;
	int oldCCval = -1;
	int TeleX;
	int TeleY;
}

namespace Kami{
	BOOL _stdcall KamiVac();
	double _stdcall CoordinatesCheck(int TeleX, int TeleY);
}

void Kami::BreathTimer(){//Thread
	int i;

	while(1){
		while(Breath == FALSE){
			Sleep(100);
		}

		for(i=0; i<60; i++){
			Sleep(100);
		}

		Breath = FALSE;

	}

}


void Kami::CCTimer(){//Thread

	while(1){
		if(CCval != 0 && Action == 0){
			if(oldCCval != CCval){
				oldCCval = CCval;
				CCTime = 0;
				CC = FALSE;
			}else{
				if(CCval <= CCTime){
					CC = TRUE;
					CCTime = 0;
				}
			}
			Sleep(1000);
			CCTime++;
		}else{
			Sleep(1000);
			CC = FALSE;
			CCTime = 0;
		}
	}

}


BOOL _stdcall Kami::KamiVac(){
	int ItemCount;
	int PeopleCount;
	MobData Tele;

	if(CC == TRUE && Action == 0){
		Action = 1;
		return FALSE;
	}

	PeopleCount = SCR::GetPeopleCount();

	//Default
	if(Action == 0){

		if(PeopleCount > 0){
			Action = 1;//Try CCing
			return FALSE;
		}

		Tele = SCR::GetMobXY();

		if(Tele.Flag == TRUE){
			Tele.X -= RangeX;
			Tele.Y -= RangeY;
			TeleX = Tele.X;//fudgeX
			TeleY = Tele.Y;//fudgeY
			SCR::UseFinalAttack();
			return TRUE;
		}else{
			ItemCount = SCR::GetItemCount();
			if(ItemCount > 0){
				//SCR::PickUp();
				//return TRUE;
			}
		}
		return FALSE;

	}

	//Breath Timer Start
	if(Action == 1){
		Breath = TRUE;//Timer Start
		Action = 2;//
		return FALSE;
	}

	//Sleep And CCing
	if(Action == 2){
		if(Breath == FALSE){
			Channel = SCR::GetCurrentChannel();//Save Current Channel
			SCR::ChannelChange();
			Breath = TRUE;//This is used for channel change check
			Action = 3;
		}else{
			ItemCount = SCR::GetItemCount();
			if(ItemCount > 0 && PeopleCount == 0){
				//SCR::PickUp();
				//return TRUE;
			}
		}
		return FALSE;
	}


	if(Action == 3){
		if(Breath == TRUE){
			if(Channel != SCR::GetCurrentChannel()){
				Action = 0;//Channel Change is Successful
				Breath = FALSE;
			}
		}else{//Retry
			Action = 2;
		}
		return FALSE;
	}

	return FALSE;
}


namespace Kami{
	DWORD dwKamiHook = 0x005334C6;//fudge! SS EAX(?)
	
	DWORD dwItemXYHook = 0x005A7CC1;
	DWORD dwItemXYHook_ret = dwItemXYHook + 5;

	DWORD dwLoot_call_1 = 0x005334A0;
	DWORD dwChar_Base_Pointer = 0x013FF0CC;
	DWORD dwSpawnPointHook = 0x00CC2B33;
	DWORD dwSpawnPointHook_ret = dwSpawnPointHook + 6;

	void KamiHook();
	void ItemXYHook();
	void SpawnPointHook();
	void SetXY();
	int SpawnPoint[16] = {0};
}

void _declspec(naked) Kami::KamiHook(){
	_asm{
		pushad
		call KamiVac
		test eax,eax
		je KamiHook_Ending
		popad
		pushad
		mov ebx,[TeleX]
		mov ecx,[TeleY]
		mov [eax],ebx
		mov [eax+0x04],ecx
KamiHook_Ending:
		popad
		ret 0x0004
	}
}


void _declspec(naked) Kami::ItemXYHook(){
	_asm{
		mov dword ptr [TeleY],eax
		mov dword ptr [TeleX],ebp
		lea edx,[esp+0x34]
		push edx
		jmp dwItemXYHook_ret
	}
}


void _declspec(naked) Kami::SpawnPointHook(){
	_asm{
		lea ebx,[SpawnPoint]
		mov edi,[ebx]
		mov ebx,[ebx+0x04]
		jmp dwSpawnPointHook_ret
	}
}


void Kami::SetXY(){
	_asm{
		mov ecx,dwChar_Base_Pointer
		mov ecx,[ecx]
		test ecx,ecx
		je SetXY_False
		add ecx,4
		lea eax,[SpawnPoint]
		push eax
		call dwLoot_call_1
		jmp SetXY_Ending
SetXY_False:
		mov eax,SpawnPoint
		mov dword ptr [eax],0
		mov dword ptr [eax+0x04],0
SetXY_Ending:
	}
}


void Kami::WRYYY(BOOL Flag){
	if(Flag == TRUE){//Enable
		SetXY();
		Asm::Write_Hook("jmp", dwKamiHook, (DWORD)KamiHook);
		Asm::Write_Hook("jmp", dwSpawnPointHook, (DWORD)SpawnPointHook, 1);
		//Asm::Write_Hook("jmp", dwItemXYHook, (DWORD)ItemXYHook);
		SCR::AllScript(Flag);
	}else{//Disable
		Asm::Write_code(dwKamiHook, "C2 04 00 CC CC");
		Asm::Write_code(dwSpawnPointHook, "8B 78 0C 8B 58 10");
		//Asm::Write_code(dwItemXYHook, "8D 54 24 34 52");
		SCR::AllScript(Flag);
	}
}
