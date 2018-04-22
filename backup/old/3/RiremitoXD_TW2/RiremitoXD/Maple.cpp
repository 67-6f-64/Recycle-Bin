#include"AirBypass.h"
DWORD Memory, Memory_Start, Memory_End;
DWORD MSCRC1, MSCRC1_Ret;

void _declspec(naked) MSCRC1_Hook(){
	_asm{
		xor eax,eax
		add eax,edx
		mov edx,[ebp+0x18]
		sub eax,0x08
		mov eax,[edx]
		shr eax,0x08
		xor ecx,ecx
		mov ecx,eax
		shl ecx,0x08
		mov ecx,[ebp+0x08]
		add ecx,[ebp-0x38]
		xor edx,edx
		mov ebx,[ebp+0x08]
		cmp ecx,[Memory_Start]
		jb Ending
		cmp ecx,[Memory_End]
		ja Ending
		sub ecx,[Memory_Start]
		add ecx,[Memory]
Ending:
		jmp dword ptr [MSCRC1_Ret]
	}
}


char MapleID[128];
void _stdcall SetMapleID(char s[]){
	int i;
	ZeroMemory(MapleID, 128);
	for(i=0; s[i]; i++){
		MapleID[i] = s[i];
	}
}

DWORD Login_Ret;
DWORD Login_Mov;
void _declspec(naked) Login_Hook(){
	_asm{
		mov eax,[esp+0x08]//MapleID
		pushad
		push eax
		call SetMapleID
		popad
		push 0x50
		mov eax,[Login_Mov]
		jmp dword ptr [Login_Ret]
	}
}

void _stdcall HWID_Randomizer(BYTE HWID[]){
	int i, MapleID_Size;
	DWORD seed = 0, seed2 = 0;

	for(i=0; MapleID[i]; i++){
		seed += MapleID[i];
		seed2 ^= MapleID[i];
	}
	MapleID_Size = i;


	for(i=0; i<0x10; i++){
		HWID[i] ^= MapleID_Size;
		HWID[i] ^= seed;
	}
	for(i=0; i<0x10; i++){
		HWID[i] ^= MapleID[i%MapleID_Size];
	}
	*(DWORD *)&HWID[0x0C] ^= seed2;
	*(DWORD *)&HWID[0x08] ^= *(DWORD *)&HWID[0x0C];
	*(DWORD *)&HWID[0x04] ^= *(DWORD *)&HWID[0x08];
	*(DWORD *)&HWID[0x00] ^= *(DWORD *)&HWID[0x04];
}

DWORD HWID_Ret;
void _declspec(naked) HWID_Hook(){
	_asm{
		push ecx
		call Create_HWID
		pop ecx
		add ecx,0x14//HIWD PLZ
		push ecx
		call HWID_Randomizer
		ret
Create_HWID:
		sub esp,0x00000628
		jmp dword ptr [HWID_Ret]
	}
}

DWORD HS_Init_Ret;
void _declspec(naked) HS_ManualInit(){
	_asm{
		//mov dword ptr [esp+0x14],0x024A61AE
		push ebx
		mov eax,[esp+0x18]
		mov dword ptr [HackShield_Flag],eax//original code!
		mov ebx,0x08000000
		not ebx
		and eax,ebx
		mov ebx,0x00800000
		not ebx
		and eax,ebx
		mov dword ptr [esp+0x18],eax
		pop ebx
		push ebp
		mov ebp,esp
		sub esp,0x2B0
		jmp dword ptr [HS_Init_Ret]
	}
}

