#include<Windows.h>
#include<stdio.h>
#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")
#include"html.h"

//log
class Log{
private:
	FILE *fp;
public:
	Log(char *filename);
	~Log();
	void Write(char *s, bool lf = true);
};

Log::Log(char *filename){
	SYSTEMTIME st;
	int length = strlen(filename);
	char *fn = (char *)malloc(length + 10);
	GetLocalTime(&st);
	sprintf(fn, "%04d%02d%02d_%s", st.wYear, st.wMonth, st.wDay, filename);
	fp = fopen(fn, "w");
}

Log::~Log(){
	fclose(fp);
}

void Log::Write(char *s, bool lf){
	fprintf(fp, "%s", s);
	if(lf){
		fprintf(fp, "\n");
	}
}

//




class Brynhildr{
private:
	HINTERNET hBrowser;
	HINTERNET hPage;
	char *Referer;
	char Buffer[0x10000];
	char cURL[256];
public:
	Brynhildr(char *url);
	~Brynhildr();
	void Open(char *url);
	void Close();
	char* Read();
};

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
///
void Replace(char *s, char *p, char *n){
	char *sl = strstr(s, p);
	if(!sl) return;
	char *sr = sl + strlen(p);
	sprintf(sl, "%s%s", n, sr);
	return Replace(s, p, n);
}
///
void Brynhildr::Open(char *url){
	//Sleep(2000);
	if(strncmp(url, "http", 4) == 0){
		memcpy(cURL, url, strlen(url) + 1);
		hPage = InternetOpenUrlA(hBrowser, url, Referer, strlen(Referer), INTERNET_FLAG_RELOAD, 0);
		//printf("URL:%s\n", url);
	}
	else{
		//if(strncmp(url, "./", 2) == 0){
			int i, length;
			length = strlen(cURL);
			for(i=0; &cURL[length - i] >= cURL; i++){
				if(cURL[length - i] == '/'){
					sprintf(&cURL[length - i + 1], "%s", url);
					Replace(cURL, "&amp;", "&");
					hPage = InternetOpenUrlA(hBrowser, cURL, Referer, strlen(Referer), INTERNET_FLAG_RELOAD, 0);
					//printf("URL:%s\n", cURL);
					break;
				}
			}
		//}
	}
}

void Brynhildr::Close(){
	InternetCloseHandle(hPage);
}

char* Brynhildr::Read(){
	DWORD Length;
	InternetReadFile(hPage, Buffer, 0x10000, &Length);
	Buffer[Length] = 0;
	return Buffer;
}
//

Log log("rakuen.txt");

void machine(Brynhildr *br, char *url);
void machinelist(Brynhildr *br, char *url){
	br->Open(url);
	HTMLParser hp(br->Read());
	HTMLTag *ht;
	char href[256];
	int i = 0;
	br->Close();
	while(1){
		ht = hp.Find("div id=\"model_link\"->ul->a", i++);//機種
		if(!ht){
			break;
		}
		//if(!strstr(ht->GetText(), "ジャグラー")){
			ht->Get("href", href);
			printf("%s\n", ht->GetText());
			log.Write(ht->GetText());
			machine(br, href);
		//}
	}
	ht = hp.Find("div class=\"list_navigation\"->div style=\"float:right;\"->a");//次へ
	if(ht){
		ht->Get("href", href);
		return machinelist(br, href);
	}
}

void lastgame(Brynhildr *br, char *url){
	br->Open(url);
	HTMLParser hp(br->Read());
	HTMLTag *ht;
	int i = 0;
	br->Close();
	ht = hp.Find("head->title");
	if(ht){
		if(strcmp(ht->GetText(), "エラーページ") == 0){
			Sleep(3000);
			return lastgame(br, url);
		}
	}
	ht = hp.Find("td align=\"right\"", 2);
	if(ht){
		if(!hp.Find("td align=\"right\"", 5)){
			printf("@");
			log.Write("@", false);
		}
		printf("%sg\n", ht->GetText());
		log.Write(ht->GetText(), false);
		log.Write("g");
	}
	else{
		Sleep(500);
		return lastgame(br, url);
	}
}

void rireki(Brynhildr *br, char *top){
	br->Open(top);
	HTMLParser hp(br->Read());
	HTMLTag *ht;
	int i = 0;
	char href[256];
	br->Close();
	while(1){
		ht = hp.Find("div class=\"daiban\"", i++);
		if(ht){
			ht = ht->GetParent();//link
			ht->Get("href", href);
			printf("%s:", ht->GetText());
			log.Write(ht->GetText(), false);
			log.Write(":", false);
			Sleep(2000);
			lastgame(br, href);
		}
		else{
			break;
		}
	}
}

void machine(Brynhildr *br, char *url){
	br->Open(url);
	HTMLParser hp(br->Read());
	HTMLTag *ht;
	char href[256];
	int i = 0;
	br->Close();
	while(1){
		ht = hp.Find("div id=\"menu_link\"->ul->a", i++);//大当たり履歴データ
		if(!ht){
			break;
		}
		if(ht->GetText() && strstr(ht->GetText(), "大当り履歴データ")){
			ht->Get("href", href);
			rireki(br, href);
			break;
		}
	}
}

int main(){
	Brynhildr br("http://www.d-deltanet.com/");
	br.Open("http://www.d-deltanet.com/pc/D0101.do?pmc=22021006");
	HTMLParser hp(br.Read());
	HTMLTag *ht;
	char href[256];
	char *tt;
	int i = 0;

	while(1){
		ht = hp.Find("a", i++);
		if(ht && ht->Get("href", href) && ht->GetText()){
			if(strstr(ht->GetText(), "【20】スロ")){
				break;
			}
		}
		else{
			break;
		}
	}
	br.Close();
	machinelist(&br, href);
	//machine(&br);
	return 0;
}