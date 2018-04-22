#include<stdio.h>
#include<Windows.h>
#include"html.h"
#include"brynhildr.h"

void main(){
	Brynhildr br("https://news.yahoo.co.jp/");
	br.Access("https://news.yahoo.co.jp/pickup/6236169");
	br.Convert();
	printf("%s\n", br.Get());
}