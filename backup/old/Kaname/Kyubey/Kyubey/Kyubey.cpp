#include"Kyubey.h"

/*
	Class Kyubey
*/

Kyubey::Kyubey(WORD Port, void (*thread)(Kyubey*)){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	Server = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(Server, (sockaddr *)&addr, sizeof(addr));
	listen(Server, 5);
	//printf("====================\nKyubey Server v0.00\nIP Address = %s\nPORT = %d\n====================\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	int len = sizeof(addr_client);
	while(1){
		Client = accept(Server, (struct sockaddr *)&addr_client, &len);
		Kyubey *NewClient = new Kyubey(Client);
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread, (LPVOID)NewClient, NULL, NULL);
		//printf("%p:[Connect]%s@%d\n", GetThreadId(hThread), inet_ntoa(addr_client.sin_addr), ntohs(addr_client.sin_port));
		CloseHandle(hThread);
	}
}

Kyubey::Kyubey(char *Address, WORD Port){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	Client = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	addr.sin_addr.S_un.S_addr = inet_addr(Address);
	connect(Client, (struct sockaddr *)&addr, sizeof(addr));
	Server = 0;
	//printf("Server:%s(%d)\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}

Kyubey::Kyubey(SOCKET NewClient){
	Client = NewClient;
	Server = 0;
}

Kyubey::~Kyubey(){
	if(Server){
		shutdown(Server, SD_BOTH);
		closesocket(Server);
		WSACleanup();
	}
	else{
		shutdown(Client, SD_BOTH);
		closesocket(Client);
	}
}

void Kyubey:: Reconnect(){
	shutdown(Client, SD_BOTH);
	closesocket(Client);
	Client = socket(AF_INET, SOCK_STREAM, 0);
	connect(Client, (struct sockaddr *)&addr, sizeof(addr));
}

int Kyubey::ProcessPacket(InPacket *p){
	signed long int Length, RemainingLength;
	signed long int len;
	p->setDecodedLength(0);
	do{
		len = recv(Client, (char *)&Length, 4, MSG_PEEK);
		if(len < 1){
			return 0;
		}
	}while(len != 4);
	Length = 0;
	len = recv(Client, (char *)&Length, 4, 0);
	if(len < 4 || Length > MAX_LENGTH){
		return 0;
	}
	RemainingLength = Length;
	do{
		len = recv(Client, (char *)(p->getPacket() + (Length - RemainingLength)), RemainingLength, 0);
		if(len < 1){
			return 0;
		}
		RemainingLength -= len;
	}while(RemainingLength);
	return Length;
}

void Kyubey::SendPacket(OutPacket *p){
	char Packet[MAX_LENGTH +4];
	*(DWORD *)&Packet[0] = p->getLength();
	memcpy(&Packet[4], p->getPacket(), p->getLength());
	send(Client, Packet, p->getLength() + 4, 0);
}