#include"SimpleKami.h"
#include"SimpleScript.h"
#include<math.h>

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
	DWORD dwChar_Base_Pointer = 0x0147197C;
	DWORD dwGet_Char_XY = 0x00542D90;
	int oldX = 0, oldY = 0;

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
	int Xlist[1024];
	int Ylist[1024];
	double vector[1024];
	double rX[1024], rY[1024];
	int MobCount;
}

namespace Kami{
	BOOL _stdcall KamiVac();
	BOOL _stdcall RangeCheck();
	int _stdcall GetMobXY();
}



int _stdcall Kami::GetMobXY(){
	int i = 0;
	_asm{
		xor edi,edi
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
		je GetMobXY_Prev
		call GetMobXY_True
GetMobXY_Prev:
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
		je GetMobXY_Next
		call GetMobXY_True
GetMobXY_Next:
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
		mov dword ptr [Xlist + edi*4],eax
		mov dword ptr [Ylist + edi*4],ebx
		inc edi
		ret 

GetMobXY_False:
		mov dword ptr [i],edi
	}

	return i;
}





BOOL _stdcall Kami::RangeCheck(){
	int i;

	for(i=0; i<MobCount; i++){
		rX[i] = (double)(Xlist[i] - oldX);
		rY[i] = (double)(Ylist[i] - oldY);
		vector[i] = sqrt(rX[i]*rX[i] + rY[i]*rY[i])/30.0;
	}

	for(i=0; i<MobCount; i++){
		if(vector[0] > vector[i]){
			vector[0] = vector[i];
			rX[0] = rX[i];
			rY[0] = rY[i];
		}
	}

	//if(vector[0] < 1.0){
		Xlist[0] = oldX + rX[0];
		Ylist[0] = oldY + rY[0];
		return TRUE;
	//}
	
	Xlist[0] = oldX + (int)rX[0]/vector[0];
	Ylist[0] = oldY + (int)rY[0]/vector[0];

	return FALSE;
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

		MobCount = Kami::GetMobXY();

		if(MobCount != 0){
			//Tele.X -= RangeX;
			//Tele.Y -= RangeY;
			if(RangeCheck() == TRUE){
			oldX = Xlist[0] - RangeX;
			oldY = Ylist[0] - RangeY;
				SCR::TeleportXY(oldX, oldY);
				SCR::UseFinalAttack();
			}else{

			oldX = Xlist[0] - RangeX;
			oldY = Ylist[0] - RangeY;
				SCR::TeleportXY(oldX, oldY);
			}


			return TRUE;
		}else{
			ItemCount = SCR::GetItemCount();
			if(ItemCount > 0){
				SCR::PickUp();
				return TRUE;
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
				SCR::PickUp();
				return TRUE;
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
	DWORD dwKamiHook = 0x00D21FA9;
	DWORD dwKamiHook_ret = dwKamiHook + 5;
	DWORD dwKamiHook_OriginalCall = 0x00D658D0;
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
		oldX = 0;
		oldY = 0;
		Asm::Write_Hook("jmp", dwKamiHook, (DWORD)KamiHook, 0);
		SCR::AllScript(Flag);
	}else{//Disable
		Asm::Write_Hook("call", dwKamiHook, dwKamiHook_OriginalCall, 0);
		SCR::AllScript(Flag);
	}
}