AirMemory AM;
// v178.0 ~
void Bypass(){
	AM.Init();
	AM.CreateMemoryDump();
	AM.GetDumpInfo(&Memory_Start, &Memory_End, &Memory);
	//return;
	//MSCRC1 = AM.AobScan("3B 8D 70 FF FF FF 0F 85 ?? ?? ?? ?? 6A 00 E9") + 6;//removed
	//MSCRC1_Ret = AM.AobScan("8A 11 80 C2 01 8B 4D 18");//removed

	DWORD DR_Check = AM.AobScan("55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 E9");//v178
	DWORD ASPLunchr = AM.AobScan("55 8B EC 83 EC 0C 56 57 8B F1 E9");//v178
	DWORD mkd25tray = AM.AobScan("55 8B EC 83 EC 0C 56 57 8B F1 E9", 2);//v178

	AM.WriteHook(MSCRC1, JNE, MSCRC1_Hook);
	AM.MemoryWriter(DR_Check, "31 C0 C3");
	AM.MemoryWriter(ASPLunchr, "31 C0 C3");
	AM.MemoryWriter(mkd25tray, "31 C0 C3");

	DWORD HSUpdate = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 ?? ?? ?? ?? 53 56 57 89 65 E8 C7 45 BC 00 00 00 00 C7 45 E4 00 00 00 00 C7 85 A8 FE FF FF 00 00 00 00 C7 45 D4 00 00 00 00 33 C0 89 45 D8 89 45 DC 89 45 E0 C7 85 18 FE FF FF 00 00 00 00 B9 10 00 00 00 33 C0 8D BD 1C FE FF FF F3 AB");
	AM.MemoryWriter(HSUpdate, "31 C0 C2 10 00");

	DWORD MSCRC23 = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D ?? ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? 83 A5 ?? ?? ?? ?? 00 6A 01");
	AM.MemoryWriter(MSCRC23, "31 C0 C3");

	DWORD HS_Init = AM.AobScan("55 8B EC 81 EC B0 02 00 00 57 C7 ?? ?? 00 00 00 00 C7 ?? ?? 00 00 00 00 C6 ?? ?? ?? ?? ?? 00");
	
	if(HS_Init){
		AM.WriteHook(HS_Init, JMP, HS_ManualInit, &HS_Init_Ret, 4);
	}

	DWORD HWID = AM.AobScan("81 EC 28 06 00 00 33 C0 56 89 44 24 ?? 8B F1 89 44 24  ?? 8D ?? ?? ?? 89 44 24 ?? 51");
	if(HWID){
		AM.WriteHook(HWID, JMP, HWID_Hook, &HWID_Ret, 1);
	}

	DWORD Login = AM.AobScan("6A 50 B8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B F1 33 FF 47 33 DB 89 7D ?? 39 9E ?? ?? ?? ?? 74");//v178
	if(Login){
		Login_Mov = *(DWORD *)(Login + 3);
		AM.WriteHook(Login, JMP, Login_Hook, &Login_Ret, 2);
	}
}

/* ~v177.2
void Bypass(){
	AM.Init();
	AM.CreateMemoryDump();
	AM.GetDumpInfo(&Memory_Start, &Memory_End, &Memory);
	//return;
	MSCRC1 = AM.AobScan("3B 8D 70 FF FF FF 0F 85 ?? ?? ?? ?? 6A 00 E9") + 6;
	MSCRC1_Ret = AM.AobScan("8A 11 80 C2 01 8B 4D 18");

	DWORD DR_Check = AM.AobScan("55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9");
	DWORD ASPLunchr = AM.AobScan("55 8B EC 83 EC 0C 56 57 6A 00 8B F1 E9");
	DWORD mkd25tray = AM.AobScan("55 8B EC 83 EC 0C 56 57 6A 00 8B F1 E9", 2);

	AM.WriteHook(MSCRC1, JNE, MSCRC1_Hook);
	AM.MemoryWriter(DR_Check, "31 C0 C3");
	AM.MemoryWriter(ASPLunchr, "31 C0 C3");
	AM.MemoryWriter(mkd25tray, "31 C0 C3");

	DWORD HSUpdate = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 ?? ?? ?? ?? 53 56 57 89 65 E8 C7 45 BC 00 00 00 00 C7 45 E4 00 00 00 00 C7 85 A8 FE FF FF 00 00 00 00 C7 45 D4 00 00 00 00 33 C0 89 45 D8 89 45 DC 89 45 E0 C7 85 18 FE FF FF 00 00 00 00 B9 10 00 00 00 33 C0 8D BD 1C FE FF FF F3 AB");
	AM.MemoryWriter(HSUpdate, "31 C0 C2 10 00");

	DWORD MSCRC23 = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D ?? ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? 83 A5 ?? ?? ?? ?? 00 6A 01");
	AM.MemoryWriter(MSCRC23, "31 C0 C3");

	DWORD HS_Init = AM.AobScan("55 8B EC 81 EC B0 02 00 00 57 C7 ?? ?? 00 00 00 00 C7 ?? ?? 00 00 00 00 C6 ?? ?? ?? ?? ?? 00");
	
	if(HS_Init){
		AM.WriteHook(HS_Init, JMP, HS_ManualInit, &HS_Init_Ret, 4);
	}

	DWORD HWID = AM.AobScan("81 EC 28 06 00 00 33 C0 56 89 44 24 ?? 8B F1 89 44 24  ?? 8D ?? ?? ?? 89 44 24 ?? 51");
	if(HWID){
		AM.WriteHook(HWID, JMP, HWID_Hook, &HWID_Ret, 1);
	}

	DWORD Login = AM.AobScan("6A 50 B8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B F1 33 FF 33 DB 47 53 89 7D ?? E9");
	if(Login){
		Login_Mov = *(DWORD *)(Login + 3);
		AM.WriteHook(Login, JMP, Login_Hook, &Login_Ret, 2);
	}
}
*/
