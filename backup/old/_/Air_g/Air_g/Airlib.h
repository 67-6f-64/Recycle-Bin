#ifndef __AIRLIB_H__
#define __AIRLIB_H__

#include<Windows.h>

namespace Air{
	bool EmebedWindow(HWND hWndNewParent, LPCSTR lpWindowName, int nWidth = 800, int nHeight = 600);
}

#endif