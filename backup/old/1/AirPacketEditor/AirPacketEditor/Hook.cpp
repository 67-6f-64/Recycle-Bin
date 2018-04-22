#include<Windows.h>
#include"Hook.h"
#include"Packet.h"
#include"gui.h"
#include"MapTeleport.h"

//
DWORD Ptr_ThreadID;// = 0x01D8FF4C;
DWORD Addr_SendPacket;
DWORD Addr_FakeRet;
//


void (__fastcall *_SendPacketProxy)(DWORD ecx, SPacket *p);
void (__fastcall *_SendPacket)(DWORD ecx, DWORD edx, SPacket *p);
void (__fastcall *_ProcessPacket)(DWORD ecx, DWORD edx, RPacket *p);
DWORD (__fastcall *_GetData4)(DWORD ecx);

void (__fastcall *_InitPacket)(SPacket *p, DWORD edx, WORD w);
void (__fastcall *_Encode1)(SPacket *p, DWORD edx, BYTE b);
void (__fastcall *_Encode2)(SPacket *p, DWORD edx, WORD w);
void (__fastcall *_Encode4)(SPacket *p, DWORD edx, DWORD dw);
void (__fastcall *_Encode8)(SPacket *p, DWORD edx, ULONGLONG ull);
void (__fastcall *_EncodeStr)(SPacket *p, DWORD edx, char *s);
void (__fastcall *_EncodeBuffer)(SPacket *p, DWORD edx, BYTE *b, int Length);

BYTE (__fastcall *_Decode1)(RPacket *p, DWORD edx);
WORD (__fastcall *_Decode2)(RPacket *p, DWORD edx);
DWORD (__fastcall *_Decode4)(RPacket *p, DWORD edx);
ULONGLONG (__fastcall *_Decode8)(RPacket *p, DWORD edx);
char** (__fastcall *_DecodeStr)(RPacket *p, DWORD edx, LPVOID lpv1);
void (__fastcall *_DecodeBuffer)(RPacket *p, DWORD edx, BYTE *b, int Length);

//format start
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)


SPInfo *spi = NULL;
DWORD firstthread = 0;

void __fastcall InitPacket_Hook(SPacket *p, DWORD edx, WORD w){
	_InitPacket(p, edx, w);
	//allocate memory
	//spi = new SPInfo;
	//spi->s = new char[65535];
	//ZeroMemory(spi->s, 65535);
	Air::AirPrint(spi->s, "%w", w);
	spi->s_Length = 4;
	spi->EncodedLength = 2;
	if(!firstthread){
		firstthread = GetCurrentThreadId();
	}
}

void __fastcall Encode1_Hook(SPacket *p, DWORD edx, BYTE b){
	_Encode1(p, edx, b);
	if(spi && spi->s){
		Air::AirPrint(&spi->s[spi->s_Length], " %b", b);
		spi->s_Length += 1 + 2;
		spi->EncodedLength += 1;
	}
}

void __fastcall Encode2_Hook(SPacket *p, DWORD edx, WORD w){
	_Encode2(p, edx, w);
	if(spi && spi->s && p->Length != 2){
		Air::AirPrint(&spi->s[spi->s_Length], " %w", w);
		spi->s_Length += 1 + 4;
		spi->EncodedLength += 2;
	}
}

void __fastcall Encode4_Hook(SPacket *p, DWORD edx, DWORD dw){
	_Encode4(p, edx, dw);
	if(spi && spi->s){
		Air::AirPrint(&spi->s[spi->s_Length], " %d", dw);
		spi->s_Length += 1 + 8;
		spi->EncodedLength += 4;
	}
}

void __fastcall Encode8_Hook(SPacket *p, DWORD edx, ULONGLONG ull){
	_Encode8(p, edx, ull);
	if(spi && spi->s){
		Air::AirPrint(&spi->s[spi->s_Length], " %q", (DWORD)ull, (DWORD)(ull >> 32));
		spi->s_Length += 1 + 16;
		spi->EncodedLength += 8;
	}
}


