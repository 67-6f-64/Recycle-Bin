#pragma once
#include<Windows.h>

void AirBypass();

BOOL WINAPI Inject(DWORD processID, wchar_t * sourceDLL);