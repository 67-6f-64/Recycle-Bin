#include<stdio.h>
#include<Windows.h>
#include<Wininet.h>
#pragma comment(lib, "Wininet.lib")
#include"ShizukuChan.h"

ShizukuChan::ShizukuChan(char *url){
	hIO = InternetOpenA("Shizuku-Chan ver1.00", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	TextData = (char *)malloc(MAXSIZE);
	ZeroMemory(TextData, MAXSIZE);
	TextDataLength = 0;
	ZeroMemory(DomainName, sizeof(DomainName));
	ZeroMemory(Referer, 256);
	Access(url);
}

ShizukuChan::~ShizukuChan(){
	InternetCloseHandle(hIO);
	ZeroMemory(TextData, MAXSIZE);
	free(TextData);
	TextData = NULL;
	TextDataLength = 0;
}

void ShizukuChan::Access(char *url){
	hIOU = InternetOpenUrlA(hIO, url, Referer, strlen(Referer), INTERNET_FLAG_RELOAD, 0);
	SaveTextData();
	InternetCloseHandle(hIOU);
	int i;
	for(i=strlen(url); i>=0; i--){
		if(url[i] == '/'){
			RtlCopyMemory(DomainName, url, i + 1);
			break;
		}
	}
	printf(">>%s\n", url);
	sprintf(Referer, "Referer: %s", url);
	//scHTML html(TextData);
	//html.ShowTextData();
}

void ShizukuChan::JSAccess(char *url){
	hIOU = InternetOpenUrlA(hIO, url, Referer, strlen(Referer), INTERNET_FLAG_RELOAD, 0);
	SaveTextData();
	InternetCloseHandle(hIOU);
	int i;
	for(i=strlen(url); i>=0; i--){
		if(url[i] == '/'){
			RtlCopyMemory(DomainName, url, i + 1);
			break;
		}
	}
	//printf(">>%s\n", url);
	//sprintf(Referer, "Referer: %s", url);
}

void ShizukuChan::SaveTextData(){
	DWORD ReadSize = 0;
	ZeroMemory(TextData, MAXSIZE);
	TextDataLength = 0;
	while(InternetReadFile(hIOU, &TextData[TextDataLength], 1024, &ReadSize)){
		if(!ReadSize){
			break;
		}
		TextDataLength += ReadSize;
	}
}

void ShizukuChan::ShowTextData(){
	printf("%s\n", TextData);
}

void ShizukuChan::Processing(){
	char *NewTextData = strstr(TextData, "<tr><td align=\"right\">-</td><td align=\"right\">--</td><td align=\"right\">");//先頭
	if(!NewTextData){
		printf("エラーp1\n");
		return;
	}
	NewTextData += strlen("<tr><td align=\"right\">-</td><td align=\"right\">--</td><td align=\"right\">");
	char *NewTextDataEnd = strstr(NewTextData, "</td></tr>");//末尾
	if(!NewTextDataEnd){
		printf("エラーp2\n");
		return;
	}
	NewTextDataEnd[0] = 0;//終端付加
	printf("最終回転数:%s\n", NewTextData);
}


//Shizuku Chan HTML class
scHTML::scHTML(char *s){
	TextData = (char *)malloc(MAXSIZE);
	RtlCopyMemory(TextData, s, strlen(s));
	Compress();
}

scHTML::~scHTML(){
	free(TextData);
}

void scHTML::ShowTextData(){
	printf("%s", TextData);
}

void scHTML::Replace(char *s1, char *s2){
	char *pp = strstr(TextData, s1);

	if(!pp){
		return;
	}
	
	char *pn1 = pp + strlen(s1);
	char *pn2 = pp + strlen(s2);
	memcpy((void *)pn2, (void *)pn1, strlen(pn1) + 1); 
	memcpy((void *)pp, (void *)s2, strlen(s2));

	return Replace(s1, s2);
}

void scHTML::Cut(char *sp, char *sn){
	char *pp = strstr(TextData, sp);

	if(!pp){
		return;
	}

	char *pn = strstr(pp, sn);

	if(!pn){
		return;
	}

	sprintf(pp, "%s", pn + strlen(sn));
	return Cut(sp, sn);
}

void scHTML::Delete(char *s){
	return Replace(s, "");
}

void scHTML::Reduce(char *sp, char *sn){
	char *pp = strstr(TextData, sp);

	if(!pp){
		return;
	}

	char *pn = strstr(pp, sn);

	if(!pn){
		return;
	}

	pn[strlen(sn)] = 0;
	sprintf(TextData, "%s", pp);
}

void scHTML::DeleteUnwantedTag(char *sp, char *sn){
	char *pp = strstr(TextData, sp);

	if(!pp){
		return;
	}

	char *pn = strstr(pp, sn);

	if(!pn){
		return;
	}
	int i;
	int p_frtag = -1;
	for(i=0; i<(pn - pp); i++){
		if( p_frtag == -1 && pp[i] == '>'){
			p_frtag = i;
			break;
		}
	}
	sprintf(&pp[p_frtag + 1], "%s", pn);
	//return DeleteUnwantedTag(sp, sn);
}

void scHTML::Compress(){
	Reduce("<body ", "</body>");
	Replace("&amp;", "&");
	Replace("./", "http://www.d-deltanet.com/pc/");
	Cut("<script ", "</script>");
	
	Cut("<img", ">");
	Cut("<div", ">");
	Delete("</div>");
	Delete("<BR>");
	Delete("<li>");
	Delete("</li>");
	Cut("<ul", ">");
	Delete("</ul>");
	Replace("  ", " ");
	Replace("	", "");
	//DeleteUnwantedTag("<a", "</a>");
	//Replace("\n", "");
	//Replace("\r\n\r\n", "\r\n");
	//Cut("<script ", "</script>");
	//Cut("<div", "</div>");
}