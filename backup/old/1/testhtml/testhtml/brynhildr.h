#ifndef __BRYNHILDR_H__
#define __BRYNHILDR_H__

#define MAX_HTML_SIZE 0x10000
#define MAX_URL_LENGTH 0x100

#ifndef HINTERNET
#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")
#endif

class Brynhildr{
private:
	HINTERNET hBrowser;
	char *Referer;
	char Buffer[MAX_HTML_SIZE];//html
	char cURL[MAX_URL_LENGTH];//full url
	void Replace(char *s, char *p, char *n);

public:
	Brynhildr(char *url);
	~Brynhildr();
	void Access(char *url);
	char* Get();
	void Convert();
};

#endif