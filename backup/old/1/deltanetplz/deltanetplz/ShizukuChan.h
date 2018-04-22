#ifndef __SHIZUKUCHAN_H__
#define __SHIZUKUCHAN_H__

#define MAXSIZE 1048576//1MB

typedef struct tagLinkList{
	char text[256];
	char url[256];
	tagLinkList *Next;
}LinkList;

class scHTML{
private:
	char *TextData;
	void Compress();
	void Replace(char *s1, char *s2);
	void Cut(char *sp, char *sn);
	void Delete(char *s);
	void Reduce(char *sp, char *sn);
	void DeleteUnwantedTag(char *sp, char *sn);

public:
	scHTML(char *s);
	~scHTML();
	void ShowTextData();
};

class ShizukuChan{
private:
	char DomainName[256];
	char Referer[256];
	char *TextData;
	DWORD TextDataLength;
	HINTERNET hIO;
	HINTERNET hIOU;
	void SaveTextData();

public:
	ShizukuChan(char *url);
	~ShizukuChan();
	void Access(char *url);
	void JSAccess(char *url);
	void ShowTextData();
	void Processing();
};

#endif