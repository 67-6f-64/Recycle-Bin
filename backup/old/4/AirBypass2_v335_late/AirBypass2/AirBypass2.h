#ifndef __AIRBYPASS2_H__
#define __AIRBYPASS2_H__

#include"AirWindow.h"
#pragma comment(lib, "AirWindow.lib")
#include"AirMemory.h"
#pragma comment(lib, "AirMemory.lib")

#include<stdio.h>

#define EDIT_LOG 2050
#define CHECK_AC 3050
#define CHECK_HL 3060

extern AirWindow BW;
extern AirMemory AM, HM;

extern DWORD Memory, Memory_Start, Memory_End;

void AirBypass2_Run(HINSTANCE hinstDLL);

void Bypass();
void HackShieldBypass();

#endif