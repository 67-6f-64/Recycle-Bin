#ifndef __AIRWINDOW_H__
#define __AIRWINDOW_H__

#include<Windows.h>
#include<Richedit.h>

class AirWindow{
	private:
		HINSTANCE hInstanceAW;
		HWND hWndAW;
		bool IsBaseWindow;
		void (*GuiControl)(WORD);

		//MainWindow
		void AdjustWindowSize(int nWidth, int nHeight);
		void SetLayer(BYTE bAlpha);
		LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//SubWindow
		int GetWidthByString(LPCSTR lpWindowName);
		void SetFont(int nIDDlgItem);

		//Function
		void DWORDtoChar(char *temp, DWORD dw);
		void WORDtoChar(char *temp, WORD w);
		void BYTEtoChar(char *temp, BYTE b);
		void BYTEArraytoString(char *temp, DWORD dwSize, BYTE Array[]);
		void INTtoChar(char *temp, DWORD d);
		int GetStringLength(char *_String);
		void _stdcall _CreateFormatString(char *_Output, char *_Format, DWORD *dwArgument);
		void _stdcall _AddFormatString(int nIDDlgItem, char *_Format, DWORD *dwArgument);

	public:
		AirWindow();
		//MainWindow
		bool CreateSimpleWindow(HINSTANCE hInstance, LPCSTR lpWindowName, int nWidth = 800, int nHeight = 600, void (*pFunction)(WORD) = NULL, WNDPROC lpfnWndProc = NULL, bool bMain = false, BYTE bAlpha = 0, bool bShow = true);
		LRESULT WINAPI _DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		void WaitForQuit();

		//SubWindow
		bool CreateCheckBox(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, bool bCheck = false, int uWidth = 0);
		bool CreateButton(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);
		bool CreateEditBox(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int nWidth = 100, bool bNumber = false, int nLine = 1);
		bool CreateStaticText(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);

		//Function
		void _stdcall CreateFormatString(char *_Output, char *_Format, ...);
		void AddString(int nIDDlgItem, LPCSTR lpNewString, bool bScroll = true);
		void SetString(int nIDDlgItem, LPCSTR lpString);
		void ReadOnly(int nIDDlgItem, bool bFlag = true);
		LPSTR GetString(int nIDDlgItem);
		void _stdcall AddFormatString(int nIDDlgItem, char *_Format, ...);
};

#endif