#include"gui.h"
#include"Hook.h"
#include"Packet.h"
#include<stdio.h>
#include"MapTeleport.h"

AirWindow AW;
AirList AL;
//packet ignore block
#define EDIT_PACKET 1000
#define EDIT_FORMATPACKET 1100
#define EDIT_RETURN 1001
#define EDIT_HEADER1 1002
#define EDIT_HEADER2 1003
#define EDIT_HEADER3 1004
#define CHECK_HOOKS 2000
#define CHECK_HOOKR 2001
#define BUTTON_CLEAR 3000
#define BUTTON_SEND 3001
#define BUTTON_ADD1 3002
#define BUTTON_ADD2 3003
#define BUTTON_ADD3 3004
#define BUTTON_ADD4 3005
#define BUTTON_RESET1 3006
#define BUTTON_RESET2 3007
#define BUTTON_SAVE 3008
#define BUTTON_LOAD 3009
#define BUTTON_EF 3010
#define EDIT_MAPID 1101//add
#define BUTTON_TELEPORT 3300//add

AirList SL, RL;

bool bSHook = true, bRHook = true;

WORD List_SI[0x100], List_SB[0x100], List_RI[0x100], List_RB[0x100];
int sic, sbc, ric, rbc;

bool ListCheck(WORD *List, int mc, WORD w){
	int i;
	for(i=0; i<mc; i++){
		if(List[i] == w){
			return true;
		}
	}
	return false;
}

void ResetList1(){
	sic = 0;
	sbc = 0;
	List_SI[sic] = 0xFEFE;
	List_SB[sbc] = 0xFEFE; 
}

void ResetList2(){
	ric = 0;
	rbc = 0;
	List_RI[ric] = 0xFEFE;
	List_RB[rbc] = 0xFEFE; 
}

char deadbeef[128] = "0123456789ABCDEF0123456789abcdef";

WORD StringToWORD(char *String){
	int i, j, hbc = 0;
	int bs[4] = {4, 0, 12, 8};

	WORD w = 0;
	for(i=0; String[i]; i++){
		for(j=0; deadbeef[j]; j++){
			if(deadbeef[j] == String[i]){
				if(String[0] == '@'){
					w += (j%0x10) << (12 - hbc*4);
				}
				else{
					w += (j%0x10) << bs[hbc];
				}
				hbc++;
				break;
			}
		}
		if(hbc == 4){
			return w;
		}
		else if(hbc > 4){
			return 0xFFFF;
		}
	}
	return 0xFFFF;
}

void UpdateList1(){
	int i;
	char *s = SL._GetSelectedItem(0), *t = SL._GetSelectedItem(1);
	WORD wDel, *List;
	int *mc;

	if(s == NULL){
		return;
	}

	if(t[0] == 'S'){
		if(t[1] == 'I'){
			List = List_SI;
			mc = &sic;
		}
		else if(t[1] == 'B'){
			List = List_SB;
			mc = &sbc;
		}
	}
	else{
		return;
	}

	SL.DeleteSelectedItem();
	wDel = StringToWORD(s);

	for(i=0; List[i] != wDel; i++){
	}

	for(i; i<*mc; i++){
		List[i] = List[i+1];
	}

	(*mc)--;
	List[*mc] = 0xFEFE;
}

void UpdateList2(){
	int i;
	char *s = RL._GetSelectedItem(0), *t = RL._GetSelectedItem(1);
	WORD wDel, *List;
	int *mc;

	if(s == NULL){
		return;
	}

	if(t[0] == 'R'){
		if(t[1] == 'I'){
			List = List_RI;
			mc = &ric;
		}
		else if(t[1] == 'B'){
			List = List_RB;
			mc = &rbc;
		}
	}
	else{
		return;
	}

	RL.DeleteSelectedItem();
	wDel = StringToWORD(s);

	for(i=0; List[i] != wDel; i++){
	}

	for(i; i<*mc; i++){
		List[i] = List[i+1];
	}

	(*mc)--;
	List[*mc] = 0xFEFE;
}

bool AddList(WORD *List, int *lc, WORD w){
	int i;
	for(i=0; i<*lc; i++){
		if(List[i] == w){
			return false;
		}
	}
	List[(*lc)++] = w;
	List[(*lc)] = 0xFEFE;
	return true;
}

