#include"asdf.h"
#include"agadd.h"

int APL = -1;
void  _stdcall AddPacketList(HWND PacketList){
	APL++;

	LVITEM item;
	item.mask = LVIF_TEXT;

	item.pszText = "Send";
	item.iItem = APL;
	item.iSubItem = 0;
	ListView_InsertItem(PacketList , &item);

	item.pszText = "01234567";
	item.iItem = APL;
	item.iSubItem = 1;
	ListView_SetItem(PacketList, &item);

	item.pszText = "10";
	item.iItem = APL;
	item.iSubItem = 2;
	ListView_SetItem(PacketList, &item);

	item.pszText = "01 02 03 04 05 06 07 08 09 0A";//_Packet;
	item.iItem = APL;
	item.iSubItem = 3;
	ListView_SetItem(PacketList, &item);

	//Scroll to Bottom
	ListView_EnsureVisible(PacketList, APL, TRUE);
}

void InitPacketList(HWND PacketList){	
	LVCOLUMN col;
	col.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM |LVCF_WIDTH;//LVCF_WIDTH
	col.fmt = LVCFMT_LEFT | LVCFMT_FIXED_WIDTH;

	/*
	col.cx = 40;
	col.pszText = TEXT("Type");
	col.iSubItem = 0;
	ListView_InsertColumn(PacketList , 0, &col);

	col.cx = 70;
	col.pszText = TEXT("retAddr");
	col.iSubItem = 1;
	ListView_InsertColumn(PacketList, 1, &col);

	col.cx = 40;
	col.pszText = TEXT("Size");
	col.iSubItem = 2;
	ListView_InsertColumn(PacketList , 2, &col);

	col.cx = 250;
	col.pszText = TEXT("Packets");
	col.iSubItem = 3;
	ListView_InsertColumn(PacketList, 3, &col);
	*/
	col.cx = 40;
	col.pszText = TEXT("Type");
	col.iSubItem = 1;
	ListView_InsertColumn(PacketList , 1, &col);

	col.cx = 70;
	col.pszText = TEXT("retAddr");
	col.iSubItem = 2;
	ListView_InsertColumn(PacketList, 2, &col);

	col.cx = 40;
	col.pszText = TEXT("Size");
	col.iSubItem = 3;
	ListView_InsertColumn(PacketList , 3, &col);

	col.cx = 250;
	col.pszText = TEXT("Packets");
	col.iSubItem = 4;
	ListView_InsertColumn(PacketList, 4, &col);
}