void __fastcall EncodeStr_Hook(SPacket *p, DWORD edx, char *s){
	_EncodeStr(p, edx, s);
	if(spi && spi->s){
		Air::AirPrint(&spi->s[spi->s_Length], " %xs", s);
		spi->s_Length += 1 + strlen(s) + 2;
		spi->EncodedLength += 2 + strlen(s);
	}
}


void __fastcall EncodeBuffer_Hook(SPacket *p, DWORD edx, BYTE *b, int Length){
	_EncodeBuffer(p, edx, b, Length);
	if(spi && spi->s){
		Air::AirPrint(&spi->s[spi->s_Length], " %a", b, Length);
		spi->s_Length += 1 + Length*2;
		spi->EncodedLength += Length;
	}
}

//
RFormat *RF = NULL;
//
BYTE __fastcall Decode1_Hook(RPacket *p, DWORD edx){
	BYTE b = _Decode1(p, edx);

	if(RF && !RF->FunctionAddress){
		if(RF->DecodeCount == 1){
			RF->FunctionAddress = (DWORD)_ReturnAddress();
		}
		RF->DecodeCount++;
	}

	if(RF && RF->String){
		Air::AirPrint(&RF->String[RF->lpv2], " %b", b);
		RF->lpv2 += 1 + 2;
	}
	return b;
}

WORD __fastcall Decode2_Hook(RPacket *p, DWORD edx){
	WORD w = _Decode2(p, edx);

	if(RF && !RF->FunctionAddress){
		if(RF->DecodeCount == 1){
			RF->FunctionAddress = (DWORD)_ReturnAddress();
		}
		RF->DecodeCount++;
	}

	if(RF && RF->String){
		if(p->DecodedLength == 6){//decode header
			Air::AirPrint(&RF->String[RF->lpv2], "%w", w);
			RF->lpv2 = 4;
		}
		else{
			Air::AirPrint(&RF->String[RF->lpv2], " %w", w);
			RF->lpv2 += 1 + 4;
		}
	}
	return w;
}

DWORD __fastcall Decode4_Hook(RPacket *p, DWORD edx){
	DWORD dw = _Decode4(p, edx);

	if(RF && !RF->FunctionAddress){
		if(RF->DecodeCount == 1){
			RF->FunctionAddress = (DWORD)_ReturnAddress();
		}
		RF->DecodeCount++;
	}

	if(RF && RF->String){
		Air::AirPrint(&RF->String[RF->lpv2], " %d", dw);
		RF->lpv2 += 1 + 8;
	}
	return dw;
}

ULONGLONG __fastcall Decode8_Hook(RPacket *p, DWORD edx){
	ULONGLONG ull = _Decode8(p, edx);

	if(RF && !RF->FunctionAddress){
		if(RF->DecodeCount == 1){
			RF->FunctionAddress = (DWORD)_ReturnAddress();
		}
		RF->DecodeCount++;
	}

	if(RF && RF->String && p->DataLength <= 4096){
		Air::AirPrint(&RF->String[RF->lpv2], " %q", (DWORD)ull, (DWORD)(ull >> 32));
		RF->lpv2 += 1 + 16;
	}
	return ull;
}

char** __fastcall DecodeStr_Hook(RPacket *p, DWORD edx, LPVOID lpv1){
	char **s = _DecodeStr(p, edx, lpv1);

	if(RF && !RF->FunctionAddress){
		if(RF->DecodeCount == 1){
			RF->FunctionAddress = (DWORD)_ReturnAddress();
		}
		RF->DecodeCount++;
	}

	if(RF && RF->String){
		Air::AirPrint(&RF->String[RF->lpv2], " %xs", *s);
		RF->lpv2 += 1 + strlen(*s) + 2;
	}
	return s;
}

void __declspec(noinline) __fastcall DecodeBuffer_Hook(RPacket *p, DWORD edx, BYTE *b, int Length){
	_DecodeBuffer(p, edx, b, Length);

	if(RF && !RF->FunctionAddress){
		if(RF->DecodeCount == 1){
			RF->FunctionAddress = (DWORD)_ReturnAddress();
		}
		RF->DecodeCount++;
	}

	if(RF && RF->String){
		Air::AirPrint(&RF->String[RF->lpv2], " %a", &b[0], Length);
		RF->lpv2 += 1 + Length*2;
	}
}

