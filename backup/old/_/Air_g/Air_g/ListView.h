#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__

#include<Windows.h>
#include<Richedit.h>
#include<CommCtrl.h>
#pragma comment(lib,"comctl32.lib")

namespace Air{
	class ListView{
		private:
			HINSTANCE hMainInstance;
			HWND hMain;
			HWND hwndLV;
			int HeaderCount;
			int ItemCount;

		public:
			ListView();
			bool Init(HWND hWndParent, int X, int Y, int nWidth, int nHeight, bool bMultiSelect = true, bool bGridLine = true);
			bool CreateHeader(LPSTR pszText, int cx);
			bool AddList(LPSTR pszText[]);
			bool Clear();
			LPSTR GetList(int nOffset, int uLine = -1);

	};

}

#endif