void AddHeaderList(WORD wID){
	char *HeaderString;
	WORD Header;

	if(wID == BUTTON_ADD1 || wID == BUTTON_ADD2){
		HeaderString = AW.GetString(EDIT_HEADER1);
	}
	else{
		HeaderString = AW.GetString(EDIT_HEADER2);
	}

	if(!HeaderString || HeaderString[0] == NULL){//error check
		return;
	}

	Header = StringToWORD(HeaderString);

	if(Header == 0xFFFF){//error
		return;
	}

	switch(wID){
	case BUTTON_ADD1:
		if(AddList(List_SI, &sic, Header)){
			SL.AddFormatItem("@%w", Header);
			SL.AddItem("SI");
		}
		break;
	case BUTTON_ADD2:
		if(AddList(List_SB, &sbc, Header)){
			SL.AddFormatItem("@%w", Header);
			SL.AddItem("SB");
		}
		break;
	case BUTTON_ADD3:
		if(AddList(List_RI, &ric, Header)){
		RL.AddFormatItem("@%w", Header);
		RL.AddItem("RI");
		}
		break;
	case BUTTON_ADD4:
		if(AddList(List_RB, &rbc, Header)){
			RL.AddFormatItem("@%w", Header);
			RL.AddItem("RB");
		}
		break;
	default:
		break;
	}

}


void SaveList(){
	FILE *fp;
	int i;

	fp = fopen("AirPE_SI.txt", "w");
		for(i=0; List_SI[i] != 0xFEFE; i++){
			fprintf(fp, "@%04X\r\n", List_SI[i]);
		}
	fclose(fp);


	fp = fopen("AirPE_SB.txt", "w");
		for(i=0; List_SB[i] != 0xFEFE; i++){
			fprintf(fp, "@%04X\r\n", List_SB[i]);
		}
	fclose(fp);

	fp = fopen("AirPE_RI.txt", "w");
		for(i=0; List_RI[i] != 0xFEFE; i++){
			fprintf(fp, "@%04X\r\n", List_RI[i]);
		}
	fclose(fp);


	fp = fopen("AirPE_RB.txt", "w");
		for(i=0; List_RB[i] != 0xFEFE; i++){
			fprintf(fp, "@%04X\r\n", List_RB[i]);
		}
	fclose(fp);

}

void LoadList(){
	FILE *fp;
	int i;
	WORD w;

	SL.Clear();
	RL.Clear();
	ResetList1();
	ResetList2();

	fp = fopen("AirPE_SI.txt", "r");
	if(fp){
		while(fscanf(fp,"@%04X\r\n", &w) != EOF){
			SL.AddFormatItem("@%w", w);
			SL.AddItem("SI");
			AddList(List_SI, &sic, w);
		}
	fclose(fp);
	}
	if(fp){
	fp = fopen("AirPE_SB.txt", "r");
		while(fscanf(fp,"@%04X\r\n", &w) != EOF){
			SL.AddFormatItem("@%w", w);
			SL.AddItem("SB");
			AddList(List_SB, &sbc, w);
		}
	fclose(fp);
	}
	fp = fopen("AirPE_RI.txt", "r");
	if(fp){
		while(fscanf(fp,"@%04X\r\n", &w) != EOF){
			RL.AddFormatItem("@%w", w);
			RL.AddItem("RI");
			
			AddList(List_RI, &ric, w);
		}
	fclose(fp);
	}
	if(fp){
	fp = fopen("AirPE_RB.txt", "r");
		while(fscanf(fp,"@%04X\r\n", &w) != EOF){
			RL.AddFormatItem("@%w", w);
			RL.AddItem("RB");
			AddList(List_RB, &ric, w);
		}
	fclose(fp);
	}
}
//end

CRITICAL_SECTION css, css2, csr, csg;