//format end

void _declspec(naked) __fastcall __SendPacket(DWORD ecx, DWORD edx, SPacket *p){
	_asm{
		push PUSHBACK
		push p
		push [Addr_FakeRet]//fake return
		jmp dword ptr [_SendPacket]
PUSHBACK:
		ret 0x0004
	}
}

void __fastcall SendPacket_Hook(DWORD ecx, DWORD edx, SPacket *p){
	if(firstthread == GetCurrentThreadId()){
		gui_AddSendPacket(p, (DWORD)_ReturnAddress());
		spi->EncodedLength = 0;
		spi->s_Length = 0;
		//deallocate
		/*
		if(spi){
			if(spi->s){
				delete[] spi->s;
			}
			spi->s = NULL;
			delete spi;
		}
		spi = NULL;
		*/
	}
	else{
		gui_AddSendPacket2(p, (DWORD)_ReturnAddress());
	}
	return __SendPacket(ecx, edx, p);
}

void __fastcall ProcessPacket_Hook(DWORD ecx, DWORD edx, RPacket *p){
	//RF = new RFormat;
	//RF->String = new char[65535];
	//ZeroMemory(RF->String, 65535);
	RF->lpv2 = 0;
	RF->FunctionAddress = 0;
	RF->DecodeCount = 0;
	_ProcessPacket(ecx, edx, p);
	if(p->Length >= 6){
		gui_AddProcessPacket(p);
	}
	RF->lpv2 = 0;
	RF->FunctionAddress = 0;
	RF->DecodeCount = 0;
	//delete[] RF->String;
	//RF->String = NULL;
	//delete RF;
	//RF = NULL;
}

DWORD __fastcall GetData4_Hook(DWORD ecx){
	if(ecx == Ptr_ThreadID){
		return 0;
	}
	return _GetData4(ecx);
}

//========================================
void FullAccess(DWORD Address, DWORD Size){
	DWORD old;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &old);
}

void write_jmp(DWORD Prev, DWORD Next){
	FullAccess(Prev, 5);
	*(BYTE *)(Prev) = 0xE9;
	*(DWORD *)(Prev + 1) = Next - Prev - 5;
}

void write_call(DWORD Prev, DWORD Next){
	FullAccess(Prev, 5);
	*(BYTE *)(Prev) = 0xE8;
	*(DWORD *)(Prev + 1) = Next - Prev - 5;
}

void write_byte(DWORD Address, BYTE b){
	FullAccess(Address, 1);
	*(BYTE *)Address = b;
}

void set_hook(DWORD Function, DWORD Size, LPVOID HookFunction, LPVOID NewFunction){
	DWORD OverWritten = (DWORD)new BYTE[Size + 5];
	FullAccess(OverWritten, Size + 5);//ŽÀs‰Â”\‚É•ÏX
	RtlCopyMemory((LPVOID)OverWritten, (LPVOID)Function, Size);//Á‚¦‚é•”•ª‚ð•œŒ³
	if(*(BYTE *)Function == 0xE9){
		DWORD Address = *(DWORD *)(Function + 1) + Function + 5;
		*(DWORD *)(OverWritten + 1) = Address - OverWritten - 5;
	}


	write_jmp(OverWritten + Size, Function + Size);
	*(DWORD *)NewFunction = (DWORD)OverWritten;
	FullAccess(Function, 5);
	write_jmp(Function, (DWORD)HookFunction);
	if(Size > 5){
		for(int i=0; i<Size-5; i++){
			*(BYTE *)(Function + 5 + i) = 0x90;
		}
	}
}

