#ifndef __HOOK_H__
#define __HOOK_H__

#include"AirMemory.h"

void hook();
void FullAccess(DWORD Address, DWORD Size);
void write_jmp(DWORD Prev, DWORD Next);
void write_call(DWORD Prev, DWORD Next);

void GetItem();

extern DWORD Action;

typedef struct {
	DWORD ItemID;
	WORD Count;
}ConsumeInventory;

extern ConsumeInventory IV[100];

void Sell(int i);

extern int currenti;

void MapTeleport(DWORD MapID);

void AllDrop();

void set_hook(DWORD Function, DWORD Size, LPVOID HookFunction, LPVOID NewFunction);

#endif