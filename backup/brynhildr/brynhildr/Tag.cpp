#include<Windows.h>
#include<stdio.h>
#include"html.h"

HTMLTag::HTMLTag(char *pp, char *pn){
	int length;

	for(length=0; &pp[length] <= pn; length++){
		if(pp[length] == ' '){
			break;
		}
	}

	tag = (char *)malloc(length + 1);
	memcpy(tag, pp, length);
	tag[length] = 0;

	length = pn - pp + 1;
	fulltag = (char *)malloc(length + 1);
	memcpy(fulltag, pp, length);
	fulltag[length] = 0;

	next = NULL;
	parent = NULL;
	text = NULL;
}

HTMLTag::~HTMLTag(){
	free(tag);
	free(fulltag);
	if(text){
		free(text);
	}
}

HTMLTag* HTMLTag::GetNext(){
	return next;
}

HTMLTag* HTMLTag::GetParent(){
	return parent;
}

void HTMLTag::SetNext(HTMLTag *ht){
	next = ht;
}

void HTMLTag::SetParent(HTMLTag *ht){
	parent = ht;
}

char* HTMLTag::GetTag(){
	return tag;
}

char* HTMLTag::GetFullTag(){
	return fulltag;
}

bool HTMLTag::Get(char *name, char *out){
	char *ename, *sp;
	int length = strlen(name) + 1;
	ename = (char *)malloc(length + 1);
	sprintf(ename, "%s=", name);
	sp = strstr(fulltag, ename);
	if(!sp){
		free(ename);
		return false;
	}
	else{
		int i, j = 0, count = 0;
		sp += strlen(ename);
		for(i=0; sp[i]; i++){
			switch(sp[i]){
			case '\"':
				count++;
				break;
			default:
				if(count == 1){
					out[j++] = sp[i];
				}
				break;
			}
		}
		out[j] = 0;
		free(ename);
		return true;
	}
}

void HTMLTag::SetText(char *s){
	int length = strlen(s);
	text = (char *)malloc(length +1);
	memcpy(text, s, length + 1);
}
char* HTMLTag::GetText(){
	return text;
}