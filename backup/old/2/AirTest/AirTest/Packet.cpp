#include"AirTest.h"

typedef struct _SPacket{
	DWORD Always01;
	BYTE *Packet;
	DWORD Size;
	DWORD Always02;
}SPacket;

/*
class AirPacket{
private:
	DWORD SF_JMP;
	DWORD SF_FAKE_RET;
	DWORD SF_CLASS;
	SPacket NewPacket;
public:
	AirPacket();
	~AirPacket();
	void Init(WORD Header);
	void SendPacket();
	void FreePacket();
};

AirPacket::AirPacket(){
	SF_JMP = 0x00602AC0;
	SF_FAKE_RET = 0x020E6A52;
	SF_CLASS = 0x01CED108;
}

void AirPacket::Init(WORD Header){
	NewPacket.Packet = (BYTE *)malloc(256);
	*(WORD *)&NewPacket.Packet[0] = Header;
	NewPacket.Size = 2;
}

void AirPacket::SendPacket(){
	_asm{
		push RETURN
		push [NewPacket]
		mov ecx,[SF_CLASS]
		mov ecx,[ecx]
		push [SF_RET]//fake return
		jmp dword ptr [SF_JMP]
RETURN:
	}
}

void AirPacket::FreePacket(){
	if(NewPacket.Packet){
		free(NewPacket.Packet);
	}
}

*/
//void (__thiscall *SendPacket)(DWORD ecx, SPacket *Packet) = (void (__thiscall *)(DWORD, SPacket*))0x00602AC0;

DWORD SF_JMP =  0x00602AC0;//sendpacket
DWORD SF_RET = 0x020E6A52;//return address check bypass (nop ret)
void SendFunction(SPacket *Packet){
	_asm{
		push RETURN
		push [Packet]
		mov ecx,0x01CED108
		mov ecx,[ecx]
		push [SF_RET]//fake return
		jmp dword ptr [SF_JMP]
RETURN:
	}
	free(Packet->Packet);
	ZeroMemory(Packet, sizeof(SPacket));
}
DWORD CreatePacket = 0x00B3D050;
void InitSPacket(SPacket *Packet, WORD Header){
	Packet->Packet = (BYTE *)malloc(1024);
	*(WORD *)&Packet->Packet[0] = Header;
	Packet->Size = 2;
}

void Encode4(SPacket *Packet, DWORD dw){
	*(DWORD *)&Packet->Packet[Packet->Size] = dw;
	Packet->Size += 4;
}

void Encode2(SPacket *Packet, WORD w){
	*(WORD *)&Packet->Packet[Packet->Size] = w;
	Packet->Size += 2;
}

void Encode1(SPacket *Packet, BYTE b){
	*(BYTE *)&Packet->Packet[Packet->Size] = b;
	Packet->Size += 1;
}
//block @026A unknown packet (thread detection)
void MapTeleport(DWORD MapID){
	SPacket Packet;
	InitSPacket(&Packet, 0x004C);//Teleport
	Encode1(&Packet, 0x02);//StarPlanet 1ch
	Encode4(&Packet, MapID);//MapID
	Encode2(&Packet, 0);//??
	Encode2(&Packet, 1);//must be 1
	Encode2(&Packet, 0);//??
	Encode4(&Packet, 0);//fixed
	SendFunction(&Packet);
	/*
	InitSPacket(&Packet, 0x002A);//Start HeartBeat
	Encode1(&Packet, 0x01);
	Encode4(&Packet, 0);
	Encode4(&Packet, 0);
	SendFunction(&Packet);
	InitSPacket(&Packet, 0x002A);//Disconnect
	SendFunction(&Packet);
	*/
	//InitSPacket(&Packet, 0x0080);//Disconnect
	//Encode1(&Packet, 0x07);
	//SendFunction(&Packet);
}

void Disconnect(){
	SPacket Packet;
	InitSPacket(&Packet, 0x002A);//Disconnect
	SendFunction(&Packet);
}