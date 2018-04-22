#ifndef __GUI_H__
#define __GUI_H__

// bien start
#include <vector>
#include <stdio.h>
#include <stdarg.h>
using namespace std;
// bien end

#define AUTORESIZE 0
#define MaxStringAL 1024 * 4

class gui{
private:
	HWND hWndAW;
	HINSTANCE hInstanceAW;
	void (*createAW)(HWND);
	void (*controlAW)(WORD);
	void (*notifyAW)(HWND);
	void AdjustWindow(int nWidth, int nHeight);
	static void FixCursor(HWND hwnd, UINT uMsg);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//SubWindow
	int GetWidth(LPSTR lpWindowName);
	int GetHeight(LPSTR lpWindowName);
	void SetFont(int nIDDlgItem);

public:
	void Run(HINSTANCE hInstance, LPSTR lpWindowName, int nWidth, int nHeight, void (*create)(HWND) = NULL, void (*control)(WORD) = NULL, void (*notify)(HWND) = NULL);
	void SetHWND(HWND hWnd);
	//SubWindow
	void StaticText(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, int nWidth = AUTORESIZE);
	void CheckBox(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, bool bCheck = false, int nWidth = AUTORESIZE);
	void EditBox(int nIDDlgItem, LPSTR lpWindowName = NULL, int X = 3, int Y = 3, int nWidth = AUTORESIZE, int nLine = 0, bool IsOnlyNumber = false);
	void Button(int nIDDlgItem, LPSTR lpWindowName, int X = 3, int Y = 3, int nWidth = AUTORESIZE);
	bool CheckBoxStatus(int nIDDlgItem);
	void ReadOnly(int nIDDlgItem, bool bFlag = true);
	void SetString(int nIDDlgItem, LPCSTR lpString);

	void GetString(int nIDDlgItem, char *s);
};

// bien start
typedef struct{
	vector<string> item;
}LIST_ITEM;
// bien end

class list{
private:
	HWND hWndAL;
	int HeaderCount;
	int CurrentLine;

public:
	list();
	void Set(HWND hWndParent, int X, int Y, int nWidth = 394, int nHeight = 294);
	void AddHeader(LPSTR pszText, int cx);
	void AddItem(int nItem, LPSTR pszText);
	void Clear();
	void GetSelectedItem(int Index, char *s);
	HWND GetHWND();
// bien
	LIST_ITEM list_item;
	vector<LIST_ITEM> rows;
	void MakeItemList(int itemCnt, ...);
};

#endif