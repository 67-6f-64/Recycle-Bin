#pragma once
#include<Windows.h>
#include<Richedit.h>

void CursorAnimationFixer(HWND hwnd, UINT msg);
void CreateEditControl(HWND hwnd, int Index, char *Value, int X = 0, int Y = 0, int Width = 100);
void CreateStaticControl(HWND hwnd, int Index, char *Value, int X = 0, int Y = 0);
void CreatePushButtonControl(HWND hwnd, int Index, char *Value, int X = 0, int Y = 0);
void CreateCheckBoxControl(HWND hwnd, int Index, char *Value, int X = 0, int Y = 0);
void CreateNumberEditControl(HWND hwnd, int Index, char *Value, int X = 0, int Y = 0, int Width = 100);
void CreateMultiLineEditControl(HWND hwnd, int Index, char *Value, int X = 0, int Y = 0, int Width = 100, int VisibleLine = 4);

void ScrollMultiLineEdit(HWND hwnd, int nIDDlgItem, WPARAM wParam = SB_BOTTOM);
void EditControlAddString(HWND hwnd, int nIDDlgItem, char *NewValue, bool NL = true);
void SetFont(HWND hwnd);