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


class Kyubey{
private:
	SOCKET Server, Client;
	sockaddr_in addr;
	sockaddr_in addr_client;
public:
	Kyubey(char *Address, WORD Port, LPTHREAD_START_ROUTINE thread);
	~Kyubey();
	int ReceivePacket(BYTE *Packet);
	void SendPacket(OutPacket *p);
};

Kyubey::Kyubey(char *Address, WORD Port, LPTHREAD_START_ROUTINE thread){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	Server = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	//addr.sin_addr.S_un.S_addr = inet_addr(Address);
	//addr.sin_addr.S_un.S_addr = inet_addr(INADDR_ANY);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(Server, (sockaddr *)&addr, sizeof(addr));
	listen(Server, 5);
	printf("Server:%s(%d)\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	int len = sizeof(addr_client);
	while(1){
		Client = accept(Server, (struct sockaddr *)&addr_client, &len);
		printf("Client:%s(%d)\n", inet_ntoa(addr_client.sin_addr), ntohs(addr_client.sin_port));
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread, (LPVOID)this, NULL, NULL);
		CloseHandle(hThread);
	}
}

Kyubey::~Kyubey(){
	closesocket(Server);
	WSACleanup();
}

int Kyubey::ReceivePacket(BYTE *Packet){
	int ret = recv(Client, (char *)Packet, 256, 0);
	//WORD Header = 0x1111;
	//send(Client, (char *)Header, 2, 0);
	return ret;
}

void Kyubey::SendPacket(OutPacket *p){
	send(Client, (char *)p->Packet, p->Length, 0);
}

void Reply1(Kyubey *s){
	OutPacket op(HEADER_MESSAGE);
	op.EncodeString("to client\n");
	s->SendPacket(&op);
	printf("-> Reply\n");
}

void thread(Kyubey *s){
	BYTE Packet[256];
	while(s->ReceivePacket(Packet) >= 2){
		InPacket ip(Packet);
		WORD Header = ip.Decode2();
		switch(Header){
		case HEADER_MESSAGE:
			printf("<-@%04X \"%s\" (%d)\n", Header, ip.DecodeString(), ip.DecodedLength);
			Reply1(s);
			break;
		case HEADER_EXIT:
			printf("³íI—¹\n");
			return;
		default:
			printf("<-@%04X Unknown Packet (%d)\n", Header, ip.DecodedLength);
			return;
		}
	}
	printf("Ø’f\n");
}

int main(){
	Kyubey Server("127.0.0.1", 1337, (LPTHREAD_START_ROUTINE)thread);
	return 0;
}