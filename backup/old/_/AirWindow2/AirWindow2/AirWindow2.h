#ifndef __AIRWINDOW2_H__
#define __AIRWINDOW2_H__

#include<Windows.h>
#include<Richedit.h>
#include<CommCtrl.h>
#pragma comment(lib,"comctl32.lib")

namespace Air{
	//Main Window
	void CreateMainWindow(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpWindowName, int iWidth, int iHeight, void (*Creates)(HINSTANCE, HWND) = NULL, void (*Commands)(HINSTANCE, HWND, WORD) = NULL, void (*Notifies)(HINSTANCE, HWND) = NULL);

	void CreateMainWindow2(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpWindowName, int iWidth, int iHeight, void (*Creates)(HINSTANCE, HWND) = NULL, void (*Commands)(HINSTANCE, HWND, WORD) = NULL, void (*Notifies)(HINSTANCE, HWND) = NULL);


	//SubWindow
	bool CreateCheckBox(HINSTANCE hInstance, HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, bool bCheck = false, int uWidth = 0);
}








#endif