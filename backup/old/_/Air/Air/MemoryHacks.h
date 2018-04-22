#pragma once
#include<Windows.h>

bool MSCRCBypass();
bool RemoveThemidaCRCs();
bool VEHBypass();
bool StopProcessListPacket();
bool KillProcesses();
bool HSless();

extern unsigned char *Memory;