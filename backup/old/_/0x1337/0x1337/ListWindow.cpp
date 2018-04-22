#include"ListWindow.h"

#define DefaultListControlStyle LVS_REPORT|WS_CHILD|LVS_NOSORTHEADER|WS_BORDER
#define DefaultListControlHeaderMask LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH
#define DefaultListControlHeaderFmt  LVCFMT_LEFT|LVCFMT_FIXED_WIDTH



typedef struct{
	char *pszText;
	int cx;
}LVHeader;

void CreateListControlHeader2(HWND hwndLV, int Width, LVHeader *LVHeader){
	int i;
	int lastWidth = Width;
	LVCOLUMNA col;

	ZeroMemory(&col, sizeof(col));
	col.mask = DefaultListControlHeaderMask;
	col.fmt = DefaultListControlHeaderFmt;

	for(i=0; i<4; i++){
		col.pszText = LVHeader[i].pszText;
		col.cx = LVHeader[i].cx;
		col.iSubItem = i+1;//for bypassing fixed width problem

		if(!LVHeader[i+1].pszText){
			col.cx = lastWidth;
		}
		lastWidth -= col.cx;
		ListView_InsertColumn(hwndLV, i+1, &col);
	}

}

HWND CreateListControl(HWND hwnd, int X, int Y, int Width, int Height, bool GR, bool RS){
    INITCOMMONCONTROLSEX icex;// Structure for control initialization.
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    HWND hwndLV = CreateWindowExA(NULL, WC_LISTVIEWA, NULL, DefaultListControlStyle, X, Y,Width , Height, hwnd, NULL, NULL, NULL);

	if(RS){
		SendMessageA(hwndLV, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	}

	if(GR){
		ListView_SetExtendedListViewStyle(hwndLV, ListView_GetExtendedListViewStyle(hwndLV)|LVS_EX_GRIDLINES);
	}

	LVHeader LVHeader[0x10];
	ZeroMemory(LVHeader, sizeof(LVHeader));

	LVHeader[0].pszText ="Type";
	LVHeader[1].pszText ="Size";
	LVHeader[2].pszText ="Packet";
	LVHeader[0].cx =40;
	LVHeader[1].cx = 40;
	LVHeader[2].cx = 100;


	CreateListControlHeader2(hwndLV, Width, LVHeader);
	ShowWindow(hwndLV, SW_SHOW);

    return hwndLV;
}


void CreateListControlHeader(HWND hwndLV){	
	LVCOLUMN col;
	ZeroMemory(&col, sizeof(col));
	col.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT|LVCFMT_FIXED_WIDTH;

	//col.cx = 40;

	//col.pszText = "Type";
	//col.iSubItem = 0;
	//ListView_InsertColumn(hwndLV , 0, &col);


	col.cx = 70;
	col.pszText = "retAddr";
	col.iSubItem = 1;
	ListView_InsertColumn(hwndLV, 1, &col);

	col.cx = 40;
	col.pszText = "Size";
	col.iSubItem = 2;
	ListView_InsertColumn(hwndLV , 2, &col);

	col.cx = 250;
	col.pszText = "Packet";
	col.iSubItem = 3;
	ListView_InsertColumn(hwndLV, 3, &col);

}