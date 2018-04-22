#pragma once
#include<Windows.h>

class ExportHook{
private:
	HMODULE hModuleEH;
	void FullAccess(DWORD Address, DWORD Size);
	void WriteJMP(DWORD Prev, DWORD Next);
	bool HotPatchCheck(DWORD Address);
	void WriteBackJMP(DWORD Address);
	void AntiHook(DWORD Address);

public:
	void Init(char *DllName, bool bLoad = false);
	void Hook(char *FunctionName, void *HookFunction, void *NewFunction);
};