#ifndef __AIRWINDOW_H__
#define __AIRWINDOW_H__

/*
	AirWindow - Win32 Simple GUI Library
	by Riremito
*/

#include<Windows.h>
#include<Richedit.h>
#include<CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

#if defined _M_IX86
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#define AUTORESIZE 0
#define MaxString 1024
#define MaxStringAL 1024*4

#define STATICTEXT_TEST 100
#define CHECKBOX_TEST 200
#define BUTTON_TEST 300
#define EDITBOX_TEST 400

//AirIO.cpp
namespace Air{
	extern HFONT AWFont;
	void _stdcall AirPrint(char *Output, char *Format, ...);
	void _AirPrint(char *Output, char *Format, DWORD *Input);
}

//AirMW.cpp and AirSW.cpp
class AirWindow{
	private:
		//MainWindow
		HINSTANCE hInstanceAW;
		HWND hWndAW;
		char lpClassNameAW[MaxString];

		void AdjustWindow(int nWidth, int nHeight);

		//SubWindow
		void SetFont(int nIDDlgItem);

	public:
		//MainWindow
		AirWindow();
		void SetData(HINSTANCE hInstance, HWND hWnd);
		void Init(HINSTANCE hInstance, LPSTR lpWindowName, int nWidth = 400, int nHeight = 300, bool IsMainWindow = true, void (*Create)(HWND) = NULL, void (*Control)(WORD) = NULL, void (*Notify)() = NULL);//MW
		void UnInit();
		void Show();

		//SubWindow
		void StaticText(int nIDDlgItem, LPSTR lpWindowName, int X = 3, int Y = 3, int nWidth = AUTORESIZE);
		void CheckBox(int nIDDlgItem, LPSTR lpWindowName, int X = 3, int Y = 3, bool bCheck  = false, int nWidth = AUTORESIZE);
		void Button(int nIDDlgItem, LPSTR lpWindowName, int X = 3, int Y = 3, int nWidth = AUTORESIZE);
		void EditBox(int nIDDlgItem, LPSTR lpWindowName = NULL, int X = 3, int Y = 3, int nWidth = AUTORESIZE, int nLine = 0, bool IsOnlyNumber = false);

		bool CheckBoxStatus(int nIDDlgItem);
		void SetString(int nIDDlgItem, LPCSTR lpString);
		void AddString(int nIDDlgItem, LPCSTR lpNewString);
		void ReadOnly(int nIDDlgItem, bool bFlag = true);
		LPSTR GetString(int nIDDlgItem);
		void _stdcall AddFormatString(int nIDDlgItem, char *Format, ...);
		void _stdcall SetFormatString(int nIDDlgItem, char *Format, ...);
};

class AirList{
	private:
		HWND hWndAL;
		int HeaderCount;
		int CurrentLine;
		int CurrentRow;
		int CurrentCol;
		int SelectedRow;
		int SelectedCol;

	public:
		AirList();
		void Init(HWND hWndParent, int X = 3, int Y = 3, int nWidth = 394, int nHeight = 294);
		void AddHeader(LPSTR pszText, int cx);
		void NewLine();
		void Clear();
		void DeleteSelectedItem();
		void AddItem(LPSTR pszText);
		void _stdcall AddFormatItem(char *Format, ...);
		HWND GetHWND();
		LPSTR GetSelectedItem(int Index);
		LPSTR _GetSelectedItem(int Index);
};

#endif