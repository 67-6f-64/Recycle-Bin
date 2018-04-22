#pragma once
#include<Windows.h>

bool MSCRCBypass();
bool RemoveThemidaCRCs();
bool VEHBypass();
bool StopProcessListPacket();
bool KillProcesses();
bool HSless();
bool InternalDetection();

extern unsigned char *Memory;