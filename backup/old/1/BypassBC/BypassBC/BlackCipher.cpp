#include<Windows.h>
#include"BlackCipher.h"
#include"apihook.h"
#pragma comment(lib, "ws2_32.lib")
#include"memory.h"

void BlackCipher(){
	MessageBoxA(NULL, "Injected", "BypassBC", NULL);
}
