#include<Windows.h>
#include"memory.h"
#include"Magica.h"
#include"Packet.h"
#include"Action.h"

DWORD get(DWORD dw){
	return *(DWORD *)dw;
}

void set(LPVOID lpv, DWORD dw){
	*(DWORD *)lpv = dw;
}

DWORD getAddress(DWORD dw){
	return *(DWORD *)(dw + 0x01) + dw + 5;
}

DWORD AndroidClass;
DWORD AndroidOffset;
void (__thiscall *_AndroidTalk)(DWORD ecx);

DWORD ClassInventory;
DWORD OffsetInventory;
DWORD OffsetCashItem;
void (__stdcall *_DeleteItem)(DWORD ItemID);
DWORD (__thiscall *_GetItemID)(DWORD ecx);
DWORD (__thiscall *_GetItemCount)(DWORD ecx);

DWORD ClassNPC;
void (__thiscall *_NPCTalk)(DWORD ecx, DWORD KeyCode);
DWORD ClassShop;
void (__thiscall *_CloseShop)(DWORD ecx, DWORD Code);
DWORD OffsetSelected;
DWORD OffsetList;
DWORD OffsetTab;
void (__thiscall *_SellItem)(DWORD ecx, DWORD ClickType);
void (__thiscall *_SelectTab)(DWORD ecx, DWORD Tab);

ULONGLONG (__thiscall *_GetMesoValue)(DWORD ecx);

//inline
DWORD ClassMiniMap;
DWORD OffsetMapID;

//My functions
bool ShopCheck(){
	DWORD Address = ClassShop;
	Address = *(DWORD *)Address;
	if(Address){
		return true;
	}
	else{
		return false;
	}
}

void CloseShop(){
	_CloseShop(*(DWORD *)ClassShop, 0x02);
	gui_update_action(ACTION_CLOSINGSHOP);
}

void ExitCloseShop(){
	_CloseShop(*(DWORD *)ClassShop, 0x02);
	gui_update_action(ACTION_EXIT_CLOSINGSHOP);
}

void SellItem(){
	DWORD Address;
	Address = get(ClassShop);
	_SelectTab(get(Address + OffsetTab), 0x01);//è¡îÔóì
	if(get(Address + OffsetList)){
		*(DWORD *)(Address + OffsetSelected) = 0;//àÍî‘ñ⁄ÇîÑãp
		_SellItem(Address, 0x200);//Left Click
	}
	else{
		gui_update_action(ACTION_CLOSESHOP);
	}
}

bool IsUseInventoryFull(){
	DWORD Address;
	DWORD SlotMax, Slot;
	DWORD ItemInfo;

	Address = get(get(get(ClassInventory) + OffsetInventory) + OffsetCashItem - 0x0C);//Use Item
	SlotMax = get(Address - 0x04);//Number of Inventory Slots
	Address = Address + 0x04;
	for(Slot=1; Slot<SlotMax; Slot++){
		ItemInfo = get(Address + Slot * 0x08);
		if(!ItemInfo){
			return false;
		}
	}
	gui_update_action(ACTION_OPENSHOP);
	return true;
}

void DeleteUnwantedItems(){
	DWORD Address;
	DWORD SlotMax, Slot;
	DWORD ItemInfo, ItemID;
	Address = get(get(get(ClassInventory) + OffsetInventory) + OffsetCashItem - 0x0C);//Use Item
	SlotMax = get(Address - 0x04);//Number of Inventory Slots
	Address = Address + 0x04;

	for(Slot=1; Slot<SlotMax; Slot++){
		ItemInfo = get(Address + Slot * 0x08);
		if(ItemInfo){
			if(_GetItemCount(ItemInfo) == 1){
				_DeleteItem(_GetItemID(ItemInfo + 0x10));
			}
		}
	}
	
	Address = get(get(get(ClassInventory) + OffsetInventory) + OffsetCashItem - 0x10);//Equip -10
	SlotMax = get(Address - 0x04);//Number of Inventory Slots
	Address = Address + 0x04;

	for(Slot=1; Slot<SlotMax; Slot++){
		ItemInfo = get(Address + Slot * 0x08);
		if(ItemInfo){
			ItemID = _GetItemID(ItemInfo + 0x10);
			if(ItemID == 1002662){
				_DeleteItem(ItemID);
			}
		}
	}
}

void MesoCheck(){
	ULONGLONG Meso;
	Meso = _GetMesoValue(get((get(ClassInventory) + OffsetInventory)));
	if(Meso > 9990000000){
		gui_update_action(ACTION_MESOMAX);
	}
	gui_update_meso(Meso);
}

