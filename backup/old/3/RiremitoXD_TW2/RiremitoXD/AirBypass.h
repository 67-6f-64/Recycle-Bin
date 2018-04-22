#pragma once
#include<Windows.h>
#include"AirMemory.h"

void AirBypass();

BOOL WINAPI Inject(DWORD processID, wchar_t * sourceDLL);

void FullAccess(DWORD Address, DWORD Size);
void Bypass();
void HackShieldBypass();
extern DWORD Memory, Memory_Start, Memory_End;

extern DWORD HackShield_Flag;