#include<Windows.h>
#include<stdio.h>
#include"html.h"

HTMLParser::HTMLParser(char *src){
	int length = strlen(src);
	html = (char *)malloc(length + 1);
	memcpy(html, src, length);
	html[length] = 0;
	htlist = NULL;
	htp = NULL;
	hti = NULL;
	Search();
}

HTMLParser::~HTMLParser(){
	free(html);
	HTMLTag *ntag = hti;
	while(ntag){
		hti = ntag;
		ntag = ntag->GetNext();
		delete hti;
	}
	hti = NULL;
}

void HTMLParser::Search(int d){
	char *pp, *pn;
	pp = strstr(&html[d], "<");
	if(!pp){
		//printf("Search:Exit\n");
		return;
	}
	else if(strncmp(pp, "<!--", 4) == 0){
		pn = strstr(pp, "-->");
		if(!pn){
			//printf("Search:Error(Comment)\n");
			return;
		}
		else{
			return Search(pn + strlen("-->") - html);
		}
	}
	else{
		pn = strstr(pp, ">");
		if(!pn){
			//printf("Search:Error(Tag)\n");
			return;
		}
		else{
			pp++;
			pn--;
			HTMLTag *ht = new HTMLTag(pp, pn);
			if(ht->GetTag()[0] != '/'){
				char *stag = (char *)malloc(strlen(ht->GetTag()) + 4);
				sprintf(stag, "</%s>", ht->GetTag());
				char *st = &pn[2], *et = strstr(pn, stag);
				if(et){
					////////////////////////
					int i, j, length;
					bool btext = true;
					char *text;
					length = 0;
					for(i=0; &st[i] < et; i++){
						switch(st[i]){
						case '\t':
						case '\n':
						case '\r':
						case ' ':
							break;
						case '<':
							btext = false;
							break;
						case '>':
							btext = true;
							break;
						default:
							length++;
							break;
						}
					}
					text = (char *)malloc(length + 1);
					j = 0;
					for(i=0; &st[i] < et; i++){
						switch(st[i]){
						case '\t':
						case '\n':
						case '\r':
						case ' ':
							break;
						case '<':
							btext = false;
							break;
						case '>':
							btext = true;
							break;
						default:
							if(btext){
								text[j++] = st[i];
							}
							break;
						}
					}
					text[j] = 0;
					if(j){
						ht->SetText(text);
					}
					free(text);
					//////////////////////////////
					if(!htlist){
						htlist = ht;
						hti = ht;
					}
					else{
						ht->SetParent(htp);
						htlist->SetNext(ht);
						htp = ht;
						htlist = ht;
					}
				}
				else{
					if(!htlist){
						htlist = ht;
						hti = ht;
					}
					else{
						ht->SetParent(htp);
						htlist->SetNext(ht);
						htlist = ht;
					}
				}
				free(stag);
			}
			else{
				if(htp){
					htp = htp->GetParent();
					ht->SetParent(htp);
					htlist->SetNext(ht);
					htlist = ht;
				}
			}
			return Search(pn + strlen("<") - html);
		}
	}
}

void HTMLParser::ShowTree(){
	HTMLTag *ht = hti;
	while(ht){
		htp = ht->GetParent();
		while(htp){
			printf("_");
			htp = htp->GetParent();
		}
		printf("<%s>\n", ht->GetTag());
		ht = ht->GetNext();
	}
}
///////////////////////////////////////////
class CharPlus{
private:
	int count;
	char *target[16];
public:
	CharPlus();
	~CharPlus();
	void Add(char *s);
	int GetCount();
	char* GetChar(int d);
};

CharPlus::CharPlus(){
	count = 0;
}
CharPlus::~CharPlus(){
	int i;
	for(i=0; i<count; i++){
		free(target[i]);
	}
}
void CharPlus::Add(char *s){
	int length = strlen(s);
	target[count] = (char *)malloc(length + 1);
	memcpy(target[count], s, length);
	target[count++][length] = 0;
}
int CharPlus::GetCount(){
	return count;
}
char* CharPlus::GetChar(int d){
	return target[d];
}
////////////////////////////////////////////
HTMLTag* HTMLParser::Find(char *path, int d){
	char *sp, *tp, *cpath;
	CharPlus cp;
	int i, count, tcount = 0;
	HTMLTag *ht = hti;
	cpath = (char *)malloc(strlen(path) + 1);
	memcpy(cpath, path, strlen(path) + 1);
	do{
		sp = cpath;
		do{
			tp = strstr(sp, "->");
			if(tp){
				sp = tp + strlen("->");
			}
		}while(tp);
		cp.Add(sp);
		if(sp != cpath){
			sp[-strlen("->")] = 0;
		}
	}while(sp != cpath);
	free(cpath);
	count = cp.GetCount();
	do{
		HTMLTag *htp = ht;
		for(i=0; i<count && htp; i++){
			if(strstr(cp.GetChar(i), " ")?strstr(htp->GetFullTag(), cp.GetChar(i)):strcmp(htp->GetTag(), cp.GetChar(i)) == 0){
				if(i == count -1){
					if(tcount == d){
						return ht;
					}
					else{
						tcount++;
					}
				}
				htp = htp->GetParent();
			}
			else{
				break;
			}
		}
		ht = ht->GetNext();
	}while(ht);
	return 0;
}

HTMLTag * HTMLParser::GetTagList(){
	return hti;
}