ULONGLONG MesoPlz(){
	return _GetMesoValue(get((get(ClassInventory) + OffsetInventory)));
}

DWORD __cdecl MesoMax(LPVOID lpv1){
	gui_update_meso(MesoPlz());
	gui_update_action(ACTION_MESOMAX);
	return 1;
}

//
bool bMagica = false;

//v355.1
#include"Packet.h"
DWORD Header_NPCTalk;
void (__thiscall *_OpenNPCTalk)(DWORD ecx, CInPacket *p);
void __fastcall OpenNPCTalk_Hook(DWORD ecx, DWORD edx, CInPacket *p){
	if(*(DWORD *)&p->Packet[4 + 3] == 9110205){
		BYTE Packet[4];
		COutPacket ntp;
		*(WORD *)&Packet[0] = Header_NPCTalk;
		Packet[2] = 0x00;
		Packet[3] = 0xFF;
		ntp.Length = 4;
		ntp.Packet = Packet;
		__SendPacket((LPVOID)*_CClientSocket, &ntp);
		return;
	}
	return _OpenNPCTalk(ecx, p);
}

//v356.0
#include<stdio.h>
char charlist[1024][0x10] = {0};
int xptr = 0;
bool bMagicaK = false;
void LoadCharList(){
	int i = 0;
	FILE *fp;
	if(!bMagicaK){
		fp = fopen("charlist.txt", "r");
	}
	else{
		fp = fopen("charlist2.txt", "r");
	}
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

void WriteLog(){
	FILE *fp;
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	if(!bMagicaK){
		fp = fopen("deliverylog.txt", "a");
	}
	else{
		fp = fopen("deliverylog2.txt", "a");
	}
	fprintf(fp, "%02d/%02d %02d:%02d %s\n", stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, charlist[xptr]);
	fclose(fp);
}

void delistart();
DWORD Class_DeliList;
DWORD (__thiscall *_CloseDelivery)(DWORD ecx);
void (__thiscall *_ReDeli)(DWORD ecx);
void (__thiscall *_DeliClearName)(DWORD ecx);
void (__cdecl *_Deli)(DWORD State);


#include"Client.h"

void __cdecl Deli_Hook(DWORD State){
	switch(State){
	case 0x14://sent
		gui_update_name(charlist[xptr]);
		SendDeliveryLog(charlist[xptr]);
		//WriteLog();
		delistart();
		break;
	case 0x11://fill
		xptr++;//next
		if(charlist[xptr][0]){
			delistart();
		}
		else{
			gui_update_action(ACTION_CLOSEDELIVERY);
		}
		break;
	case 0x0D://no meso
		gui_update_action(ACTION_CLOSEDELIVERY);
		break;
	default:
		return _Deli(State);
		break;
	}
}

void delistart(){
	set(LPVOID(get(get(ClassShop) + 0x154) + 0x14), 99999999);//meso!
	DWORD ZXString = (DWORD)get(get(get(get(get(ClassShop) + 0x154) + 0x1C) + 0x138) + 0x48);
	*(DWORD *)(ZXString - 0x04) = strlen(charlist[xptr]);//length
	sprintf((char *)ZXString, "%s", charlist[xptr]);
	_ReDeli(get(get(ClassShop) + 0x154));
}

DWORD Class_Account;
DWORD Offset_MapleID;
void SendState(){
	if(!get(Class_Account)) return;
	SendMesoValue((char *)get(get(Class_Account) + Offset_MapleID), MesoPlz());
}

//
Frame *Client;

void CALLBACK MagicaRush(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

char psOpen[0x100];
WORD H_T = 0;
//v354.2
void magica(){
	memory Maple;
	Maple.scan("Frame", "A1 ?? ?? ?? ?? 80 B8 ?? ?? ?? ?? 00 56 8B F1 0F 84");
	Client = (Frame *)(get(get(Maple.get() + 0x01)));

	//Header
	Maple.scan("Header", "68 ?? ?? ?? ?? 8D 4C 24 ?? E8 ?? ?? ?? ?? 6A 08 8D 4C 24 ?? C7 44 24 ?? 00 00 00 00 E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 85 C9 74 0A 8D 44 24 ?? 50 E8 ?? ?? ?? ?? 8B 16 8B 42 ?? 6A 02 8B CE FF D0 8D 4C 24 ?? C7 44 24 ?? FF FF FF FF E8");
	sprintf(psOpen, "@%04X 00", get(Maple.get() + 0x01));
	H_T = get(Maple.get() + 0x01);
	//Delivery UI
	Maple.scan("Deli UI1", "0F B6 CB F7 D9 1B C9 83 E1 02 51 8B C8 E8", "31 C9 90");

	//Delivery UI2
	Maple.scan("Deli UI2", "E8 ?? ?? ?? ?? 83 FF ?? 75 ?? 8B 86 ?? ?? ?? ?? 2B C5 74 ?? 83 E8 01 75 ?? 8B CE E8 ?? ?? ?? ?? EB ?? 8B CE E8");
	set(&_Deli, getAddress(Maple.get() - 0x0C));
	Maple.call(Maple.get() - 0x0C, (DWORD)Deli_Hook);
	set(&_DeliClearName, getAddress(getAddress(Maple.get() + 0x24) + 0x15));//u-n?
	Maple.write(Maple.get() + 0x24, "90 90 90 90 90");

	//Item Get Log (v355.1)
	Maple.scan("Item Log", "EB 08 57 8B CE E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 85 C9 74 05 E8");
	Maple.write(getAddress(Maple.get() + 0x05), "C2 04 00");

	//NPC talk range
	Maple.scan("NPC talk range", "75 ?? 8B 47 04 8B 0F 50 51 8D 54 24 ?? 52 FF 15 ?? ?? ?? ?? 85 C0 75 ?? 8B 47 04 8B 0F 50 51 8D 54 24 ?? 52 FF 15", "EB");

	//NPC talk speed (v355.1)
	Maple.scan("NPC Talk Speed", "0F 85 ?? ?? ?? ?? 8B 7C 24 ?? 85 FF 0F 84 ?? ?? ?? ?? 8B 43 ?? 8B 50 ?? 8D 73 ?? 8D 4C 24 ?? 51 8B CE FF D2", "90 90 90 90 90 90");

	//Mirror Log
	Maple.scan("Mirror log", "6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B D9 8B 4C 24 ?? 8B 35 ?? ?? ?? ?? E8 ?? ?? ?? ?? 6A 00 8B CE 0F B6 F8 E8", "C2 04 00");

	//Store Blue Box1
	Maple.scan("blue box1", "E8 ?? ?? ?? ?? 8B 4C 24 ?? 89 44 24 ?? C6 44 24 ?? 00 85 C9 74", "83 C4 0C 8B C6");


	//Android (v355.1)
	Maple.scan("Android Talk", "6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 E8 ?? ?? ?? ?? 0F B6 C0 50 E8");//Android Talk
	set(&_AndroidTalk, Maple.get());
	Maple.scan("AndroidClass", "8B 0D ?? ?? ?? ?? 3B CD 74 19 8B 54 24 ?? 8B 44 24 ?? 52 8B 54 24 ?? 50 8B 44 24 ?? 52 50 E8");//Class
	AndroidClass = get(Maple.get() + 0x02);
	Maple.scan("AndroidOffset", "8B 8D ?? ?? ?? ?? 85 C9 74 0F 8B 54 24 ?? 8B 44 24 ?? 52 50 E8");//Offset
	AndroidOffset = get(Maple.get() + 0x02);

	Maple.scan("GetItemID", "E8 ?? ?? ?? ?? 8B C8 B8 83 DE 1B 43");//GetItemID
	set(&_GetItemID, getAddress(Maple.get()));
	

	Maple.scan("DeleteItem", "E8 ?? ?? ?? ?? 8B 44 24 ?? 89 6C 24 ?? 85 C0 74 ?? 83 C0 F4 50 E8 ?? ?? ?? ?? 83 C4 04 B8 01 00 00 00 E9 ?? ?? ?? ?? 8B CE E8");//DeleteItem
	set(&_DeleteItem, getAddress(Maple.get()));

	Maple.scan("GetItemCount", "8B 41 ?? 50 83 C1 ?? 51 E8 ?? ?? ?? ?? 83 C4 08 0F B7 C0 C3");//GetItemCount
	set(&_GetItemCount, Maple.get());
	


	Maple.scan("ClassInventory", "A1 ?? ?? ?? ?? 39 98 ?? ?? ?? ?? 75");//ClassInventory
	ClassInventory = get(Maple.get() + 0x01);
	Maple.scan("OffsetInventory", "8B 8E ?? ?? ?? ?? 8D 44 24 ?? 50 68 ?? ?? ?? ?? 8D 54 24 ?? 52 C6 44 24 ?? 01 E8");//OffsetInventory
	OffsetInventory = get(Maple.get() + 0x02);
	Maple.scan("OffsetCashItem", "8B 87 ?? ?? ?? ?? 85 C0 74 03 8B 40 FC 8D 58 FF BE");//OffsetCashItem
	OffsetCashItem = get(Maple.get() + 0x02);


	Maple.scan("ClassShop", "A3 ?? ?? ?? ?? EB 0A C7 05 ?? ?? ?? ?? 00 00 00 00 C7");//ClassShop
	ClassShop = get(Maple.get() + 0x01);
	Maple.scan("CloseShop", "8B 44 24 ?? 56 8B F1 3D ?? ?? ?? ?? 77");//CloseShop
	set(&_CloseShop, Maple.get());
	Maple.scan("OffsetSelected", "39 86 ?? ?? ?? ?? 0F 85 ?? ?? ?? ?? B9");//OffsetSelected
	OffsetSelected = get(Maple.get() + 0x02);
	Maple.scan("OffsetList", "8B 8E ?? ?? ?? ?? 83 C5 ?? 89 6C 24 ?? 85 C9 74");//OffsetList
	OffsetList = get(Maple.get() + 0x02);
	Maple.scan("OffsetTab", "8B 81 ?? ?? ?? ?? 8B 40 ?? 40 8D 48 ?? 83 F9 ?? 77");//OffsetTab
	OffsetTab = get(Maple.get() + 0x02);
	Maple.scan("SelectTab", "53 8B 5C 24 ?? 56 8B F1 85 DB 7C ?? 3B 5E ?? 7D ?? 55 8B 6E ?? 57 6A 00 89 5E ?? E8");//SelectTab
	set(&_SelectTab, Maple.get());
	Maple.scan("SellItem", "3D E8 03 00 00 75 58 E8 ?? ?? ?? ?? 5E C2 04 00 6A 00 E8");//v356.0
	set(&_SellItem, getAddress(Maple.get() + 0x12));

	Maple.scan("NPC Talk", "6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 8B 3D ?? ?? ?? ?? 85 FF 0F 84 ?? ?? ?? ?? 8D 44 24 ?? 50 8B CF E8 ?? ?? ?? ?? 8D 4C 24 ?? E8");//NPC Talk
	set(&_NPCTalk, Maple.get());
	Maple.scan("ClassNPC", "A1 ?? ?? ?? ?? 3B C3 0F 84 ?? ?? ?? ?? 05 ?? ?? ?? ?? 8B C8 89 44 24 ?? E8");//ClassNPC
	ClassNPC = get(Maple.get() + 0x01);

	Maple.scan("GetMesoValue", "E8 ?? ?? ?? ?? 85 F6 7C ?? 7F");//Meso
	set(&_GetMesoValue, getAddress(Maple.get()));

	Maple.scan("ClassMiniMap", "8B 0D ?? ?? ?? ?? 85 C9 74 05 E8 ?? ?? ?? ?? B8 01 00 00 00 8B");//ClassMiniMap
	ClassMiniMap = get(Maple.get() + 0x02);
	Maple.scan("OffsetMapID", "8B 86 ?? ?? ?? ?? 50 8D 4C 24 ?? 51 8B 0D ?? ?? ?? ?? C6 84 24 ?? ?? ?? ?? 03 E8");//OffsetMapID
	OffsetMapID = get(Maple.get() + 0x02);

	//NPC talk +?
	Maple.scan("OpenNPCTalk", "6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 14 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F9 83 7F ?? 00 74 ?? 68");
	Maple.Hook(Maple.get(), OpenNPCTalk_Hook, &_OpenNPCTalk);
	Maple.scan("Header_NPCTalk", "68 ?? ?? ?? ?? 8D 4C 24 ?? E8 ?? ?? ?? ?? 6A 00 8D 4C 24 ?? C6 44 24 ?? 05 E8");
	Header_NPCTalk = get(Maple.get() + 0x01);

	//takuhai mode5
	Maple.scan("Takuhai", "E8 ?? ?? ?? ?? 83 C4 1C 83 F8 06 0F 85 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8D 4C 24 ?? E8 ?? ?? ?? ?? 6A 03 8D 4C 24 ?? C6 44 24 ?? 06 E8 ?? ?? ?? ?? 0F B6 46 ?? 50 8D 4C 24 ?? E8", "B8 06 00 00 00");
	//takuhai mode5-2
	Maple.scan("takuhai2", "8B 89 ?? ?? ?? ?? E8 ?? ?? ?? ?? C2 08 00 8B 89 ?? ?? ?? ?? E8 ?? ?? ?? ?? C2 08 00 89 54 24 ?? 89 44 24 ?? E9");
	set(&_ReDeli, getAddress(Maple.get() + 0x06));

	//no back ground
	Maple.scan("background", "55 8D 6C 24 ?? 83 EC ?? 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 45 ?? 53 56 57 50 8D 45 ?? 64 A3 00 00 00 00 89 4D ?? 51 8B C4 89 65 ?? 68", "C2 28 00");

	Maple.scan("Class_DeliList", "8B 0D ?? ?? ?? ?? 8B 11 8B 42 ?? 6A 00 FF D0 5E 5F C2 10 00");
	Class_DeliList = get(Maple.get() + 0x02);
	Maple.scan("Offset_DeliList", "FF 24 85 ?? ?? ?? ?? E8 ?? ?? ?? ?? C2 04 00 8B 89 ?? ?? ?? ?? E8 ?? ?? ?? ?? C2 04 00");
	set(&_CloseDelivery, getAddress(Maple.get() + 0x07));
	//

	Maple.scan("MesoMax", "E8 ?? ?? ?? ?? 83 C4 04 8B 4C 24 ?? 64 89 0D 00 00 00 00 59 5F 5E 5D 5B 83 C4 ?? C3 51 8B CC 89 A4 24 ?? ?? ?? ?? 68 ?? ?? ?? ?? E9");
	Maple.call(Maple.get(), (DWORD)MesoMax);

	Maple.scan("Class_Account", "8B 0D ?? ?? ?? ?? 83 B9 ?? ?? ?? ?? 00 C6 44 24 ?? 01 74");
	Class_Account = get(Maple.get() + 0x02);
	Maple.scan("Offset_MapleID", "8B BE ?? ?? ?? ?? 8B 28 52 8B CE C6 44 24 ?? 02 E8");
	Offset_MapleID = get(Maple.get() + 0x02);

	EnablePacketSender();

	while(!Client->hWnd){
		Sleep(50);
	}
	
	SetTimer(Client->hWnd, 1336, 50, ScriptInjector);
	SetTimer(Client->hWnd, 123, 50, MagicaRush);
}

void takuhai(){
		BYTE Packet[4];
		COutPacket ntp;
		*(WORD *)&Packet[0] = H_T;
		Packet[2] = 0x00;
		ntp.Length = 3;
		ntp.Packet = Packet;
		__SendPacket((LPVOID)*_CClientSocket, &ntp);
}

DWORD MagicaRushState = 0;
void CALLBACK MagicaRush(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime){
	__try{
		switch(MesoAction){
		case ACTION_GETREWARD:
			DeleteUnwantedItems();
			if(!IsUseInventoryFull()){
				if(get(ClassNPC)){
					_NPCTalk(get(ClassNPC), 0x2F);
				}
			}
			break;

		case ACTION_OPENSHOP:
			if(ShopCheck() == false){
				_AndroidTalk(get(get(AndroidClass) + AndroidOffset));
			}
			else{
				gui_update_action(ACTION_SELLREWARD);
			}
			break;

		case ACTION_SELLREWARD:
			MesoCheck();
			SellItem();
			break;

		case ACTION_CLOSESHOP:
			CloseShop();
			break;

		case ACTION_CLOSINGSHOP:
			if(ShopCheck() == false){
				gui_update_action(ACTION_GETREWARD);
			}
			break;

		case ACTION_MESOMAX:
			ExitCloseShop();
			break;

		case ACTION_EXIT_CLOSINGSHOP:
			if(ShopCheck() == false){
				//gui_update_action(ACTION_EXIT);
				gui_update_action(ACTION_OPENDELIVERY);
			}
			break;

		case ACTION_OPENDELIVERY:
				takuhai();
				gui_update_action(ACTION_DELIVERY);
			break;

		case ACTION_DELIVERY:
			if(get(ClassShop)){
				gui_update_action(ACTION_DELIVERING);
			}
			break;

		case ACTION_DELIVERING:
			if(get(ClassShop)){
				xptr = 0;//reset
				LoadCharList();
				gui_update_action(ACTION_POSTING);
				delistart();
			}
			break;

		case ACTION_CLOSEDELIVERY:
			_CloseDelivery(get(Class_DeliList));
			if(MesoPlz() > 9990000000){
				gui_update_action(ACTION_EXIT);
			}
			else{
				if(!bMagica){
					gui_update_action(ACTION_DEFAULT);
				}
				else{
					gui_update_action(ACTION_GETREWARD);
				}
			}
			break;
		
		case ACTION_CHARMAX:
			break;

		case ACTION_DEFAULT:
			break;

		default:
			break;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		gui_update_action(ACTION_EXCEPTION);
	}
}