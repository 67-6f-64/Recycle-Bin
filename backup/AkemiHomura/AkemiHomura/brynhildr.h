#ifndef __BRYNHILDR_H__
#define __BRYNHILDR_H__

#ifndef HINTERNET
#include<Windows.h>
#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")
#endif

#define MAX_BUFFER_LENGTH 0x1000

class Brynhildr{
private:
	HINTERNET hBrowser;
	HINTERNET hPage;
	char *Referer;
	char *Buffer;
	char cURL[MAX_BUFFER_LENGTH];
	void Replace(char *s, char *p, char *n);
	DWORD BufferLength;
	int interval;

public:
	Brynhildr(char *url);
	~Brynhildr();
	void Access(char *url);
	char* Get();
	void Convert();//utf to sjis
	DWORD GetBufferLength();
	void SetInterval(int msec);
};

#endif