//=========================================================
/*
#define POINTER_THREAD_ID 0x01D8FF2C
#define ADDRESS_GETDATA4 0x004A14B0
#define ADDRESS_GETDATA4_PROXY 0x0055E080
#define ADDRESS_NOP8 0x016AEE07
#define ADDRESS_SENDPACKET 0x0061B070
#define ADDRESS_PROCESSPACKET 0x0061BE20
#define ADDRESS_ENCODE1 0x004AAB30
#define ADDRESS_ENCODE2 0x004AAB80
#define ADDRESS_ENCODE4 0x004A1C70
#define ADDRESS_ENCODE8 0x0053B0F0
#define ADDRESS_ENCODESTR 0x0053BF70
#define ADDRESS_ENCODEBUFFER 0x0057D0A0
#define ADDRESS_INITPACKET 0x00B6BB70
#define ADDRESS_DECODE1 0x00415420
#define ADDRESS_DECODE2 0x004154D0
#define ADDRESS_DECODE4 0x00415580
#define ADDRESS_DECODE8 0x00538ED0
#define ADDRESS_DECODESTR 0x004CABF0
#define ADDRESS_DECODEBUFFER 0x00415630
*/
#include"AirMemory.h"

#include<intrin.h>
#pragma intrinsic(_ReturnAddress)
#include<stdio.h>

void AobError(char name[]){
	char asdf[256];
	sprintf(asdf, "Aob is broken(%s:%p)", name, _ReturnAddress());
	MessageBoxA(NULL, asdf, "Error", NULL);
}

DWORD ProxyScan(DWORD Address){
	DWORD i;
	for(i=0x00501000; i<0x02000000; i++){
		if(*(BYTE *)i == 0xE9){
			if(*(DWORD *)(i + 1) + i + 5 == Address){
				return i;
			}
		}
	}
	return 0;
}


void (__fastcall *_fSendPacket)(DWORD ecx, SPacket *p);
DWORD CClientSocket;

