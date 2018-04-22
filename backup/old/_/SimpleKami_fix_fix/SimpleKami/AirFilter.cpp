#include"AirFilter.h"

typedef struct{
	BOOL type;
	int itemID1;
	int itemID2;
}AirFilter;

namespace AIF{
	DWORD dwItemIDHook = 0x005BF207;
	DWORD dwItemIDHook_ret = dwItemIDHook + 5;

	DWORD dwRecvHook= 0x00522001;//v97.1
	DWORD dwRecvFunction = 0x00521DC0;//97.1

	BOOL _stdcall ItemIDCompare(int itemID);
	BOOL _stdcall ItemIDCompare2(int itemID);
	void ItemIDHook();
	void ItemIDHook2();
	int FilterCount = 0;
	int FilterCount2 = 0;
	AirFilter ItemList[1024];
	AirFilter ItemList2[1024];
}

void _declspec(naked) AIF::ItemIDHook(){
	_asm{
		cmp dword ptr [esi+0x30],1//Meso
		je ItemIDHook_Ending
		pushad
		push eax
		call ItemIDCompare
		test eax,eax
		je ItemID_Modification
		popad
		jmp ItemIDHook_Ending

ItemID_Modification:
		popad
		xor eax,eax
ItemIDHook_Ending:
		mov ecx,ebx
		mov [esi+0x34],eax//ItemID
		jmp dwItemIDHook_ret
	}
}


void _declspec(naked) AIF::ItemIDHook2(){
	_asm{
		mov eax,[esp+0x04]
		mov eax,[eax+0x08]//RecvPacket
		pushad
		xor ebx,ebx
		add eax,0x04//garbage 4bytes
		cmp word ptr [eax],0x01D4//Spawn Header1
		je SpawnBlock
		inc ebx
		cmp word ptr [eax],0x01D6//Spawn Header2
		je SpawnBlock
		jmp ItemIDHook2_Ending

SpawnBlock:
		add eax,0x07
		add eax,ebx
		mov eax,[eax]
		push eax
		call ItemIDCompare2
		test eax,eax
		jne ItemIDHook2_Ending
		popad
		ret 0x0004

ItemIDHook2_Ending:
		popad
		jmp dwRecvFunction

	}
}




void AIF::ItemFilter(BOOL Flag){
	if(Flag == TRUE){
		Asm::Write_Hook("jmp", dwItemIDHook, (DWORD)ItemIDHook);
	}else{
		Asm::Write_code(dwItemIDHook, "8B CB 89 46 34", 0);
	}
}

void AIF::ItemFilter2(BOOL Flag){
	if(Flag == TRUE){
		Asm::Write_Hook("call", dwRecvHook, (DWORD)ItemIDHook2);
	}else{
		Asm::Write_Hook("call", dwRecvHook, dwRecvFunction);
	}
}


BOOL _stdcall AIF::ItemIDCompare(int itemID){
	int i;

	for(i=0; i<FilterCount; i++){
		if(ItemList[i].type == TRUE){
			if(ItemList[i].itemID1 <= itemID && ItemList[i].itemID2 >= itemID){
				return TRUE;
			}
		}else{
			if(ItemList[i].itemID1 == itemID){
				return TRUE;
			}
		}
	}

	return FALSE;
}


BOOL _stdcall AIF::ItemIDCompare2(int itemID){
	int i;

	for(i=0; i<FilterCount2; i++){
		if(ItemList2[i].type == TRUE){
			if(ItemList2[i].itemID1 <= itemID && ItemList2[i].itemID2 >= itemID){
				return FALSE;
			}
		}else{
			if(ItemList2[i].itemID1 == itemID){
				return FALSE;
			}
		}
	}

	return TRUE;
}


int AIF::LoadItemList(){
	unsigned char buf[0x100];
	unsigned char buf2[0x100];
	FILE *fp;
	int i, j, k;
	int Hit = 0;

	fp = fopen("DLL/ItemList.txt", "r");

	if(fp == NULL){
		return -1;
	}

	for(i=0; fgets((char *)buf, 0x100, fp)!=NULL; i++){
		ZeroMemory(&ItemList[Hit], sizeof(AirFilter));
		ZeroMemory(buf2, 0x100);
		k = 0;
		for(j=0; buf[j]!=0x00; j++){

			if(buf[j] >= '0' && buf[j] <= '9'){
				buf2[k] = buf[j];
				k++;
			}

			if(buf[j]=='-'){
				ItemList[Hit].type = TRUE;
				buf2[k] = ' ';
				k++;
			}

			if(buf[j]=='/'){
				break;
			}
		}

		buf2[k]=0x00;




		if(ItemList[Hit].type == TRUE){//range A - B
			sscanf_s((char* )buf2, "%d %d", &ItemList[Hit].itemID1, &ItemList[Hit].itemID2);
			if(ItemList[Hit].itemID2 < ItemList[Hit].itemID1){
				continue;
			}
		}else{
			sscanf_s((char* )buf2, "%d", &ItemList[Hit].itemID1);
		}

		if(ItemList[Hit].itemID1 != 0){
			Hit++;
		}
	}

	fclose(fp);

	fp = fopen("DLL/ItemList_.txt", "w");

	for(i=0; i<Hit; i++){
		if(ItemList[i].type == TRUE){
			fprintf(fp, "Line%03d:%d-%d\n", i, ItemList[i].itemID1, ItemList[i].itemID2);
		}else{
			fprintf(fp, "Line%03d:%d\n", i, ItemList[i].itemID1);
		}
	}
	fclose(fp);

	FilterCount = Hit;

	return Hit;
}



int AIF::LoadItemList2(){
	unsigned char buf[0x100];
	unsigned char buf2[0x100];
	FILE *fp;
	int i, j, k;
	int Hit = 0;

	fp = fopen("DLL/MobList.txt", "r");

	if(fp == NULL){
		return -1;
	}

	for(i=0; fgets((char *)buf, 0x100, fp)!=NULL; i++){
		ZeroMemory(&ItemList2[Hit], sizeof(AirFilter));
		ZeroMemory(buf2, 0x100);
		k = 0;
		for(j=0; buf[j]!=0x00; j++){

			if(buf[j] >= '0' && buf[j] <= '9'){
				buf2[k] = buf[j];
				k++;
			}

			if(buf[j]=='-'){
				ItemList2[Hit].type = TRUE;
				buf2[k] = ' ';
				k++;
			}

			if(buf[j]=='/'){
				break;
			}
		}

		buf2[k]=0x00;




		if(ItemList2[Hit].type == TRUE){//range A - B
			sscanf_s((char* )buf2, "%d %d", &ItemList2[Hit].itemID1, &ItemList2[Hit].itemID2);
			if(ItemList2[Hit].itemID2 < ItemList2[Hit].itemID1){
				continue;
			}
		}else{
			sscanf_s((char* )buf2, "%d", &ItemList2[Hit].itemID1);
		}

		if(ItemList2[Hit].itemID1 != 0){
			Hit++;
		}
	}

	fclose(fp);

	fp = fopen("DLL/MobList_.txt", "w");

	for(i=0; i<Hit; i++){
		if(ItemList2[i].type == TRUE){
			fprintf(fp, "Line%03d:%d-%d\n", i, ItemList2[i].itemID1, ItemList2[i].itemID2);
		}else{
			fprintf(fp, "Line%03d:%d\n", i, ItemList2[i].itemID1);
		}
	}
	fclose(fp);

	FilterCount2 = Hit;

	return Hit;
}
