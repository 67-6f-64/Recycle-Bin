#include<stdio.h>
#pragma comment(lib, "Kyubey.lib")
#include"Kyubey.h"

#define H_CONNECT	0x0100//[Header]
#define H_EXIT		0x01FF//[Header]
#define H_DELIVERY	0x0001//[Header] [String(CharName)] [4 Bytes(Time)]
#define H_DATABASE	0x0002//[Header]
#define H_STATE		0x0003//[Header] [BYTE(Server)]
#define H_GETSTATE	0x0004//[Header]

void writetest(Kyubey *Client, char *Name){
	OutPacket p(H_DELIVERY);
	p.EncodeString(Name);
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	p.Encode1(stTime.wMinute);
	p.Encode1(stTime.wHour);
	p.Encode1(stTime.wDay);
	p.Encode1(stTime.wMonth);
	Client->SendPacket(&p);
}

void DownloadLog(InPacket *ip){
	HANDLE hFile = CreateFileA("Delivery.log", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	CloseHandle(hFile);
}

#define H_DATA_START 0x0010
#define H_DATA		 0x0011
#define H_DATA_END	 0x0012

#define H_BOTDATA_START	 0x0020
#define H_BOTDATA		 0x0021
#define H_BOTDATA_END	 0x0022

void getdata(Kyubey *Client){
	OutPacket p(H_DATABASE);
	Client->SendPacket(&p);

	InPacket ip;

	FILE *fp;

	while(Client->ProcessPacket(&ip) >= 2){
		WORD Header = ip.Decode2();
		switch(Header){
		case H_DATA_START:
			fp = fopen("NewDelhi.txt", "w");
			printf("->NewDelhi.txt\n");
			break;
		case H_DATA:
			char s[0x10];
			ip.DecodeString(s);
			fprintf(fp, "%02d/%02d %02d:%02d %s\n",ip.Decode1(), ip.Decode1(), ip.Decode1(), ip.Decode1(), s);
			break;
		case H_DATA_END:
			fclose(fp);
			return;
		}
	}
}

void getstate(Kyubey *Client){
	OutPacket p(H_GETSTATE);
	Client->SendPacket(&p);
	InPacket ip;
	printf("current state:\n");
	while(Client->ProcessPacket(&ip) >= 2){
		WORD Header = ip.Decode2();
		switch(Header){
		case H_BOTDATA_START:
			break;
		case H_BOTDATA:
			char s[0x10];
			ip.DecodeString(s);
			ULONGLONG Meso;
			Meso = ip.Decode8();
			printf("(%02d/%02d %02d:%02d)%-12s:%lld\n", ip.Decode1(), ip.Decode1(), ip.Decode1(), ip.Decode1(), s, Meso);
			break;
		case H_BOTDATA_END:
			return;
		}
	}
}

void con(Kyubey *Client){
	for(int i=0; i<1337; i++){
		OutPacket p(H_CONNECT);
		Client->SendPacket(&p);
	}
}

int main(){
	Kyubey Client("192.168.0.101", 1337);
	getdata(&Client);
	getstate(&Client);
	con(&Client);
	OutPacket p(H_EXIT);
	Client.SendPacket(&p);
	Sleep(5000);
	return 0;
}