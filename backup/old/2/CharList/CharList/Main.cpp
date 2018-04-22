#include<Windows.h>
#include"gui.h"

BOOL SetClipboard(char *s){
	int length = strlen(s) + 1;
	void *gmem;
	HANDLE hMem;

	hMem = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, strlen(s) + 1);
	if (!hMem) return FALSE;

	gmem = GlobalLock(hMem);
	if(gmem){
		memcpy(gmem, s, length);
		GlobalUnlock(hMem);
		if(OpenClipboard(NULL)){
			EmptyClipboard();
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
			return TRUE;
		}
	}
	return FALSE;
}

#define B_PREV 101
#define B_NEXT 102
#define B_RESET 103
#define S_NAME 201

gui cl;

void cl_create(HWND hWnd){
	cl.StaticText(S_NAME, "            ", 3, 3);
	cl.Button(B_PREV, "Prev", 3, 20);
	cl.Button(B_NEXT, "Next", 60, 20);
	cl.Button(B_RESET, "Reset", 120, 20);
}

char charlist[0x100][0x10] = {0};
int clptr = 0;

void cl_control(WORD wID){
	switch(wID){
	case B_PREV:
		if(clptr > 1){
			clptr--;
			cl.SetString(S_NAME, charlist[clptr]);
			SetClipboard(charlist[clptr]);
		}
		break;
	case B_NEXT:
		if(clptr < 0x100){
			clptr++;
			cl.SetString(S_NAME, charlist[clptr]);
			SetClipboard(charlist[clptr]);
		}
		break;
	case B_RESET:
		clptr = 0;
		cl.SetString(S_NAME, charlist[clptr]);
		SetClipboard(charlist[clptr]);
		break;
	default:
		break;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	if(lpCmdLine[0]){
		int i = 0;
		FILE *fp;
		fp = fopen(lpCmdLine, "r");
		while(fgets(charlist[i], 0x10, fp) != NULL){
			int j;
			for(j=0; charlist[i][j]; j++){
				if(charlist[i][j] == '\r' || charlist[i][j] == '\n'){
					charlist[i][j] = 0;
				}
			}
			if(charlist[i][0]){
				i++;
			}
		}
		fclose(fp);
	}
	cl.Run(hInstance, "CharList", 200, 150, cl_create, cl_control);
	return 0;
}