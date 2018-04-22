#ifndef __MAGICA_H__
#define __MAGICA_H__

typedef struct Frame{
	LPVOID lpv1;
	LPVOID lpv2;
	HWND hWnd;
	LPVOID lpv4;
	LPVOID lpv5;
	LPVOID lpv6;
	LPVOID lpv7;
	DWORD time;
};

extern Frame *Client;

extern bool bMagica;
extern bool bMagica2;

void magica();
extern char psOpen[0x100];

#endif