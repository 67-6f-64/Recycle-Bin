#include<stdio.h>
#include"brynhildr.h"
#include"html.h"

void Rireki(Brynhildr *br, char *url){
	br->Access(url);
	char *html = br->Get();
	HTMLParser hp(html);
	HTMLTag *ht;
	int i = 0;
	int number, lastgame;

	ht = hp.Find("div class=\"kashitama_head\"");
	if(ht){
		sscanf(ht->GetText(), "%04d", &number);
	}

	ht = hp.Find("td align=\"right\"", 2);
	if(ht){
		sscanf(ht->GetText(), "%d", &lastgame);
	}

	printf("%d:%dg\n", number, lastgame);

}

void DaiList(Brynhildr *br, char *url){
	br->Access(url);
	char *html = br->Get();
	HTMLParser hp(html);
	HTMLTag *ht;
	int i = 0;
	char href[0x100];

	do{
		ht = hp.Find("div class=\"daiban\"", i++);
		if(ht){
			ht = ht->GetParent();
			if(ht){
				ht->Get("href", href);
				Rireki(br, href);
			}
		}
	}while(ht);

}

void Machine(Brynhildr *br, char *url){
	br->Access(url);
	char *html = br->Get();
	HTMLParser hp(html);
	HTMLTag *ht;
	int i = 0;
	char href[0x100];

	do{
		ht = hp.Find("ul->a", i++);
		if(ht){
			if(strstr(ht->GetText(), "�哖�藚���f�[�^")){
				ht->Get("href", href);
				DaiList(br, href);
			}
		}
	}while(ht);

}

//test
void DownloadA(char *url, int number, char *MachineName){
	Brynhildr br("http://www.d-deltanet.com/");
	br.Access(url);
	FILE *fp;
	char FileName[0x100];
	sprintf(FileName, "%04d_%s.png", number, MachineName);
	fp = fopen(FileName, "wb");
	if(fp){
		fwrite(br.Get(), 1, br.GetBufferLength(), fp);
		fclose(fp);
	}
}

void GraphA(Brynhildr *br, char *url){
	br->Access(url);
	char *html = br->Get();
	HTMLParser hp(html);
	HTMLTag *ht;
	int i = 0;
	char href[0x100];
	int number = 0;

	ht = hp.Find("div class=\"kashitama_head\"");
	if(ht){
		sscanf(ht->GetText(), "%04d", &number);
	}

	do{
		ht = hp.Find("img", i++);
		if(ht){
			if(ht->Get("width", href)){
				if(ht->Get("src", href)){
					//printf("%s\n", href);
					strcat(href, "&big=1");
					
					/*
						+name
					*/
					ht = hp.Find("div id=\"header\"");
					if(ht){
						char MachineName[0x100];
						sscanf(ht->GetText(), "�o�ʐ��ڃO���t%s", MachineName);
						DownloadA(href, number, MachineName);
					}

					//printf("c");
				}
			}
		}
	}while(ht);

}

void DaiListA(Brynhildr *br, char *url){
	br->Access(url);
	char *html = br->Get();
	HTMLParser hp(html);
	HTMLTag *ht;
	int i = 0;
	char href[0x100];
	char MachineName[0x100] = {0};
	ht = hp.Find("div id=\"header\"");
	if(ht){
		char MachineName[0x100];
		sscanf(ht->GetText(), "�哖��f�[�^%s", MachineName);
		printf("<%s>\n", MachineName);
		//SetCurrentDirectoryA("C:\\Users\\URYYY\\Desktop\\AkemiHomura\\graph");
		//CreateDirectoryA(MachineName, NULL);
		//SetCurrentDirectoryA(MachineName);
	}

	do{
		ht = hp.Find("div class=\"daiban\"", i++);
		if(ht){
			printf("%s\n", ht->GetText());
			ht = ht->GetParent();
			if(ht){
				ht->Get("href", href);
				GraphA(br, href);
			}
		}
	}while(ht);

	ht = hp.Find("div class=\"list_navigation\"->div style=\"float:right;\"->a");
	if(ht){
		ht->Get("href", href);
		DaiListA(br, href);
	}

}

void MachineA(Brynhildr *br, char *url){
	br->Access(url);
	char *html = br->Get();
	HTMLParser hp(html);
	HTMLTag *ht;
	int i = 0;
	char href[0x100];

	do{
		ht = hp.Find("ul->a", i++);
		if(ht){
			if(strstr(ht->GetText(), "�o�ʐ��ڃO���t")){
				ht->Get("href", href);
				DaiListA(br, href);
			}
		}
	}while(ht);

}
//test

void MachineList(Brynhildr *br, char *url){
	br->Access(url);
	char *html = br->Get();
	HTMLParser hp(html);
	HTMLTag *ht;
	int i = 0;
	char href[0x100];

	do{
		ht = hp.Find("ul->a", i++);
		if(ht){
			/*if(strstr(ht->GetText(), "�o�W���X�N�`�b��E�@���`II")){
				printf("%s\n", ht->GetText());
				ht->Get("href", href);
				Machine(br, href);
			}
			else */
			/*
			if(strstr(ht->GetText(), "�R�~�R�d�x�d�r�`�����o���`")){
				printf("%s\n", ht->GetText());
				ht->Get("href", href);
				MachineA(br, href);
			}
			*/
			SetCurrentDirectoryA("C:\\Users\\URYYY\\Desktop\\2018\\AkemiHomura\\graph");
			ht->Get("href", href);
			MachineA(br, href);
		}
	}while(ht);

	ht = hp.Find("div class=\"list_navigation\"->div style=\"float:right;\"->a");
	if(ht){
		ht->Get("href", href);
		MachineList(br, href);
	}

}

void TopPage(Brynhildr *br){
	br->Access("http://www.d-deltanet.com/pc/D0101.do?pmc=22021006");//�A�N�Z�X����y�[�W
	char *html = br->Get();//��������擾
	HTMLParser hp(html);//html�����
	HTMLTag *ht;
	int i = 0;
	char href[0x100];

	do{
		ht = hp.Find("ul->a", i++);
		if(ht){
			if(strstr(ht->GetText(), "�y1000�~/47���z�X��")){
				ht->Get("href", href);
				MachineList(br, href);//2
				return;
			}
		}
	}while(ht);

}

void main(){
	Brynhildr br("http://www.d-deltanet.com/");//�h���C��(���t�@���Ƒ��΃p�X�Ɏg�p)
	br.SetInterval(2000);

	CreateDirectoryA("C:\\Users\\URYYY\\Desktop\\2018\\AkemiHomura\\graph", NULL);
	SetCurrentDirectoryA("C:\\Users\\URYYY\\Desktop\\2018\\AkemiHomura\\graph");

	TopPage(&br);//1
}