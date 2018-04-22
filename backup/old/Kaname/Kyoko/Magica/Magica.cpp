#include<Windows.h>
#include"memory.h"
#include"Magica.h"
#include"Packet.h"

bool bMagica = false;
bool bMagica2 = false;

DWORD get(DWORD dw){
	return *(DWORD *)dw;
}

void set(LPVOID lpv, DWORD dw){
	*(DWORD *)lpv = dw;
}

DWORD getAddress(DWORD dw){
	return *(DWORD *)(dw + 0x01) + dw + 5;
}

DWORD Class_MiniMap;
DWORD Offset_MapID;
DWORD Class_Inventory;
DWORD Offset_Inventory;
DWORD Class_NPC;
DWORD Class_UI;
DWORD Class_DeliList;
DWORD Offset_DeliList;
DWORD Class_CC;
DWORD Offset_SelectedCharacter;

ULONGLONG (__thiscall *_GetMesoValue)(DWORD ecx);
DWORD (__thiscall *_GetNPC)(DWORD ecx, DWORD NPCID);
void (__thiscall *_NPCTalk)(DWORD ecx, DWORD dw, DWORD NPC);
void (__thiscall *_SummonNPC)(DWORD ecx, InPacket *p);
void (__cdecl *_OpenDelivery)(InPacket *p);
DWORD (__thiscall *_GetDelivery)(DWORD ecx);
DWORD (__thiscall *_CloseDelivery)(DWORD ecx);
void (__cdecl *_OpenStorage)(InPacket *p);
DWORD (__thiscall *_InMeso)(DWORD ecx);
DWORD (__thiscall *_CharacterChange)(DWORD ecx, DWORD dw);
void (__thiscall *_CharacterSelect)(DWORD ecx, InPacket *p);
void (__thiscall *_SendKey)(DWORD ecx, DWORD KeyCode1, DWORD KeyCode2);
void (__thiscall *_Login)(DWORD ecx, InPacket *p);
void (__thiscall *_Encrypt1)(DWORD ecx, BYTE b);
void (__thiscall *_Encrypt4)(DWORD ecx, DWORD dw);
ULONGLONG (__thiscall *_MesoValueDialog)(DWORD ecx, LPVOID lpv1, LPVOID lpv2, LPVOID lpv3, LPVOID lpv4, LPVOID lpv5);

DWORD Class_Character;
DWORD Offset_TeleToggle;
DWORD Offset_TeleY;
void Teleport(DWORD X, DWORD Y){
	DWORD ecx = Class_Character;
	ecx = get(ecx);
	if(!ecx){
		return;
	}
	_Encrypt1(ecx + Offset_TeleToggle, 0x01);
	_Encrypt4(ecx + Offset_TeleY + 0x0C, X);
	_Encrypt4(ecx + Offset_TeleY, Y);
}

//Benri
ULONGLONG GetMesoValue(){
	return _GetMesoValue(get(get(Class_Inventory) + Offset_Inventory));
}

DWORD GetMapID(){
	if(!get(Class_MiniMap)){
		return 0;
	}
	return get(get(Class_MiniMap) + Offset_MapID);
}

void NPCTalk(DWORD NPCID){
	DWORD NPC;
	NPC =_GetNPC(get(Class_NPC), NPCID);
	if(!NPC){
		return;
	}
	InterlockedIncrement((LPLONG)(NPC + 0x0C));
	_NPCTalk(get(Class_UI), 0, NPC);
}

//Hook
void __fastcall SummonNPC_Hook(DWORD ecx, DWORD edx, InPacket *p){
	_SummonNPC(ecx, p);
	if(!bMagica){
		return;
	}
	if((GetMapID() == 100000200 || GetMapID() == 310000000) && *(DWORD *)&p->Packet[10] == 9010009){
		NPCTalk(9010009);//Delivery
	}
	else if (GetMapID() == 310000000){
		Teleport(0x00000442, 0x0000024A);
	}
}

void _cdecl OpenDelivery_Hook(InPacket *p){
	_OpenDelivery(p);
	if(!bMagica){
		return;
	}
	if(p->Packet[6] == 0x0B || p->Packet[6] == 0x19){//Open or Get
		DWORD dw;
		dw = get(Class_DeliList);
		dw = get(dw + Offset_DeliList);
		*(DWORD *)(dw + 0x04) = 0x00;//First
		if(!_GetDelivery(dw)){
			_CloseDelivery(get(Class_DeliList));
			if(bMagica2){
				_CharacterChange(get(Class_Inventory), 0);
				return;
			}
			if(GetMapID() == 100000200){
				NPCTalk(1012009);//Storage
			}
			else if(GetMapID() == 310000000){
				NPCTalk(2150000);//Storage
			}
		}
	}
}

