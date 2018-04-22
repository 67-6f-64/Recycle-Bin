#ifndef __AIRPL_H__
#define __AIRPL_H__

#include"AirWindow.h"
#pragma comment(lib, "AirWindow.lib")
#include"AirMemory.h"
#pragma comment(lib, "AirMemory.lib")

void AirPL_Run(HINSTANCE hInstance);

void _stdcall SendLog(BYTE Packet[], DWORD dwSize, DWORD dwReturn);
void _stdcall RecvLog(BYTE Packet[], DWORD dwSize, DWORD dwReturn);

void WriteHooks();

#endif