void Hook(){
	AirMemory AM;
	DWORD Address;
	AM.Init();
	
	/*
	Address = AM.AobScan("8B 0D ?? ?? ?? ?? BB 01 00 00 00 89 5C 24 ?? E8");//u
	if(!Address) AobError("ThreadIDCheck");
	Ptr_ThreadID = *(DWORD *)(Address + 2) + 8;
	*/

	Address = AM.AobScan("55 8B EC 6A FF 68  ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 85 ?? ?? ?? ?? 53 56 57 50 8D 45 ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? E9");//u

	if(!Address) AobError("SendPacket");
	set_hook(Address, 5, SendPacket_Hook, &_SendPacket);
	Addr_SendPacket = Address;


	Address = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3  00 00 00 00 8B D9 83 3D ?? ?? ?? ?? 00 0F 84 ?? ?? ?? ?? E9");//u
		if(!Address) AobError("ProcessPacket");
	set_hook(Address, 7, ProcessPacket_Hook, &_ProcessPacket);

	Address = AM.AobScan("83 EC 08 53 56 57 8B F9 8B 57 ?? 8B 02 89 44 24 ?? 8A 42 ?? 84 C0 75", 2);
	if(!Address) AobError("Encrypt");
	
	_GetData4 = (DWORD (__fastcall *)(DWORD))Address;
	Address = ProxyScan(Address);
	if(!Address) AobError("ProxyAddress");
	write_jmp(Address, (DWORD)GetData4_Hook);

	Address = AM.AobScan("52 E8 ?? ?? ?? ?? 90 C3", 0, 0x00500000, 0x02000000);
	if(!Address){
		Address = AM.AobScan("CC CC CC CC CC CC CC CC", 0, 0x00500000, 0x02000000);
		if(!Address) AobError("FreeSpace");
		write_byte(Address, 0x52);//push edx
		write_call(Address + 1, Addr_SendPacket);
		write_byte(Address + 1 + 5, 0x90);//nop
		write_byte(Address + 1 + 5 + 1, 0xC3);//ret
	}
	_fSendPacket = (void (__fastcall *)(DWORD, SPacket*))Address;//new sendpacket
	Addr_FakeRet = Address + 6;

	Address = AM.AobScan("8B 0D ?? ?? ?? ?? 85 C9 74 0A 8D 54 24 ?? 52 E8");//u
	if(!Address) AobError("CClientSocket");
	CClientSocket = *(DWORD *)(Address + 2);

	Address = AM.AobScan("8B 44 24 04 6A 00 6A 00 50 E8 ?? ?? ?? ?? C2 04 00");
	if(!Address) AobError("Init");
	
	set_hook(Address, 6, InitPacket_Hook, &_InitPacket);

	Address = AM.AobScan("56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 41 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 4C 24 0C 51 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 56 08 8B 07 8A 4C 24 0C 88 0C 02 FF 46 08 5F 5E C2 04 00");
	if(!Address) AobError("Encode1");
	set_hook(Address, 6, Encode1_Hook, &_Encode1);

	Address = AM.AobScan("56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 83 C1 02 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 4C 24 0C 51 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 56 08 8B 07 66 8B 4C 24 0C 66 89 0C 02 83 46 08 02 5F 5E C2 04 00");
	if(!Address) AobError("Encode2");
	set_hook(Address, 6, Encode2_Hook, &_Encode2);

	Address = AM.AobScan("56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 83 C1 04 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 4C 24 0C 51 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 56 08 8B 07 8B 4C 24 0C 89 0C 02 83 46 08 04 5F 5E C2 04 00");
	if(!Address) AobError("Encode4");
	set_hook(Address, 6, Encode4_Hook, &_Encode4);

	Address = AM.AobScan("56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 83 C1 08 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 4C 24 0C 51 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 46 08 8B 0F 8B 54 24 0C 89 14 08 8B 54 24 10 89 54 08 04 83 46 08 08 5F 5E C2 08 00");
	if(!Address) AobError("Encode8");
	set_hook(Address, 6, Encode8_Hook, &_Encode8);

	Address = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 51 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 8B 44 24 ?? C7 44 24 ?? 00 00 00 00 85 C0 74");
	if(!Address) AobError("EncodeStr");
	set_hook(Address, 7, EncodeStr_Hook, &_EncodeStr);

	Address = AM.AobScan("53 56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 8B 5C 24 14 03 CB 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 54 24 14 52 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 4E 08 8B 44 24 10 03 0F 53 50 51 E8 ?? ?? ?? ?? 01 5E 08 83 C4 0C 5F 5E 5B C2 08 00");
	if(!Address) AobError("EncodeBuffer");
	set_hook(Address, 7, EncodeBuffer_Hook, &_EncodeBuffer);
	

	Address = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 89 4D ?? 8B 51 ?? 8B 41 ?? 8B 71 ?? 2B C2 C7 45 ?? 00 00 00 00 83 F8 01");
	if(!Address) AobError("Decode1");
	set_hook(Address, 5, Decode1_Hook, &_Decode1);

	Address = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 89 4D ?? 8B 51 ?? 8B 41 ?? 8B 71 ?? 2B C2 C7 45 ?? 00 00 00 00 83 F8 02");
	if(!Address) AobError("Decode2");
	set_hook(Address, 5, Decode2_Hook, &_Decode2);

	Address = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 89 4D ?? 8B 51 ?? 8B 41 ?? 8B 71 ?? 2B C2 C7 45 ?? 00 00 00 00 83 F8 04");
	if(!Address) AobError("Decode4");
	set_hook(Address, 5, Decode4_Hook, &_Decode4);

	Address = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 89 4D ?? 8B 71 ?? 8B 41 ?? 8B 51 ?? 2B C6 C7 45 ?? 00 00 00 00 83 F8 08");
	if(!Address) AobError("Decode8");
	set_hook(Address, 5, Decode8_Hook, &_Decode8);

	Address = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 8B F1 89 75 ?? C7 45 ?? 00 00 00 00 8B 7D ?? B8 01 00 00 00");
	if(!Address) AobError("DecodeStr");
	set_hook(Address, 5, DecodeStr_Hook, &_DecodeStr);

	Address = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 8B F1 89 75 ?? 8B 4E ?? 8B 46 ?? 8B 56 ?? 8B 7D ?? 2B C1 03 CA C7 45 ?? 00 00 00 00 3B C7");
	if(!Address) AobError("DecodeBuffer");
	set_hook(Address, 5, DecodeBuffer_Hook, &_DecodeBuffer);
	

	//Hook_MT();

	RF = (RFormat *)malloc(sizeof(RFormat));
	RF->String = (char *)malloc(65535);
	spi = (SPInfo *)malloc(sizeof(SPInfo));
	spi->s = (char *)malloc(65535);
}