#include<stdio.h>
#pragma comment(lib, "Kyubey.lib")
#include"Kyubey.h"

#define H_CONNECT	0x0100//[Header]
#define H_EXIT		0x01FF//[Header]
#define H_DELIVERY	0x0001//[Header] [String(CharName)] [4 Bytes(Time)]
#define H_DATABASE	0x0002//[Header]
#define H_STATE		0x0003//[Header] [String(MapleID)] [ULONGLONG(Meso)]
#define H_GETSTATE	0x0004//[Header]

CRITICAL_SECTION cs;
void Delivery(InPacket *ip){
	EnterCriticalSection(&cs);
	char s[0x100] = {0};
	DWORD dwTime;
	ip->DecodeString(s);
	dwTime = ip->Decode4();
	printf("%p:[Delivery]%s %p\n", GetCurrentThreadId(), s, dwTime);

	HANDLE hFile = CreateFileA("Delivery.log", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD dw;
	char Name[0x10] = {0};//‰Šú‰»
	while(ReadFile(hFile, Name, 12, &dw, NULL) && dw){
		if(strcmp(s, Name) == 0){
			WriteFile(hFile, &dwTime, 4, &dw, NULL);
			CloseHandle(hFile);
			LeaveCriticalSection(&cs);
			return;
		}
		SetFilePointer(hFile, 4, 0, FILE_CURRENT);
	}
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteFile(hFile, s, 12, &dw, NULL);
	WriteFile(hFile, &dwTime, 4, &dw, NULL);
	CloseHandle(hFile);
	LeaveCriticalSection(&cs);
}

#define H_DATA_START 0x0010
#define H_DATA		 0x0011
#define H_DATA_END	 0x0012

void Database(Kyubey *s){
	EnterCriticalSection(&cs);
	HANDLE hFile = CreateFileA("Delivery.log", GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD dw, dwTime;
	char Name[0x10] = {0};

	OutPacket p(H_DATA_START);
	s->SendPacket(&p);

	while(ReadFile(hFile, Name, 12, &dw, NULL) && ReadFile(hFile, &dwTime, 4, &dw, NULL) && dw){
		OutPacket p2(H_DATA);
		p2.EncodeString(Name);
		p2.Encode4(dwTime);
		s->SendPacket(&p2);
	}

	OutPacket p3(H_DATA_END);
	s->SendPacket(&p3);

	CloseHandle(hFile);
	LeaveCriticalSection(&cs);
}

char MapleID[20][16];
ULONGLONG Meso[20] = {0};
DWORD dwTime[20] = {0};
void State(InPacket *ip){
	int i;
	char s[0x100] = {0};
	ULONGLONG uMeso;
	DWORD dwTimeStamp;
	ip->DecodeString(s);
	uMeso = ip->Decode8();
	dwTimeStamp = ip->Decode4();
	for(i=0; dwTime[i]; i++){
		if(strcmp(MapleID[i], s) == 0){
			Meso[i] = uMeso;
			dwTime[i] = dwTimeStamp;
			return;
		}
	}
	for(i=0; i<20; i++){
		if(!dwTime[i]){
			memcpy(MapleID[i], s, strlen(s) + 1);
			Meso[i] = uMeso;
			dwTime[i] = dwTimeStamp;
			break;
		}
	}
}

#define H_BOTDATA_START	 0x0020
#define H_BOTDATA		 0x0021
#define H_BOTDATA_END	 0x0022

void GetState(Kyubey *s){
	OutPacket p(H_BOTDATA_START);
	s->SendPacket(&p);

	for(int i=0; dwTime[i]; i++){
		OutPacket p2(H_BOTDATA);
		p2.EncodeString(MapleID[i]);
		p2.Encode8(Meso[i]);
		p2.Encode4(dwTime[i]);
		s->SendPacket(&p2);
	}

	OutPacket p3(H_BOTDATA_END);
	s->SendPacket(&p3);
}

void ConnectTest(Kyubey *s){
	OutPacket p(H_CONNECT);
	s->SendPacket(&p);
}

void thread(Kyubey *s){
	DWORD tid = GetCurrentThreadId();
	InPacket ip;
	printf("%p:[Run]\n", tid);
	while(s->ProcessPacket(&ip) >= 2){
		WORD Header = ip.Decode2();
		switch(Header){
		case H_CONNECT:
			//printf("%p:[Test]\n", tid);
			ConnectTest(s);
			break;
		case H_DELIVERY:
			printf("%p:[Delivery]\n", tid);
			Delivery(&ip);
			break;
		case H_DATABASE:
			printf("%p:[DataBase]\n", tid);
			Database(s);
			break;
		case H_STATE:
			printf("%p:[State]\n", tid);
			State(&ip);
			break;
		case H_GETSTATE:
			printf("%p:[GetState]\n", tid);
			GetState(s);
			break;
		case H_EXIT:
			printf("%p:[Exit]\n", tid);
			return;
		default:
			printf("%p:[Error]%04X\n", tid, Header);
			return;
		}
	}
	printf("%p:[Disconnect]\n", tid);
}

int main(){
	InitializeCriticalSection(&cs);
	Kyubey Server(1337, thread);
	DeleteCriticalSection(&cs);
	return 0;
}