void gui_create(HWND hWnd){
	AL.Init(hWnd, 3, 3, 594, 594 - 100);
	AL.AddHeader("Type", 40);
	AL.AddHeader("Return", 60);
	AL.AddHeader("Size", 50);
	AL.AddHeader("Packet", 400);
	AL.AddHeader(NULL, 0);//Format
	//ignore block
	AW.EditBox(EDIT_PACKET, NULL, 3, 520, 400);
	AW.EditBox(EDIT_FORMATPACKET, NULL, 3, 540, 400);
	AW.EditBox(EDIT_RETURN, NULL, 3, 500, 80);
	AW.CheckBox(CHECK_HOOKS, "SendHook", 100, 500, true);
	AW.CheckBox(CHECK_HOOKR, "RecvHook", 180, 500, true);
	AW.Button(BUTTON_CLEAR, "Clear", 260, 500);
	//AW.Button(BUTTON_SEND, "Send", 410, 520);

	//AW.Button(BUTTON_EF, "EF", 450, 520);

	SL.Init(hWnd, 600, 3, 144, 94);
	SL.AddHeader("Header", 80);
	SL.AddHeader("Type", 40);
	AW.EditBox(EDIT_HEADER1, NULL, 600, 100, 50);
	AW.Button(BUTTON_ADD1, "SI", 660, 100);
	AW.Button(BUTTON_ADD2, "SB", 700, 100);
	AW.Button(BUTTON_RESET1, "Delete", 650, 120);

	RL.Init(hWnd, 600, 153, 144, 94);
	RL.AddHeader("Header", 80);
	RL.AddHeader("Type", 40);
	AW.EditBox(EDIT_HEADER2, NULL, 600, 250, 50);
	AW.Button(BUTTON_ADD3, "RI", 660, 250);
	AW.Button(BUTTON_ADD4, "RB", 700, 250);
	AW.Button(BUTTON_RESET2, "Delete", 650, 270);

	ResetList1();
	ResetList2();
	LoadList();
	AW.Button(BUTTON_SAVE, "Save", 710, 290);
	AW.Button(BUTTON_LOAD, "Load", 710, 320);
	//
	InitializeCriticalSection(&css);//critial
	InitializeCriticalSection(&css2);//critial
	InitializeCriticalSection(&csr);//critial
	InitializeCriticalSection(&csg);//critial

//	AW.EditBox(EDIT_MAPID, "680000502", 600, 400, 100, 0, true);
//	AW.Button(BUTTON_TELEPORT, "Teleport", 710, 400);

	Hook();
}

DWORD gui_AddSendPacket(SPacket *p, DWORD Return){
	//EnterCriticalSection(&css);
	if(ListCheck(List_SI, sic, *(WORD *)&p->Packet[0])){
		//LeaveCriticalSection(&css);
		return PACKET_IGNORE;
	}

	if(ListCheck(List_SB, sbc, *(WORD *)&p->Packet[0])){
		//LeaveCriticalSection(&css);
		return PACKET_BLOCK;
	}

	if(!bSHook){
		//LeaveCriticalSection(&css);
		return 0;
	}
	__try{
		//EnterCriticalSection(&css);
		AL.AddItem("Out");
		AL.AddFormatItem("%d", Return);
		AL.AddFormatItem("%i", p->Length);
		AL.AddFormatItem("%xa", p->Packet, p->Length);

		//if(spi && spi->s && spi->EncodedLength == p->Length){//success
		if(firstthread == GetCurrentThreadId() && spi->EncodedLength == p->Length){
			AL.AddFormatItem("@%s", spi->s);
		}
		else{//failure
			AL.AddItem("Format Error");
		}
		//LeaveCriticalSection(&css);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
	}
	return 0;
}

DWORD gui_AddSendPacket2(SPacket *p, DWORD Return){
	//EnterCriticalSection(&css2);
	if(ListCheck(List_SI, sic, *(WORD *)&p->Packet[0])){
		//LeaveCriticalSection(&css2);
		return PACKET_IGNORE;
	}

	if(ListCheck(List_SB, sbc, *(WORD *)&p->Packet[0])){
		//LeaveCriticalSection(&css2);
		return PACKET_BLOCK;
	}

	if(!bSHook){
	//	LeaveCriticalSection(&css2);
		return 0;
	}
	__try{
		EnterCriticalSection(&css2);
		AL.AddItem("Out");
		AL.AddFormatItem("%d", Return);
		AL.AddFormatItem("%i", p->Length);
		AL.AddFormatItem("%xa", p->Packet, p->Length);

		//if(spi && spi->s && spi->EncodedLength == p->Length){//success
		//if(firstthread == GetCurrentThreadId() && spi->EncodedLength == p->Length){
		//	AL.AddFormatItem("@%s", spi->s);
		//}
		//else{//failure
			AL.AddItem("Format Error");
	//	}
		LeaveCriticalSection(&css2);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
	}
	return 0;
}

