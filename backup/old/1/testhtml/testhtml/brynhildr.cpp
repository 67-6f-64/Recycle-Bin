#include<Windows.h>
#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")
#include<stdio.h>
#include"brynhildr.h"

Brynhildr::Brynhildr(char *url){
	memcpy(cURL, url, strlen(url) + 1);
	hBrowser = InternetOpenA("brynhildr", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	Referer = (char *)malloc(strlen(url) + 10);
	sprintf(Referer, "Referer: %s", url);
}

Brynhildr::~Brynhildr(){
	free(Referer);
	InternetCloseHandle(hBrowser);
}

void Brynhildr::Replace(char *s, char *p, char *n){
	char *sl = strstr(s, p);
	if(!sl) return;
	char *sr = sl + strlen(p);
	sprintf(sl, "%s%s", n, sr);
	return Replace(s, p, n);
}

void Brynhildr::Access(char *url){
	HINTERNET hPage;
	DWORD Length;
	if(strncmp(url, "http", 4) == 0){
		memcpy(cURL, url, strlen(url) + 1);
		hPage = InternetOpenUrlA(hBrowser, url, Referer, strlen(Referer), INTERNET_FLAG_RELOAD, 0);
	}
	else{
			int i, length;
			length = strlen(cURL);
			for(i=0; &cURL[length - i] >= cURL; i++){
				if(cURL[length - i] == '/'){
					sprintf(&cURL[length - i + 1], "%s", url);
					Replace(cURL, "&amp;", "&");
					break;
				}
			}
	}
	hPage = InternetOpenUrlA(hBrowser, cURL, Referer, strlen(Referer), INTERNET_FLAG_RELOAD, 0);
	InternetReadFile(hPage, Buffer, MAX_HTML_SIZE, &Length);
	Buffer[Length] = 0;
	InternetCloseHandle(hPage);
}

char* Brynhildr::Get(){
	return Buffer;
}

void Brynhildr::Convert(){
	wchar_t ws[MAX_HTML_SIZE];
	MultiByteToWideChar(CP_UTF8, 0, Buffer, MAX_HTML_SIZE, ws, MAX_HTML_SIZE);
	WideCharToMultiByte(CP_ACP, 0, ws, MAX_HTML_SIZE, Buffer, MAX_HTML_SIZE, NULL, NULL);
}