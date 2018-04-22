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
	int oldTeleX = 0;
	int oldTeleY = 0;
}

namespace Kami{
	void _stdcall KamiVac();
	BOOL _stdcall CoordinatesCheck(int TeleX, int TeleY);
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


BOOL _stdcall Kami::CoordinatesCheck(int TeleX, int TeleY){

	if(oldTeleX - TeleX < 30 || TeleX - oldTeleX < 30){
		if(oldTeleY - TeleY < 30 || TeleY - oldTeleY < 30){
		return FALSE;
		}
	}

		oldTeleX = TeleX;
		oldTeleY = TeleY;

	return TRUE;
}


void _stdcall Kami::KamiVac(){
	int ItemCount;
	int PeopleCount;
	MobData Tele;


	if(CC == TRUE && Action == 0){
		Action = 1;
		return;
	}

	PeopleCount = SCR::GetPeopleCount();

	//Default
	if(Action == 0){

		if(PeopleCount > 0){
			Action = 1;//Try CCing
			return;
		}

		Tele = SCR::GetMobXY();

		if(Tele.Flag == TRUE){
			Tele.X -= RangeX;
			Tele.Y -= RangeY;

			//DC Bypass
			//if(CoordinatesCheck(Tele.X, Tele.Y) == TRUE){
				SCR::TeleportXY(Tele.X, Tele.Y);
			//}

			//SCR::UseFinalAttack();

		}else{
			ItemCount = SCR::GetItemCount();
			if(ItemCount > 0){
				SCR::PickUp();
			}
		}

		return;
	}

	//Breath Timer Start
	if(Action == 1){
		Breath = TRUE;//Timer Start
		Action = 2;//
		return;
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
				SCR::PickUp();
			}
		}
		return;
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
		return;
	}

	return;
}


namespace Kami{
	DWORD dwKamiHook = 0x00CE70D9;
	DWORD dwKamiHook_ret = dwKamiHook + 5;
	DWORD dwKamiHook_OriginalCall = 0x00D26A40;
	/*
	DWORD dwKamiHook = 0x00CE7107;
	DWORD dwKamiHook_ret = dwKamiHook + 5;
	DWORD dwKamiHook_OriginalCall = 0x0077A080;
	*/
	void KamiHook();
}

void _declspec(naked) Kami::KamiHook(){
	_asm{
		call dwKamiHook_OriginalCall
		pushad
		call KamiVac
		popad
		jmp dwKamiHook_ret
	}
}



void Kami::WRYYY(BOOL Flag){
	if(Flag == TRUE){//Enable
		oldTeleX = 0;
		oldTeleY = 0;
		Asm::Write_Hook("jmp", dwKamiHook, (DWORD)KamiHook, 0);
		SCR::AllScript(Flag);
	}else{//Disable
		Asm::Write_Hook("call", dwKamiHook, dwKamiHook_OriginalCall, 0);
		SCR::AllScript(Flag);
	}
}
