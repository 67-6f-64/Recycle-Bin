#include"Kyubey.h"
#pragma comment(lib, "Kyubey.lib")
#include"Client.h"

#define H_CONNECT	0x0100//[Header]
#define H_EXIT		0x01FF//[Header]
#define H_DELIVERY	0x0001//[Header] [String(CharName)] [4 Bytes(Time)]
#define H_DATABASE	0x0002//[Header]
#define H_STATE		0x0003//[Header]

Kyubey KyubeyClient("127.0.0.1", 1337);

void ConnectTest(Kyubey *s){
	OutPacket p(H_CONNECT);
	s->SendPacket(&p);
}

void KeepAlive(){
	OutPacket p(H_CONNECT);
	KyubeyClient.SendPacket(&p);

	DWORD tid = GetCurrentThreadId();
	InPacket ip;
	while(1){
		while(KyubeyClient.ProcessPacket(&ip) >= 2){
			WORD Header = ip.Decode2();
			switch(Header){
			case H_CONNECT:
				Sleep(5000);
				ConnectTest(&KyubeyClient);
				break;
			default:
				break;
			}
		}
		Sleep(3000);
		KyubeyClient.Reconnect();
	}
}

void SendDeliveryLog(char *Name){
	OutPacket p(H_DELIVERY);
	p.EncodeString(Name);
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	p.Encode1(stTime.wMinute);
	p.Encode1(stTime.wHour);
	p.Encode1(stTime.wDay);
	p.Encode1(stTime.wMonth);
	KyubeyClient.SendPacket(&p);
}

void SendMesoValue(char *MapleID, ULONGLONG Meso){
	OutPacket p(H_STATE);
	p.EncodeString(MapleID);
	p.Encode8(Meso);
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	p.Encode1(stTime.wMinute);
	p.Encode1(stTime.wHour);
	p.Encode1(stTime.wDay);
	p.Encode1(stTime.wMonth);
	KyubeyClient.SendPacket(&p);
}