ULONGLONG __fastcall MesoValueDialog_Hook(DWORD ecx, DWORD edx, LPVOID lpv1, LPVOID lpv2, LPVOID lpv3, LPVOID lpv4, LPVOID lpv5){
	if(!bMagica){
		return _MesoValueDialog(ecx, lpv1, lpv2, lpv3, lpv4, lpv5);
	}
	return GetMesoValue();//max meso
}


void __cdecl OpenStorage_Hook(InPacket *p){
	_OpenStorage(p);
	if(!bMagica){
		return;
	}
	if(GetMapID() == 100000200 || GetMapID() == 310000000){
		if(p->Packet[6] == 0x15){//Open
			if(_InMeso(get(Class_DeliList)) == -1){//no mesos
			_CharacterChange(get(Class_Inventory), 0);
			}
		}
		else if(p->Packet[6] == 0x12){//Meso In
			_CharacterChange(get(Class_Inventory), 0);
		}
		else if(p->Packet[6] == 0x13){//Meso Max
		}
	}
}

DWORD NewLogin = 0;//FALSE

void __fastcall CharacterSelect_Hook(DWORD ecx, DWORD edx, InPacket *p){
	_CharacterSelect(ecx, p);
	if(!bMagica){
		return;
	}
	if(NewLogin == 1){
		for(int i=0; i<40; i++){
			_SendKey(get(ecx + Offset_SelectedCharacter - 0x0C), 0x25,0x014B0000);//Left
		}
		_SendKey(get(ecx + Offset_SelectedCharacter - 0x0C), 0x0D,0x001C0000);//Enter
		NewLogin = 0;
		return;
	}
	DWORD dwSelected = get(ecx + Offset_SelectedCharacter);
	if(dwSelected != -1){
		_SendKey(get(ecx + Offset_SelectedCharacter - 0x0C), 0x27,0x014D0000);//Right
		if(get(ecx + Offset_SelectedCharacter) != dwSelected){
			_SendKey(get(ecx + Offset_SelectedCharacter - 0x0C), 0x0D,0x001C0000);//Enter
		}
	}
}

void __fastcall Login_Hook(DWORD ecx, DWORD edx, InPacket *p){
	_Login(ecx, p);
	NewLogin = 1;//TRUE
}

