#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

int main(){
	BYTE HDDSerial[4];
	PIP_ADAPTER_INFO NetworkAdapter;
	DWORD garbage = 0;

	GetVolumeInformationA("C:\\", NULL, NULL, (DWORD *)HDDSerial, NULL, NULL, NULL, NULL);
	GetAdaptersInfo(NULL, &garbage);
	NetworkAdapter = (IP_ADAPTER_INFO *) malloc(garbage);
	GetAdaptersInfo(NetworkAdapter, &garbage);
	
	printf("NetworkAdapter:%02X %02X %02X %02X %02X %02X\n",
		NetworkAdapter->Address[0], NetworkAdapter->Address[1], NetworkAdapter->Address[2]
	, NetworkAdapter->Address[3], NetworkAdapter->Address[4], NetworkAdapter->Address[5]);

	printf("HDDSerial:%02X %02X %02X %02X\n",
		HDDSerial[0], HDDSerial[1], HDDSerial[2], HDDSerial[3]);

	system("pause");//stop
	return 0;
}