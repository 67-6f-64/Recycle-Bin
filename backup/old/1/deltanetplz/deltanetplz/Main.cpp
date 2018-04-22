#include<stdio.h>
#include<Windows.h>
#include<Wininet.h>
#pragma comment(lib, "Wininet.lib")

#include"ShizukuChan.h"

void main(){
	ShizukuChan BUNBUN("http://daidata.goraggio.com/100141/unit_list/?model=%EF%BE%86%EF%BD%AD%EF%BD%B0%EF%BD%B7%EF%BE%9D%EF%BD%B8%EF%BE%9E%EF%BE%8A%EF%BE%85%EF%BE%8A%EF%BE%85&ballPrice=21.28");

	BUNBUN.ShowTextData();
}