Frame *Client;
void magica(){
	memory Maple;

	Maple.scan("Frame", "A1 ?? ?? ?? ?? 80 B8 ?? ?? ?? ?? 00 56 8B F1 0F 84");
	Client = (Frame *)(get(get(Maple.get() + 0x01)));

	//Map
	Maple.scan("Class_MiniMap", "8B 0D ?? ?? ?? ?? 85 C9 74 05 E8 ?? ?? ?? ?? B8 01 00 00 00 8B");//ClassMiniMap
	Class_MiniMap = get(Maple.get() + 0x02);
	Maple.scan("Offset_MapID", "8B 86 ?? ?? ?? ?? 50 8D 4C 24 ?? 51 8B 0D ?? ?? ?? ?? C6 84 24 ?? ?? ?? ?? 03 E8");//OffsetMapID
	Offset_MapID = get(Maple.get() + 0x02);

	//Inventory
	Maple.scan("Class_Inventory", "A1 ?? ?? ?? ?? 39 98 ?? ?? ?? ?? 75");//ClassInventory
	Class_Inventory = get(Maple.get() + 0x01);
	Maple.scan("Offset_Inventory", "8B 8E ?? ?? ?? ?? 8D 44 24 ?? 50 68 ?? ?? ?? ?? 8D 54 24 ?? 52 C6 44 24 ?? 01 E8");//OffsetInventory
	Offset_Inventory = get(Maple.get() + 0x02);

	//Meso
	Maple.scan("Function_GetMesoValue", "E8 ?? ?? ?? ?? 85 F6 7C ?? 7F");//Meso
	set(&_GetMesoValue, getAddress(Maple.get()));

	//NPC
	Maple.scan("Class_NPC", "8B 0D ?? ?? ?? ?? 68 A0 7F 17 00 E8");
	Class_NPC = get(Maple.get() + 0x02);
	set(&_GetNPC, getAddress(Maple.get() + 0x0B));

	//UI
	Maple.scan("Class_UI", "8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8D 4C 24 ?? C7 44 24 ?? FF FF FF FF E8 ?? ?? ?? ?? 8B CE E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 85 C0 74");
	Class_UI = get(Maple.get() + 0x02);
	set(&_NPCTalk, getAddress(Maple.get() + 0x06));

	//InPacket
	Maple.scan("Function_SummonNPC", "83 EC 0C 53 8B 5C 24 ?? 55 57 8B F9 8B CB E8");
	Maple.Hook(Maple.get(),  SummonNPC_Hook, &_SummonNPC, 8);
	Maple.scan("Function_OpenDelivery", "6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B B4 24 ?? ?? ?? ?? 33 ED 8B CE");
	Maple.Hook(Maple.get(),  OpenDelivery_Hook, &_OpenDelivery);

	//Deli List
	Maple.scan("Class_DeliList", "8B 0D ?? ?? ?? ?? 8B 11 8B 42 ?? 6A 00 FF D0 5E 5F C2 10 00");
	Class_DeliList = get(Maple.get() + 0x02);
	Maple.scan("Offset_DeliList", "FF 24 85 ?? ?? ?? ?? E8 ?? ?? ?? ?? C2 04 00 8B 89 ?? ?? ?? ?? E8 ?? ?? ?? ?? C2 04 00");
	Offset_DeliList = get(Maple.get() + 0x11);
	set(&_GetDelivery, getAddress(Maple.get() + 0x15));
	set(&_CloseDelivery, getAddress(Maple.get() + 0x07));
	Maple.scan("SkipBlueBox(Delivery)", "E8 ?? ?? ?? ?? 83 C4 1C 89 BC 24 ?? ?? ?? ?? F6 C3 02 74", "B8 01 00 00 00");

	//Storage
	Maple.scan("SkipBlueBox(Storage)", "E8 ?? ?? ?? ?? 8B E8 8B 44 24 ?? 8B DA C7 44 24 ?? FF FF FF FF 85 C0 74");
	set(&_MesoValueDialog, getAddress(Maple.get()));
	Maple.call(Maple.get(), (DWORD)MesoValueDialog_Hook);
	Maple.scan("Function_OpenStorage", "6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B 6C 24 ?? 8B CD E8 ?? ?? ?? ?? 0F B6 F8 A1");
	Maple.Hook(Maple.get(), OpenStorage_Hook, &_OpenStorage);
	Maple.scan("Function_InMeso", "6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 89 4C 24 ?? 83 B9 ?? ?? ?? ?? 00 0F 85");
	set(&_InMeso, Maple.get());

	//CC
	Maple.scan("Function_CC", "6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 56 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 6A 01 E8 ?? ?? ?? ?? 85 C0 0F 85");
	set(&_CharacterChange, Maple.get());
	Maple.scan("Function_CharacterSelect", "6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 8B BC 24 ?? ?? ?? ?? 33 DB 8B CF 89 9E ?? ?? ?? ?? E8 ?? ?? ?? ?? 0F B6 C0");
	Maple.Hook(Maple.get(), CharacterSelect_Hook, &_CharacterSelect);
	Maple.scan("Offset_SelectedCharacter", "89 BB ?? ?? ?? ?? 8B 8E ?? ?? ?? ?? 57 E8 ?? ?? ?? ?? 57 8B CE E8 ?? ?? ?? ?? 85 C0 0F 85");
	Offset_SelectedCharacter = get(Maple.get() + 0x02);
	Maple.scan("Function_SendKey", "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? 8B 45 ?? 25 ?? ?? ?? ?? 0F 85");
	set(&_SendKey, Maple.get());
	Maple.scan("Function_Loigin", "55 8D AC 24 ?? ?? ?? ?? 81 EC ?? ?? ?? ?? 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 85 ?? ?? ?? ?? 53 56 57 50 8D 45 ?? 64 A3 00 00 00 00 8B B5 ?? ?? ?? ?? 89 4D ?? C7 81 ?? ?? ?? ?? 00 00 00 00 8B CE E8");
	Maple.Hook(Maple.get(), Login_Hook, &_Login, 8);

	Maple.scan("Teleport", "6A 01 8D 8B ?? ?? ?? ?? E8 ?? ?? ?? ?? 33 D2 39 55 ?? 8D 8B ?? ?? ?? ?? 0F 95 C2 52 E8 ?? ?? ?? ?? 8D 44 24 ?? 50 8D 8B ?? ?? ?? ?? E8");
	set(&_Encrypt1, getAddress(Maple.get() + 0x08));
	set(&_Encrypt4,  getAddress(getAddress(Maple.get() + 0x2C) + 0x0E));
	Offset_TeleToggle = get(Maple.get() + 0x04);
	Offset_TeleY = get(Maple.get() + 0x28);

	Maple.scan("Class_Character", "8B 0D ?? ?? ?? ?? 81 C1 ?? ?? ?? ?? E8 ?? ?? ?? ?? 3D ?? ?? ?? ?? 74");
	Class_Character = get(Maple.get() + 0x02);

	Maple.scan("Server TimeOut", "FF 15 ?? ?? ?? ?? 8B 4D ?? E8 ?? ?? ?? ?? 85 C0 74 0C 6A 00 6A 00 8B 4D ?? E8 ?? ?? ?? ?? 83 7D CC ?? 0F 85");
	Maple.write(Maple.get() + 0x10, "EB");

	while(!Client->hWnd){
		Sleep(50);
	}
}