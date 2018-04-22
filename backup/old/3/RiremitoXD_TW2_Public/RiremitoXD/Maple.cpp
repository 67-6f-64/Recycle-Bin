#include"AirBypass.h"
DWORD Memory, Memory_Start, Memory_End;
AirMemory AM;
// v178.0 ~
void Bypass(){
	AM.Init();
	AM.CreateMemoryDump();
	AM.GetDumpInfo(&Memory_Start, &Memory_End, &Memory);

	DWORD DR_Check = AM.AobScan("55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 E9");//v178
	DWORD ASPLunchr = AM.AobScan("55 8B EC 83 EC 0C 56 57 8B F1 E9");//v178
	DWORD mkd25tray = AM.AobScan("55 8B EC 83 EC 0C 56 57 8B F1 E9", 2);//v178

	AM.MemoryWriter(DR_Check, "31 C0 C3");
	AM.MemoryWriter(ASPLunchr, "31 C0 C3");
	AM.MemoryWriter(mkd25tray, "31 C0 C3");

	DWORD HSUpdate = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 ?? ?? ?? ?? 53 56 57 89 65 E8 C7 45 BC 00 00 00 00 C7 45 E4 00 00 00 00 C7 85 A8 FE FF FF 00 00 00 00 C7 45 D4 00 00 00 00 33 C0 89 45 D8 89 45 DC 89 45 E0 C7 85 18 FE FF FF 00 00 00 00 B9 10 00 00 00 33 C0 8D BD 1C FE FF FF F3 AB");
	AM.MemoryWriter(HSUpdate, "31 C0 C2 10 00");

	DWORD MSCRC23 = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D ?? ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? 83 A5 ?? ?? ?? ?? 00 6A 01");
	AM.MemoryWriter(MSCRC23, "31 C0 C3");
}