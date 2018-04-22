#ifndef __SUBWINDOW_H__
#define __SUBWINDOW_H__

#include<Windows.h>
#include<Richedit.h>

namespace Air{

	class SubWindow{
		private:
			HWND hMain;
			HINSTANCE hMainInstance;
			HFONT hFont;

			int GetWidthByString(LPCSTR lpWindowName);
			void SetFont(int nIDDlgItem);

		public:
			SubWindow();
			bool Init(HWND hWndParent);
			//contorls related
			bool CreateCheckBox(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);
			bool CreateButton(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);
			bool CreateEdit(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int nWidth = 100, bool bNumber = false, int nLine = 1);
			bool CreateStaticText(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);

			//others
			void AddString(int nIDDlgItem, LPCSTR lpNewString, bool bScroll = true);
			void SetString(int nIDDlgItem, LPCSTR lpString);
			void ReadOnly(int nIDDlgItem, bool bFlag = true);
			LPSTR GetString(int nIDDlgItem);
	};

}

#endif