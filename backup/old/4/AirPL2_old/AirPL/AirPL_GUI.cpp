#include"AirPL.h"
#include<stdio.h>

#define MaxPacketSize 1024

AirWindow AP;
AirList AL;
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

DWORD _stdcall SendLog(BYTE Packet[], DWORD dwSize, DWORD dwReturn){

	if(ListCheck(List_SI, sic, *(WORD *)&Packet[0])){
		return 0;
	}

	if(ListCheck(List_SB, sbc, *(WORD *)&Packet[0])){
		return 1;//block is enabled!
	}

	if(!bSHook){
		return 0;
	}

	AL.AddFormatItem("%s", "OUT");
	AL.AddFormatItem("%d", dwReturn);
	AL.AddFormatItem("%i", dwSize);

	if(dwSize < MaxPacketSize){
		AL.AddFormatItem("@%w %xa", *(WORD *)&Packet[0], &Packet[2], dwSize-2);
	}
	else{
		AL.AddFormatItem("@%w %s", *(WORD *)&Packet[0], "(È—ª‚³‚ê‚Ü‚µ‚½)");
	}
	return 0;
}

void _stdcall RecvLog(BYTE Packet[], DWORD dwSize, DWORD dwReturn){
	if(dwSize < 6){
		return;
	}

	if(ListCheck(List_RI, ric, *(WORD *)&Packet[4])){
		return;
	}

	if(ListCheck(List_RB, rbc, *(WORD *)&Packet[4])){
		*(WORD *)&Packet[4] = 0xFFFF;//block:modify header
		return;
	}

	if(!bRHook){
		return;
	}

	dwSize -= 4;
	AL.AddFormatItem("%s", "IN");
	AL.AddFormatItem("%d", dwReturn);
	AL.AddFormatItem("%i", dwSize);
	if(dwSize < MaxPacketSize){
		AL.AddFormatItem("@%w %xa", *(WORD *)&Packet[4], &Packet[6], dwSize-2);
	}
	else{
		AL.AddFormatItem("@%w %s", *(WORD *)&Packet[4], "(È—ª‚³‚ê‚Ü‚µ‚½)");
	}
}

#define EDIT_PACKET 1000
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

void SendEditPacket(){
	char *PacketString = AP.GetString(EDIT_PACKET);
	if(PacketString){
		SendPacket(PacketString);
	}
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

/*
void ExploitFinder(){
	char *HeaderString = AP.GetString(EDIT_HEADER3);
	WORD Header = StringToWORD(HeaderString);
	if(Header != 0xFFFF){
		NullPacket(Header);
		AP.SetFormatString(EDIT_HEADER3, "@%w", Header+1);
	}
}
*/
void ExploitFinder(){
	char *HeaderString = AP.GetString(EDIT_PACKET);
	WORD Header = StringToWORD(HeaderString);
	if(Header != 0xFFFF){
		//NullPacket(Header);
		int i;
		for(i=0; HeaderString[i] != ' '; i++){
		}
		SendPacket(HeaderString);
		AP.SetFormatString(EDIT_PACKET, "@%w%s", Header+1, &HeaderString[i]);
	}
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
		HeaderString = AP.GetString(EDIT_HEADER1);
	}
	else{
		HeaderString = AP.GetString(EDIT_HEADER2);
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

void PECreate(HWND hWnd){
	AL.Init(hWnd, 3, 3, 444, 294);
	AL.AddHeader("Type", 40);
	AL.AddHeader("Return", 70);
	AL.AddHeader("Size", 40);
	AL.AddHeader("Packet", 270);
	AP.EditBox(EDIT_PACKET, NULL, 3, 320, 400);
	AP.EditBox(EDIT_RETURN, NULL, 3, 300, 80);
	AP.CheckBox(CHECK_HOOKS, "SendHook", 100, 300, true);
	AP.CheckBox(CHECK_HOOKR, "RecvHook", 180, 300, true);
	AP.Button(BUTTON_CLEAR, "Clear", 260, 300);
	AP.Button(BUTTON_SEND, "Send", 410, 320);
	
//	AP.EditBox(EDIT_HEADER3, "@0000", 3, 340, 50);
	AP.Button(BUTTON_EF, "EF", 60, 340);
	
	SL.Init(hWnd, 450, 3, 144, 94);
	SL.AddHeader("Header", 80);
	SL.AddHeader("Type", 40);
	AP.EditBox(EDIT_HEADER1, NULL, 450, 100, 50);
	AP.Button(BUTTON_ADD1, "SI", 510, 100);
	AP.Button(BUTTON_ADD2, "SB", 550, 100);
	AP.Button(BUTTON_RESET1, "Delete", 500, 120);

	RL.Init(hWnd, 450, 153, 144, 94);
	RL.AddHeader("Header", 80);
	RL.AddHeader("Type", 40);
	AP.EditBox(EDIT_HEADER2, NULL, 450, 250, 50);
	AP.Button(BUTTON_ADD3, "RI", 510, 250);
	AP.Button(BUTTON_ADD4, "RB", 550, 250);
	AP.Button(BUTTON_RESET2, "Delete", 500, 270);

	ResetList1();
	ResetList2();
	LoadList();
	AP.Button(BUTTON_SAVE, "Save", 560, 290);
	AP.Button(BUTTON_LOAD, "Load", 560, 320);

	WriteHooks();
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
		case BUTTON_SEND:
			SendEditPacket();
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
		default:
			break;
	}
}


void PENotify(){
	HWND hWnd = GetFocus();
	char *Buffer, *Buffer2, *Buffer3;

	if(hWnd == AL.GetHWND()){
		Buffer = AL.GetSelectedItem(3);
		if(Buffer){
			Buffer2 = AL._GetSelectedItem(1);
			Buffer3 = AL._GetSelectedItem(0);
			AP.SetString(EDIT_PACKET, Buffer);
			AP.SetString(EDIT_RETURN, Buffer2);
			Buffer[5] = 0;
			if(Buffer3[0] == 'O'){
				AP.SetString(EDIT_HEADER1, Buffer);
			}
			else{
				AP.SetString(EDIT_HEADER2, Buffer);
			}
		}
	}
	
}


void AirPL_Run(HINSTANCE hInstance){
	AP.Init(hInstance, "Air Packet Editor", 600, 400, true, PECreate, PEControl, PENotify);
}