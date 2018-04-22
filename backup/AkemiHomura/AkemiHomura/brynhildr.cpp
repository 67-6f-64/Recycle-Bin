#include"brynhildr.h"
#include<stdio.h>

/*
	Download Class Start
*/

class Download{
private:
	char *memory;
	DWORD length;
	Download *next;

public:
	Download();
	~Download();
	void Add(char *Memory, int Length);
	DWORD GetFileSize();
	char* GetFile();
};

Download::Download(){
	memory = NULL;
	length = 0;
	next = NULL;
}

Download::~Download(){
	free(memory);
	if(next){
		delete next;
	}
}

void Download::Add(char *Memory, int Length){
	if(!memory){
		memory = (char *)malloc(Length);
		memcpy(memory, Memory, Length);
		length = Length;
	}
	else if(!next){
		next = new Download;
		next->Add(Memory, Length);
	}
	else{
		next->Add(Memory, Length);
	}
}

DWORD Download::GetFileSize(){
	Download *DL = this;
	int FileSize = 0;
	while(DL){
		FileSize += DL->length;
		DL = DL->next;
	}
	return FileSize;
}

char* Download::GetFile(){
	DWORD FileSize = GetFileSize();
	DWORD WrittenSize = 0;
	char *File = (char *)malloc(FileSize + 1);
	Download *DL = this;
	while(DL){
		memcpy(&File[WrittenSize], DL->memory, DL->length);
		WrittenSize += DL->length;
		DL = DL->next;
	}
	File[WrittenSize] = 0;
	return File;
}
/*
	Download Class End
*/

Brynhildr::Brynhildr(char *url){
	interval = 0;
	Buffer = NULL;
	memcpy(cURL, url, strlen(url) + 1);
	hBrowser = InternetOpenA("brynhildr", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	Referer = (char *)malloc(strlen(url) + 10);
	sprintf(Referer, "Referer: %s", url);
}

Brynhildr::~Brynhildr(){
	if(Buffer){
		free(Buffer);
	}
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
	if(strncmp(url, "http", 4) == 0){
		memcpy(cURL, url, strlen(url) + 1);
		if(interval){
			Sleep(interval);
		}
		hPage = InternetOpenUrlA(hBrowser, url, Referer, strlen(Referer), INTERNET_FLAG_RELOAD, 0);
	}
	else{
			int i, length;
			length = strlen(cURL);
			for(i=0; &cURL[length - i] >= cURL; i++){
				if(cURL[length - i] == '/'){
					sprintf(&cURL[length - i + 1], "%s", url);
					Replace(cURL, "&amp;", "&");
					if(interval){
						Sleep(interval);
					}
					hPage = InternetOpenUrlA(hBrowser, cURL, Referer, strlen(Referer), INTERNET_FLAG_RELOAD, 0);
					break;
				}
			}
	}
	if(Buffer){
		free(Buffer);
		Buffer = NULL;
	}
	DWORD Length;
	Download DL;
	Buffer = (char *)malloc(MAX_BUFFER_LENGTH);
	do{
		InternetReadFile(hPage, Buffer, MAX_BUFFER_LENGTH, &Length);
		DL.Add(Buffer, Length);
	}while(Length);
	free(Buffer);
	Buffer = DL.GetFile();
	BufferLength = DL.GetFileSize();//Added
	InternetCloseHandle(hPage);
}

char* Brynhildr::Get(){
	return Buffer;
}

void Brynhildr::Convert(){
	int length = strlen(Buffer);
	wchar_t *ws = (wchar_t *)malloc(length * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, Buffer, length, ws, length);
	WideCharToMultiByte(CP_ACP, 0, ws,length, Buffer, length, NULL, NULL);
	free(ws);
}

DWORD Brynhildr::GetBufferLength(){
	return BufferLength;
}

void Brynhildr::SetInterval(int msec){
	interval = msec;
}