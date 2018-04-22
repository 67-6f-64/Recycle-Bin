#pragma once
#include<Windows.h>
#include<Richedit.h>
#include<CommCtrl.h>
#pragma comment(lib,"comctl32.lib")


HWND CreateListControl(HWND hwnd, int X = 0, int Y = 0, int Width = 400, int Height = 300, bool GR = true, bool RS = true);
void CreateListControlHeader(HWND hwndLV);