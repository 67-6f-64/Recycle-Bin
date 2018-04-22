#include"Kyubey.h"

#define H_EXIT		0x01FF//[Header]
#define H_DELIVERY	0x0001//[Header] [String(CharName)] [4 Bytes(Time)]
#define H_DATABASE	0x0002//[Header]
#define H_STATE		0x0003//[Header] [BYTE(Server)]

void Delivery(InPacket *ip){
	char s[0x100];
	ip->DecodeString(s);
	printf("%p:[Delivery]%sX\n", GetCurrentThreadId(), s);
}

void Database(InPacket *ip){

}

void State(InPacket *ip){

}

void thread(Kyubey *s){
	DWORD tid = GetCurrentThreadId();
	InPacket ip;
	while(s->ProcessPacket(&ip) >= 2){
		WORD Header = ip.Decode2();
		switch(Header){
		case H_DELIVERY:
			Delivery(&ip);
			break;
		case H_DATABASE:
			Database(&ip);
			break;
		case H_STATE:
			State(&ip);
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
	Kyubey Server(1337, thread);
	return 0;
}