DWORD gui_AddProcessPacket(RPacket *p){
	//EnterCriticalSection(&csr);
	if(ListCheck(List_RI, ric, *(WORD *)&p->Packet[4])){
		//LeaveCriticalSection(&csr);
		return PACKET_IGNORE;
	}

	if(ListCheck(List_RB, rbc, *(WORD *)&p->Packet[4])){
		//LeaveCriticalSection(&csr);
		return PACKET_BLOCK;
	}

	if(!bRHook){
		//LeaveCriticalSection(&csr);
		return 0;
	}
	__try{
		EnterCriticalSection(&csr);
		AL.AddItem("In");
		if(RF && RF->FunctionAddress){
			AL.AddFormatItem("%d", RF->FunctionAddress);
		}
		else{
			AL.AddItem("Error");
		}
		AL.AddFormatItem("%i", p->DataLength);

		//if(p->DataLength <= 4096){
			AL.AddFormatItem("%xa", &p->Packet[4], p->DataLength);
	///	}
		//else{
		//	AL.AddFormatItem("%xa", &p->Packet[4], 4096);
		//}
	
		if(RF && RF->String/* && p->DataLength <= 4096*/){
			AL.AddFormatItem("@%s", RF->String);
		}
		else{
			AL.AddItem("Format Error");
		}
		LeaveCriticalSection(&csr);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
	}
	return 0;
}



void SendStrPacket(){
	char *s = AW.GetString(EDIT_PACKET);
	OutPacket p(s);
	p.SendPacket();
}

void ExploitFinder(){
	__try{
		char *s = AW.GetString(EDIT_PACKET);
		char Buffer[256];
		WORD Header = StringToWORD(s);
		SendStrPacket();
		Header++;
		sprintf(Buffer, "%02X %02X", Header & 0xFF, Header >> 8);
		RtlCopyMemory(s, Buffer, 5);
		AW.SetString(EDIT_PACKET, s);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
	}
}

void gui_control(WORD wID){
	switch(wID){
		case BUTTON_CLEAR:
			//EnterCriticalSection(&csg);
			AL.Clear();
			//LeaveCriticalSection(&csg);
			break;
		case CHECK_HOOKS:
			bSHook = AW.CheckBoxStatus(wID);
			break;
		case CHECK_HOOKR:
			bRHook = AW.CheckBoxStatus(wID);
			break;
		case BUTTON_SEND:
			//SendStrPacket();
			break;
		case BUTTON_ADD1:
		case BUTTON_ADD2:
		case BUTTON_ADD3:
		case BUTTON_ADD4:
			AddHeaderList(wID);
			break;
		case BUTTON_RESET1:
			UpdateList1();
			break;
		case BUTTON_RESET2:
			UpdateList2();
			break;
		case BUTTON_SAVE:
			SaveList();
			break;
		case BUTTON_LOAD:
			LoadList();
			break;
		case BUTTON_EF:
			ExploitFinder();
			break;

		case BUTTON_TELEPORT:
			if(bTeleporting){
				bTeleporting = false;
			}
			else{
				bTeleporting = true;
				MapTeleport(AW.GetInt(EDIT_MAPID));
			}
			break;
		default:
			break;
	}
}

void gui_notify(){
	__try{
		HWND hWnd = GetFocus();
		char *Buffer, *Buffer2, *Buffer3, *Buffer4;

		if(hWnd == AL.GetHWND()){
			Buffer = AL.GetSelectedItem(3);
			if(Buffer){
				Buffer2 = AL._GetSelectedItem(1);
				Buffer3 = AL._GetSelectedItem(0);
				Buffer4 = AL._GetSelectedItem(4);
				AW.SetString(EDIT_PACKET, Buffer);
				AW.SetString(EDIT_FORMATPACKET, Buffer4);
				AW.SetString(EDIT_RETURN, Buffer2);
				Buffer[5] = 0;
				if(Buffer3[0] == 'O'){
					AW.SetString(EDIT_HEADER1, Buffer);
				}
				else{
					AW.SetString(EDIT_HEADER2, Buffer);
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
	}
	
}

HINSTANCE hDLL;
DWORD dwFLAG = 0;
void CALLBACK SendPacketProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime){
	if(!dwFLAG){
		AW.Init(hDLL, "AirPacketEditor beta", 800, 600, false, gui_create, gui_control, gui_notify);
		dwFLAG++;
	}
	else{
		KillTimer(hwnd, 1337);
	}
}

void Inject(){
	//HMODULE huxtheme = GetModuleHandleA("uxtheme.dll");
	HWND hMaple = NULL;
	while(!hMaple){
		Sleep(50);
		hMaple = FindWindowA("MapleStoryClass", "MapleStory");
	}
	SetTimer(hMaple, 1337, 50, SendPacketProc);
}

void gui(HINSTANCE hinstDLL){
	hDLL = hinstDLL;
	//Inject();
	//tid = *(DWORD *)((DWORD)GetModuleHandleA("uxtheme.dll") + 0x50C5C);
	AW.Init(hinstDLL, "AirPacketEditor beta", 800, 600, true, gui_create, gui_control, gui_notify);
}