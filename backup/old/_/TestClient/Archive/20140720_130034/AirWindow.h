#ifndef __AIRWINDOW_H__
#define __AIRWINDOW_H__

#include<Windows.h>
#include<Richedit.h>
#include<CommCtrl.h>
#pragma comment(lib,"comctl32.lib")

#define MaxWindowCount 128
#define MaxString 1024*3
#define MaxHeader 8

namespace Air{
	//MainWindow
	void WaitForQuit();
	void WaitForSeconds(DWORD dwTime);
	LRESULT CALLBACK DefaultProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void DefaultGuiCreation(HWND hwnd);
	void DefaultGuiControl(WORD wID);
	void DefaultGuiNotify(HWND hWnd);

	//Function
	void DWORDtoChar(char *temp, DWORD dw);
	void WORDtoChar(char *temp, WORD w);
	void BYTEtoChar(char *temp, BYTE b);
	void BYTEArraytoString(char *temp, DWORD dwSize, BYTE Array[]);
	void INTtoChar(char *temp, DWORD d);
	int GetStringLength(char *_String);
	void _stdcall _CreateFormatString(char *_Output, char *_Format, DWORD *dwArgument);
	void _stdcall CreateFormatString(char *_Output, char *_Format, ...);

	//Debug
	bool IsDebug();
	void DebugWindow(HINSTANCE hInstance);
	void DebugMessage(char *_Format, ...);
}

class AirWindow{
	private:
		HINSTANCE hInstanceAW;
		HWND hWndAW;
		bool IsBaseWindow;

		//MainWindow
		void AdjustWindowSize(int nWidth, int nHeight);
		void SetLayer(BYTE bAlpha);

		//SubWindow
		int GetWidthByString(LPCSTR lpWindowName);
		void SetFont(int nIDDlgItem);

		//WindowFunction
		void _stdcall _AddFormatString(int nIDDlgItem, char *_Format, DWORD *dwArgument);

	public:
		AirWindow();
		//MainWindow
		bool CreateSimpleWindow(HINSTANCE hInstance, LPCSTR lpWindowName, int nWidth, int nHeight, void (*GuiCreation)(HWND) = NULL, void(*GuiControl)(WORD) = NULL, void(*GuiNotify)(HWND) = NULL, bool bMain = false, BYTE bAlpha = 0, bool bShow = true);
		void CloseSimpleWindow();
		HWND GetHWND();

		//SubWindow
		bool CreateCheckBox(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, bool bCheck = false, int uWidth = 0);
		bool CreateButton(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);
		bool CreateEditBox(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int nWidth = 100, bool bNumber = false, int nLine = 1);
		bool CreateStaticText(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);

		//WindowFunction
		void AddString(int nIDDlgItem, LPCSTR lpNewString, bool bScroll = true);
		void SetString(int nIDDlgItem, LPCSTR lpString);
		void ReadOnly(int nIDDlgItem, bool bFlag = true);
		LPSTR GetString(int nIDDlgItem);
		void _stdcall AddFormatString(int nIDDlgItem, char *_Format, ...);
};


class AirList{
	private:
		HWND hWndAL;
		int HeaderCount;
		int ItemCount;
		int LastSelectedLine;

		void _stdcall _AddItem(char *_Format, DWORD *dwArgument);

	public:
		AirList();
		bool CreateListControl(HWND hWndParent, int X, int Y, int nWidth, int nHeight, bool bMS = true, bool bGL = true);
		HWND GetHWND();
		void AddHeader(LPSTR pszText, int cx);
		void _stdcall AddItem(char *_Format, ...);
		void ClearAll();
		void GetSelected(char *Data, int Index);
		void GetItem(char *Data, int Line, int Index);
};


#endif