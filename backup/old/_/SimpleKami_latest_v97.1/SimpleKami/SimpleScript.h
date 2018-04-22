#ifndef __SimpleScript_H__
#define __SimpleScript_H__
#include<stdio.h>
#include"lib/Asm.h"

typedef struct{
	int X;
	int Y;
	BOOL Flag;
}MobData;


namespace SCR{
	void _stdcall TeleportXY(int TeleX, int TeleY);
	MobData _stdcall GetMobXY();
	void _stdcall UseFinalAttack();
	void _stdcall ChannelChange();
	int _stdcall GetCurrentChannel();
	int _stdcall GetPeopleCount();
	void _stdcall PickUp();
	int _stdcall GetItemCount();
	void AllScript(BOOL Flag);
	void OtherScript(BOOL Flag);
	void AlwaysEnable();

}

#endif