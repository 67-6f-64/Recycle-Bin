#include<stdio.h>
#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define HEADER_ACCEPTED 0x1337
#define HEADER_MESSAGE 0x0001
#define HEADER_EXIT 0x0002

class OutPacket{
public:
	BYTE Packet[256];
	DWORD Length;
	OutPacket(WORD Header);
	void Encode2(WORD w);
	void EncodeString(char *s);
};

OutPacket::OutPacket(WORD Header){
	Length = 0;
	Encode2(Header);
}

void OutPacket::Encode2(WORD w){
	*(WORD *)&Packet[Length] = w;
	Length += 2;
}

void OutPacket::EncodeString(char *s){
	int len = strlen(s) + 1;
	memcpy(&Packet[Length], (void *)s, len);
	Length += len;
}

class InPacket{
public:
	BYTE *Packet;
	DWORD DecodedLength;
	InPacket(BYTE *b);
	WORD Decode2();
	char* DecodeString();
};

InPacket::InPacket(BYTE *b){
	Packet = b;
	DecodedLength = 0;
}

WORD InPacket::Decode2(){
	WORD w = *(WORD *)&Packet[DecodedLength];
	DecodedLength += 2;
	return w;
}

char* InPacket::DecodeString(){
	char *s = (char *)&Packet[DecodedLength];
	DecodedLength += strlen(s) + 1;
	return s;
}


class Madoka{
private:
	SOCKET Server;
	sockaddr_in addr;
public:
	Madoka(char *Address, WORD Port);
	~Madoka();
	void ReceivePacket(BYTE *Packet);
	void SendPacket(OutPacket *p);
};

Madoka::Madoka(char *Address, WORD Port){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	Server = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	addr.sin_addr.S_un.S_addr = inet_addr(Address);
	connect(Server, (struct sockaddr *)&addr, sizeof(addr));
	printf("Server:%s(%d)\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}

Madoka::~Madoka(){
	closesocket(Server);
	WSACleanup();
}

void Madoka::ReceivePacket(BYTE *Packet){
	recv(Server, (char *)Packet, 256, 0);
}

void Madoka::SendPacket(OutPacket *p){
	send(Server, (char *)p->Packet, p->Length, 0);
	//WORD Header;
	//recv(Server, (char *)&Header, 256, 0);
}

int main(){
	Madoka Client("192.168.0.100", 1337);

	for(int i=0; i<3; i++){
		OutPacket op(HEADER_MESSAGE);
		op.EncodeString("to server");
		Client.SendPacket(&op);
		printf("-> Send\n");

		BYTE Packet[256];
		Client.ReceivePacket(Packet);
			InPacket ip(Packet);

			WORD Header = ip.Decode2();
			if(Header == HEADER_MESSAGE){
				printf("<-@%04X \"%s\" (%d)\n", Header, ip.DecodeString(), ip.DecodedLength);
			}
			else{
				printf("<-@%04X Unknown Packet (%d)\n", Header, ip.DecodedLength);
				break;
			}
	}
	OutPacket op2(HEADER_EXIT);
	Client.SendPacket(&op2);
	printf("-> Exit\n");

	return 0;
}