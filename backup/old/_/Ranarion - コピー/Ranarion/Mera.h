#pragma once
#include<Windows.h>
#include<Richedit.h>

void CursorAnimationFixer(HWND hwnd, UINT msg);
void CreateEditControl(HWND hwnd, int Index, char *Value, int X = 0, int Y = 0, int Width = 100);
void CreateStaticControl(HWND hwnd, int Index, char *Value, int X, int Y);
void CreatePushButtonControl(HWND hwnd, int Index, char *Value, int X, int Y);
void CreateCheckBoxControl(HWND hwnd, int Index, char *Value, int X, int Y);
void CreateNumberEditControl(HWND hwnd, int Index, char *Value, int X, int Y, int Width = 100);
void CreateMultiLineEditControl(HWND hwnd, int Index, char *Value, int X, int Y, int Width = 100, int VisibleLine = 4);

void ScrollMultiLineEdit(HWND hwnd, int nIDDlgItem, WPARAM wParam = SB_BOTTOM);
void AddNewLine(HWND hwnd, int nIDDlgItem, char *NewValue);