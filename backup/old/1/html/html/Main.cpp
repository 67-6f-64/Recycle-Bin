#include<Windows.h>
#include<stdio.h>
#include"html.h"

int main(){
	FILE *fp;
	char b[0x10000];
	int length;
	fp = fopen("testdata.htm", "r");
	length = fread(b, 1, 0x10000, fp);
	b[length] = 0;
	printf("Length:%d\n", length);
	fclose(fp);

	HTMLParser html(b);
	html.ShowTree();
	HTMLTag *ht;
	printf("PAGE:%s\n", html.Find("head->title")->GetText());

	int i = 0;
	char href[256];
	while(1){
		ht = html.Find("div id=\"model_link\"->ul->a", i++);//Л@Он
		if(!ht){
			break;
		}
		if(ht->Get("href", href)){
			printf("%s\n", ht->GetText());
		}
	}

	return 0;
}