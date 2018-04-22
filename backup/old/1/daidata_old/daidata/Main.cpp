#include<stdio.h>
#include"brynhildr.h"
#include"html.h"
#pragma comment(lib, "html.lib")
#pragma comment(lib, "brynhildr.lib")
///////////////////////////////
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
/////////////////////////////
Log log("shinkan.txt");

int sueoki_check = 0;
bool saisyuu_game(Brynhildr *br){
	HTMLParser hp(br->Get());
	HTMLTag *ht;
	int i = 0;

	ht = hp.Find("head->title");

	if(ht){
		if(strcmp(ht->GetText(), "エラーページ") == 0){
			Sleep(3000);
			return false;
		}
	}
	ht = hp.Find("td align=\"right\"", 2);

	if(ht){
		if(!hp.Find("td align=\"right\"", 5)){
			sueoki_check++;
		}
		else{
			sueoki_check = 0;
		}
		if(sueoki_check == 0){
			printf("%sg\n", ht->GetText());
			log.Write(ht->GetText(), false);
			log.Write("g");
		}
		else if(sueoki_check == 3){
			printf("%sg + ??\n", ht->GetText());
			log.Write(ht->GetText(), false);
			log.Write("g + ??");
		}
		else{
			printf("%sg + ", ht->GetText());
			log.Write(ht->GetText(), false);
			log.Write("g + ", false);
		}
	}
	else{
		Sleep(500);
		return false;
	}
	/*
	ht = hp.Find("div class=\"data_update_time\"");
	if(ht){
		DWORD year, month, day, hour, minute;
		sscanf(ht->GetText(), "データ更新時間%04d/%02d/%02d%02d:%02d", &year, &month, &day, &hour, &minute);
		printf("%04d/%02d/%02d %02d:%02d\n", year, month, day, hour, minute);
	}
	*/
	return true;
}

void rireki_data(Brynhildr *br){
	HTMLParser hp(br->Get());
	HTMLTag *ht;
	int i = 0;
	char href[0x100];

	while(1){
		ht = hp.Find("div class=\"daiban\"", i++);
		if(ht){
			ht = ht->GetParent();//link
			ht->Get("href", href);
			printf("%s:", ht->GetText());
			log.Write(ht->GetText(), false);
			log.Write(":", false);
			char *zx = strstr(href, "tdd=");
			sueoki_check = 0;
			while(zx[4] != '4'){
				do{
					Sleep(2000);
					br->Access(href);
				}while(!saisyuu_game(br));
				if(!sueoki_check){
					break;
				}
				zx[4] += 1;
			}
		}
		else{
			break;
		}
	}
}

void dai_top(Brynhildr *br){
	HTMLParser hp(br->Get());
	HTMLTag *ht;
	char href[0x100];
	int i = 0;

	while(1){
		ht = hp.Find("div id=\"menu_link\"->ul->a", i++);//大当たり履歴データ
		if(!ht){
			break;
		}
		if(ht->GetText() && strstr(ht->GetText(), "大当り履歴データ")){
			ht->Get("href", href);
			br->Access(href);
			rireki_data(br);
			break;
		}
	}

}

char ng_list[][0x100] = {
	"ジャグラー",
	"ハナハナ",
	"バーサス",
	"ハナビ",
	"サンダー",
	"クレア",
	"クランキー",
	"聖魔覚醒",
	"Ａ－ＳＬＯＴ",
	//"リノ",
	"ゲッター",
	"パルサー",
	"オアシス",
	"凪のあすから",
	"絆",//kusodai
	NULL
};

bool kisyu_filter(char *s){
	int i = 0;
	while(ng_list[i][0]){
		if(strstr(s, ng_list[i])){
			return false;
		}
		i++;
	}
	return true;
}

void kisyu_list(Brynhildr *br){
	HTMLParser hp(br->Get());
	HTMLTag *ht;
	char href[0x100];
	int i = 0;

	while(1){
		ht = hp.Find("div id=\"model_link\"->ul->a", i++);//機種
		if(!ht){
			break;
		}
		if(kisyu_filter(ht->GetText())){
			ht->Get("href", href);
			printf("%s\n", ht->GetText());
			log.Write(ht->GetText());
			br->Access(href);
			dai_top(br);//3
		}
	}

	ht = hp.Find("div class=\"list_navigation\"->div style=\"float:right;\"->a");//次へ

	if(ht){
		ht->Get("href", href);
		br->Access(href);
		return kisyu_list(br);//2+
	}
}

void top_page(Brynhildr *br){
	HTMLParser hp(br->Get());
	HTMLTag *ht;
	int i = 0;
	char href[0x100];

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

	br->Access(href);
	kisyu_list(br);//2
}

void main(){
	Brynhildr br("http://www.d-deltanet.com/");
	br.Access("http://www.d-deltanet.com/pc/D0101.do?pmc=22021021");
	top_page(&br);//1
}
