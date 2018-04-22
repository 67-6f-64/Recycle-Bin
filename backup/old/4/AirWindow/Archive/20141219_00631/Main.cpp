#include"AirWindow.h"

AirWindow AW;

#define EDITBOX_AW 1337
#define CHECKBOX_AW 124
#define BUTTON_AW 235
#define STATICTEXT_AW 998
#define EDITBOX_AW2 837
#define BUTTON_AW2 813

void Create(HWND hWnd){
	AW.EditBox(EDITBOX_AW, "", 3, 3, 394, 10);
	AW.CheckBox(CHECKBOX_AW, "テスト", 3, 200);
	AW.Button(BUTTON_AW, "クリア", 100, 200);
	AW.StaticText(STATICTEXT_AW, "AirWindow v2", 3, 280);
	AW.EditBox(EDITBOX_AW2, "", 3, 250, 200);
	AW.Button(BUTTON_AW2, "追加", 3 + 200 + 10, 250);

	AW.ReadOnly(EDITBOX_AW, true);
	
	BYTE Data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	AW.AddString(EDITBOX_AW, "AddFormatString Test\r\n");
	AW.AddFormatString(EDITBOX_AW, "BYTE = %b, WORD = %w, DWORD = %d\r\n", 0x12, 0x1234, 0x12345678);
	AW.AddFormatString(EDITBOX_AW, "xBYTE = %xb, xWORD = %xw, xDWORD = %xd\r\n", 0x12, 0x1234, 0x12345678);
	AW.AddFormatString(EDITBOX_AW, "String = %s, Data = %a\r\n", "文字列", Data, sizeof(Data));
	AW.AddFormatString(EDITBOX_AW, "xString = %xs, xData = %xa\r\n", "文字列", Data, sizeof(Data));
}

void Control(WORD wID){
	switch(wID){
		case CHECKBOX_AW:
			if(AW.CheckBoxStatus(wID)){
				AW.AddString(EDITBOX_AW, "チェックがされました\r\n");
			}
			else{
				AW.AddString(EDITBOX_AW, "チェックが解除されました\r\n");
			}
			break;

		case BUTTON_AW:
			AW.SetString(EDITBOX_AW, NULL);
			break;

		case BUTTON_AW2:
			AW.AddString(EDITBOX_AW, AW.GetString(EDITBOX_AW2));
			break;

		default:
			break;
	}
}


AirList AL;

void CloseTest(){
	Sleep(3000);
	AW.UnInit();
}

void Create2(HWND hWnd){
	AL.Init(hWnd, 3, 3, 394, 274);
	AL.AddHeader("Header1", 100);
	AL.AddHeader("Header2", 100);
	AL.AddHeader("Header3", 180);
	AL.AddItem("Test1");
	AL.AddItem("Test2");
	AL.AddItem("Test3");
	AL.AddFormatItem("%s", "IN");
	AL.AddFormatItem("%d", 0x12345678);
	AL.AddFormatItem("%w %d", 0x9876, 0x21414);
	AL.AddFormatItem("%i te", 1337);
	AL.AddFormatItem("%i te", -1337);

	AW.EditBox(EDITBOX_TEST, NULL, 3, 280, 200);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CloseTest, NULL, NULL, NULL);
}

void Control2(WORD wID){
	switch(wID){
		default:
			break;
	}
}


void Notify2(){
	HWND hWnd = GetFocus();
	char *Buffer;

	if(hWnd == AL.GetHWND()){
		Buffer = AL.GetSelectedItem(1);
		if(Buffer){
			AW.SetString(EDITBOX_TEST, Buffer);
		}
	}
}


//PE

#define MaxPacketSize 1024

AirWindow AP;

bool bSHook = true, bRHook = true;

void _stdcall SendLog(BYTE Packet[], DWORD dwSize, DWORD dwReturn){
	if(!bSHook){
		return;
	}
	AL.AddFormatItem("%s", "OUT");
	AL.AddFormatItem("%d", dwReturn);
	AL.AddFormatItem("%i", dwSize);
	if(dwSize < MaxPacketSize){
		AL.AddFormatItem("%xa", Packet, dwSize);
	}
	else{
		AL.AddFormatItem("%xa %s", Packet, 2, "(省略されました)");
	}
}

void _stdcall RecvLog(BYTE Packet[], DWORD dwSize, DWORD dwReturn){
	if(!bRHook){
		return;
	}
	if(dwSize < 6){
		return;
	}
	dwSize -= 4;
	AL.AddFormatItem("%s", "IN");
	AL.AddFormatItem("%d", dwReturn);
	AL.AddFormatItem("%i", dwSize);
	if(dwSize < MaxPacketSize){
		AL.AddFormatItem("%xa", Packet+4, dwSize);
	}
	else{
		AL.AddFormatItem("%xa %s", Packet+4, 2, "(省略されました)");
	}
}

#define EDIT_PACKET 1000
#define EDIT_RETURN 1001
#define CHECK_HOOKS 2000
#define CHECK_HOOKR 2001
#define BUTTON_CLEAR 3000

void PECreate(HWND hWnd){
	AL.Init(hWnd, 3, 3, 494, 294);
	AL.AddHeader("Type", 40);
	AL.AddHeader("Return", 60);
	AL.AddHeader("Size", 40);
	AL.AddHeader("Packet", 330);
	AP.EditBox(EDIT_PACKET, NULL, 3, 320, 400);
	AP.EditBox(EDIT_RETURN, NULL, 3, 300, 80);
	AP.CheckBox(CHECK_HOOKS, "SendHook", 100, 300, true);
	AP.CheckBox(CHECK_HOOKR, "RecvHook", 200, 300, true);
	AP.Button(BUTTON_CLEAR, "Clear", 450, 300);
}

void PEControl(WORD wID){
	switch(wID){
		case BUTTON_CLEAR:
			AL.Clear();
			break;
		case CHECK_HOOKS:
			bSHook = AP.CheckBoxStatus(wID);
			break;
		case CHECK_HOOKR:
			bRHook = AP.CheckBoxStatus(wID);
			break;
		default:
			break;
	}
}


void PENotify(){
	HWND hWnd = GetFocus();
	char *Buffer, *Buffer2;

	if(hWnd == AL.GetHWND()){
		Buffer = AL.GetSelectedItem(3);
		if(Buffer){
			Buffer2 = AL._GetSelectedItem(1);
			AP.SetString(EDIT_PACKET, Buffer);
			AP.SetString(EDIT_RETURN, Buffer2);
		}
	}
	
}
//

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	AP.Init(hInstance, "Air Packet Logger", 600, 400, true, PECreate, PEControl, PENotify);
	//AW.Init(hInstance, "AirWindow", 400, 300, true, Create, Control);
	//AW.Init(hInstance, "AirWindow", 400, 300, true, Create2, Control2, Notify2);


	return 0;
}
