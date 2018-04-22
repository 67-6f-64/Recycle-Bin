#include<Windows.h>
#include"AirMemory.h"
#include"Packet.h"
#include"hook.h"
#include"AutoLogin.h"


WORD Header_Password1;
WORD Header_WorldSelect;
WORD Header_Password2;
WORD Header_CharacterSelect;

OutPacket LoginPacket[4];
bool bAutoLogin = true;
bool Disconnected = false;
DWORD LoginPacketCount = 0;

void UpdatePacketAL(SPacket *p){
	if(!bAutoLogin || LoginPacketCount == 15) return;
	WORD Header = *(WORD *)&p->Packet[0];
	if(Header == Header_Password1){
		LoginPacket[0].SetPacket(p);
		LoginPacketCount |= 1;
	}
	else if(Header == Header_WorldSelect){
		LoginPacket[1].SetPacket(p);
		LoginPacketCount |= 2;
	}
	else if(Header ==Header_Password2){
		LoginPacket[2].SetPacket(p);
		LoginPacketCount |= 4;
	}
	else if(Header == Header_CharacterSelect){
		LoginPacket[3].SetPacket(p);
		LoginPacketCount |= 8;
	}
}


void DelaySendAL(){
	Sleep(1000);
	LoginPacket[0].SendPacket();
}

void ReplyPacketAL(RPacket *rp){
	if(!bAutoLogin || LoginPacketCount !=15) return;
	InPacket p(rp);
	WORD Header = p.DecodeHeader();
	HANDLE hThread;

	switch(Header){
	case 0x0022://LoginScreen
		Disconnected = true;
		break;
	case 0x0016://HeartBeat
		if(Disconnected){
			if(p.Decode1() == 0x0B){
				Disconnected = false;
				hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DelaySendAL, NULL, NULL, NULL);
				CloseHandle(hThread);
			}
		}
		break;
	case 0x0002://WorldSelect
		LoginPacket[1].SendPacket();
		break;
	case 0x0023://CharacterSelect
		LoginPacket[2].SendPacket();
		LoginPacket[3].SendPacket();
		break;
	default:
		break;
	}
}

void AobErrorAL(){
	MessageBoxA(NULL, "Aob is broken(AutoLogin)", "Error", NULL);
}


void AutoLogin_Hook(){
	AirMemory AM;
	DWORD Address;
	AM.Init();
	Address = AM.AobScan("6A ?? 8D 4C 24 ?? E8 ?? ?? ?? ?? 51 8B CC 89 64 24 ?? 56 89 5C 24 ?? E8");
	if(!Address) AobErrorAL();

	Header_Password2 = *(BYTE *)(Address + 1);
	Header_Password1 = Header_Password2 - 2;
	Header_WorldSelect = Header_Password2 - 1;
	Header_CharacterSelect = Header_Password2 + 1;

}