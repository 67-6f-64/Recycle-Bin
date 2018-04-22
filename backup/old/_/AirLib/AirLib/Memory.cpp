#include"Memory.h"

void Sage::MemorySage(HANDLE This){
	This = GetCurrentProcess();
	while(1){
		EmptyWorkingSet(This);
		Sleep(1000);
	}
}

void Sage::StartMemorySage(){
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MemorySage, GetCurrentProcess(), NULL, NULL);
}