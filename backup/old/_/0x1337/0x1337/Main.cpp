#include<Windows.h>
#include"Hyado.h"
#include"Mera.h"
#include"ListWindow.h"


#define DefaultLVStyle LVS_REPORT|WS_CHILD|LVS_NOSORTHEADER|WS_BORDER

class SimpleLV{
	private:
		HWND hwndLV;
		int HeaderCount;
		int ItemCount;
		int ItemOffset;

	public:
		SimpleLV();
		//~SimpleLV();
		bool InitLV(HWND hwnd, int X, int Y, int Width, int Height);
		void CreateHeader(char *pszText, int cx);
		void AddList(char *pszText);

};

SimpleLV::SimpleLV(){
	INITCOMMONCONTROLSEX icex;
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
	HeaderCount = 0;
	ItemCount = 0;
	ItemOffset = 0;
}

/*
SimpleLV::~SimpleLV(){
	//DestroyWindow(hwndLV);
}
*/

void SimpleLV::AddList(char *pszText){
	LVITEMA item;

	item.mask = LVIF_TEXT;
	item.iItem = ItemCount;
	item.iSubItem = ItemOffset;
	item.pszText = pszText;

	if(!ItemOffset){
			ListView_InsertItem(hwndLV, &item);
		}else{
			ListView_SetItem(hwndLV, &item);
		}

	ItemOffset++;

	if(ItemOffset == HeaderCount){
		//自動スクロール
		ListView_EnsureVisible(hwndLV, ItemCount, true);

		ItemOffset = 0;
		ItemCount++;
	}

}

void SimpleLV::CreateHeader(char *pszText, int cx){
	LVCOLUMNA col;
	ZeroMemory(&col, sizeof(col));

	col.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT|LVCFMT_FIXED_WIDTH;
	col.pszText = pszText;
	col.cx = cx;
	col.iSubItem = HeaderCount;

	ListView_InsertColumn(hwndLV, HeaderCount, &col);

	HeaderCount++;
}


bool SimpleLV::InitLV(HWND hWndParent, int X, int Y, int Width, int Height){
	hwndLV = CreateWindowExA(NULL, WC_LISTVIEWA, NULL, DefaultLVStyle, X, Y, Width, Height, hWndParent, NULL, NULL, NULL);
	SetFont(hwndLV);
	if(!hwndLV){
		return false;
	}

	//一行一括選択
	SendMessageA(hwndLV, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	//グリッド線
	ListView_SetExtendedListViewStyle(hwndLV, ListView_GetExtendedListViewStyle(hwndLV)|LVS_EX_GRIDLINES);
	ShowWindow(hwndLV, SW_SHOW);
	return true;
}





void SubRoutine(SimpleLV SimpleLV){
	SimpleLV.AddList("SubRoutine");
}


void CreateSimpleControls(HWND hwnd){

	CreateCheckBoxControl(hwnd, 0x400, "Enable", 3, 600-3-13);
	CreateMultiLineEditControl(hwnd, 0x210, "Test", 3, 3, 300, 20);
	CreateEditControl(hwnd, 0x200, NULL, 3, 3+12*19+16+3+2, 250);
	CreatePushButtonControl(hwnd, 0x300, "Send", 3+250+3, 3+12*19+16+3);

	//HWND hwndLV = CreateListControl(hwnd, 330, 3, 400, 300);
	//SetDlgItemInt(hwnd, 0x200, (DWORD)GetDlgItem(hwndLV, 0), NULL);
	SimpleLV SimpleLV;

	SimpleLV.InitLV(hwnd, 330, 3, 400, 300);

	SimpleLV.CreateHeader("You", 50);
	SimpleLV.CreateHeader("Are", 50);
	SimpleLV.CreateHeader("Faggot", 300);
	
	SimpleLV.AddList("Test1");
	SimpleLV.AddList("Test2");
	SimpleLV.AddList("Test3");
	SimpleLV.AddList("Test4");
	SimpleLV.AddList("Test5");
	SimpleLV.AddList("Test6");
	SimpleLV.AddList("Test7");
	SubRoutine(SimpleLV);

}

char* GetEditText(HWND hwnd, WORD wUID){
	int ELength = SendMessageA(GetDlgItem(hwnd, wUID), WM_GETTEXTLENGTH, NULL, NULL);
	char *Temp = (char *)malloc(ELength+1);

	GetDlgItemTextA(hwnd, wUID, Temp, ELength+1);

	return Temp;
}

void SimpleControlActions(HWND hwnd, WORD wUID){
	switch(wUID){
		case 0x300:
			CreatePushButtonControl(hwnd, 0x300, GetEditText(hwnd,0x200), 3+250+3, 3+12*19+16+3+100);
			EditControlAddString(hwnd, 0x210, GetEditText(hwnd,0x200));
			break;

		default:
			break;
	}
	return;
}

LRESULT CALLBACK SimpleProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			CreateSimpleControls(hwnd);
			break;

		case WM_COMMAND:
			SimpleControlActions(hwnd, LOWORD(wParam));
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
			
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
			CursorAnimationFixer(hwnd, msg);
			break;

		default:
			break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){

	CreateSimpleWindow(hInstance, "0x1337", 800, 600, SimpleProc);

	return 0x1337;
}