HWND CreateListView (HWND hwndParent) 
{
    INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    RECT rcClient;                       // The parent window's client area.

    GetClientRect (hwndParent, &rcClient); 

    // Create the list-view window in report view with label editing enabled.
	
    HWND hWndListView = CreateWindow(WC_LISTVIEW, 
                                     NULL,
                                      LVS_REPORT | WS_CHILD |LVS_NOSORTHEADER |WS_BORDER,
                                     0, 0,
                                     400,
                                     300,
                                     hwndParent,
                                     NULL,
                                     NULL,
                                     NULL);

	
	//HWND hWndListView = CreateWindowA();
	//| LVS_EX_GRIDLINES
   // HWND hWndListView = CreateWindowExA(NULL, WC_LISTVIEW, NULL, LVS_REPORT | WS_CHILD  | LVS_EDITLABELS, 0, 0, 400, 300, hwndParent, NULL, NULL, NULL);
/*long value = GetWindowLong(hWndListView,GWL_EXSTYLE);
value &=~WS_EX_OVERLAPPEDWINDOW;
value &=~WS_EX_DLGMODALFRAME;
value |=WS_EX_CLIENTEDGE;
SetWindowLong(hWndListView,GWL_EXSTYLE,value);
*/
	SendMessage(hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyle(hWndListView,ListView_GetExtendedListViewStyle(hWndListView) | LVS_EX_GRIDLINES);// | LVS_EX_CHECKBOXES
	//InitPacketList(hWndListView);
	ListHeader ListHeader[5];
	ListHeader[0].str ="tes4t";
	ListHeader[0].width =40;
	ListHeader[1].str ="awe2w";
	ListHeader[1].width =80;
	ListHeader[2].str ="rtbb22";
	ListHeader[2].width =120;

	CreateHeader(hWndListView, ListHeader);
	AddPacketList(hWndListView);
	AddPacketList(hWndListView);
	//AddPacketList(hWndListView);
	//AddPacketList(hWndListView);
	//AddPacketList(hWndListView);
	//AddPacketList(hWndListView);
	ShowWindow(hWndListView, SW_SHOW);
    return (hWndListView);
}


LRESULT CALLBACK SimpleGUIProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

  switch(msg)  {
    case WM_CREATE:
		CreateListView(hwnd);
		/*
		CreateControls(hwnd);
		SCR::AlwaysEnable();
		SCR::OtherScript(TRUE);
		CheckDlgButton(hwnd, CHECK_Extra, BST_CHECKED);
		AIF::LoadItemList();
		AIF::ItemFilter(TRUE);
		CheckDlgButton(hwnd, CHECK_ItemFilter, BST_CHECKED);
		AIF::LoadItemList2();
		AIF::ItemFilter2(TRUE);
		CheckDlgButton(hwnd, CHECK_MobFilter, BST_CHECKED);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Kami::CCTimer, NULL ,NULL ,NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Kami::BreathTimer, NULL ,NULL ,NULL);
		*/
		break;


	case WM_PAINT:
		break;


	case WM_COMMAND:
		switch(LOWORD(wParam)){
				/*
			case CHECK_KamiVac:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					//checked
					KamiVacSettings(hwnd, TRUE);
					Kami::WRYYY(TRUE);
				}else{
					//unchecked
					KamiVacSettings(hwnd, FALSE);
					Kami::WRYYY(FALSE);
				}
				break;
				

			case CHECK_Extra:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					//checked
					SCR::OtherScript(TRUE);
				}else{
					//unchecked
					SCR::OtherScript(FALSE);
				}
				break;
				

			case CHECK_ItemFilter:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					AIF::LoadItemList();
					AIF::ItemFilter(TRUE);
				}else{
					AIF::ItemFilter(FALSE);
				}
				break;


			case CHECK_MobFilter:
				if(IsDlgButtonChecked(hwnd , LOWORD(wParam)) == BST_CHECKED){
					AIF::LoadItemList2();
					AIF::ItemFilter2(TRUE);
				}else{
					AIF::ItemFilter2(FALSE);
				}
				break;
				*/



  }
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	case WM_CLOSE:
		break;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}



HWND CreateNewDialog(HINSTANCE hInstance, char Title[], int nWidth, int nHeight, WNDPROC lpfnWndProc){
	WNDCLASSEX wc;
	HWND hwnd;
	RECT W_RECT, C_RECT;
	int V_WIDTH, V_HEIGHT;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//ウィンドウ情報
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = lpfnWndProc;
	wc.lpszClassName = Title;
	wc.style		= CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground	= (HBRUSH)COLOR_WINDOW;
	//wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(NULL));
	//wc.hIconSm=       LoadIcon(hInstance, MAKEINTRESOURCEA(NULL));

	if(!RegisterClassExA(&wc)) return NULL;

	//ウィンドウ作成
	hwnd = CreateWindowExA(NULL, Title, Title, (WS_EX_LAYERED | WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight, NULL, NULL, hInstance, NULL);
	//ウィンドウ領域計算
	GetWindowRect(hwnd, &W_RECT);
	GetClientRect(hwnd, &C_RECT);
	V_WIDTH	= (W_RECT.right - W_RECT.left) - (C_RECT.right - C_RECT.left);
	V_HEIGHT = (W_RECT.bottom - W_RECT.top) - (C_RECT.bottom - C_RECT.top);
	//サイズ変更
	SetWindowPos(hwnd, HWND_TOP, NULL, NULL, (nWidth + V_WIDTH), (nHeight + V_HEIGHT), SWP_NOMOVE);
	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

	return hwnd;
}










void test(){
	MSG msg;

	CreateNewDialog(NULL, "ATest", 800, 600, SimpleGUIProc);

	//閉じないようにループ
	while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}