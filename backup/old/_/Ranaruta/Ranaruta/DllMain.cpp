#include<Windows.h>
#include<stdio.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

void Write1(BYTE temp, PDWORD CurrentAddr){
	*(BYTE *)CurrentAddr[0] = temp;
	CurrentAddr[0]+=1;
}

void Write2(WORD temp, PDWORD CurrentAddr){
	*(WORD *)CurrentAddr[0] = temp;
	CurrentAddr[0]+=2;
}

void Write4(DWORD temp, PDWORD CurrentAddr){
	*(DWORD *)CurrentAddr[0] = temp;
	CurrentAddr[0]+=4;
}

void WriteCall(DWORD Addr, PDWORD CurrentAddr){
	*(BYTE *)CurrentAddr[0] = 0xE8;
	*(DWORD *)(CurrentAddr[0]+1) = Addr-CurrentAddr[0]-5;
	CurrentAddr[0]+=5;
}

void WriteJmp(DWORD Addr, PDWORD CurrentAddr){
	*(BYTE *)CurrentAddr[0] = 0xE9;
	*(DWORD *)(CurrentAddr[0]+1) = Addr-CurrentAddr[0]-5;
	CurrentAddr[0]+=5;
}

void DupeMemory(DWORD src, DWORD dwSize, PDWORD CurrentAddr){
	memcpy((DWORD *)CurrentAddr[0], (DWORD *)src, dwSize);
	CurrentAddr[0]+=dwSize;
}

void Debug(BOOL Flag){
	if(Flag == TRUE){
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
	}else{
		FreeConsole();
	}
}

void AirBypassInitialization(){
	//VirtualProtect((DWORD *)ReturnChecker, 0x100, PAGE_EXECUTE_READWRITE, &old);
	//VirtualProtect((DWORD *)SystemCallHook, 0x100, PAGE_EXECUTE_READWRITE, &old);
	//VirtualProtect((DWORD *)Sys64, 7, PAGE_EXECUTE_READWRITE, &old);
	//CurrentAddr = ReturnChecker;
}

unsigned char *Memory;
DWORD AddrCheck;

void MakeESICheck(DWORD MS_START, DWORD MS_END){
	DWORD old;
	DWORD CA;
	DWORD ESI_CHECK_FUNCTION, CRC1_Hook, CRC2_Hook, CRC3_Hook;
	CA = (DWORD)new BYTE[0x1000];

	VirtualProtect((DWORD *)AddrCheck , 0x1000, PAGE_EXECUTE_READWRITE, &old);

	ESI_CHECK_FUNCTION = CA;
	Write2(0xFE81, &CA);
	Write4(MS_START, &CA);
	Write2(0x1472, &CA);
	Write2(0xFE81, &CA);
	Write4(MS_END, &CA);
	Write2(0x0C77, &CA);
	Write2(0xEE81, &CA);
	Write4(MS_START, &CA);
	Write2(0xC681, &CA);
	Write4((DWORD)Memory, &CA);
	Write1(0xC3, &CA);

	CRC1_Hook = CA;
	Write1(0x56, &CA);//push esi
	Write2(0xF18B, &CA);//mov esi,ecx
	WriteCall(ESI_CHECK_FUNCTION, &CA);
	Write2(0x168A, &CA);//mov dl,[esi]
	Write1(0x5E, &CA);//pop esi
	DupeMemory(0x010F9AA9+2, 5-2, &CA);
	WriteJmp(0x010F9AA9+5, &CA);

	CRC2_Hook = CA;
	Write1(0x56, &CA);//push esi
	Write2(0xF18B, &CA);//mov esi,ecx
	WriteCall(ESI_CHECK_FUNCTION, &CA);
	Write2(0x0602, &CA);//add al,[ecx]
	Write1(0x5E, &CA);//pop esi
	DupeMemory(0x01ECAF3F+2, 6-2, &CA);
	WriteJmp(0x01ECAF3F+6, &CA);
	
	CRC3_Hook = CA;
	Write1(0x56, &CA);//push esi
	Write2(0xF28B, &CA);//mov esi,ecx
	WriteCall(ESI_CHECK_FUNCTION, &CA);
	Write2(0x36FF, &CA);//push [esi]
	Write1(0x5E, &CA);//pop esi
	WriteJmp(0x01DC6C22, &CA);

	//0x01DD6CCE
	//0x01DC6C22
	
	*(BYTE *)(0x010F9AA9) = 0xE9;
	*(DWORD *)(0x010F9AA9+1) = CRC1_Hook-0x010F9AA9-5;
	*(BYTE *)(0x01ECAF3F) = 0xE9;
	*(DWORD *)(0x01ECAF3F+1) = CRC2_Hook-0x01ECAF3F-5;
	*(BYTE *)(0x01DD6CCE) = 0xE9;
	*(DWORD *)(0x01DD6CCE+1) = CRC3_Hook-0x01DD6CCE-5;

}

void AutomaticHook(){

}

void MemoryDump(){
	IMAGE_NT_HEADERS *nth = (IMAGE_NT_HEADERS *)((DWORD)GetModuleHandleA(NULL) + PIMAGE_DOS_HEADER(GetModuleHandleA(NULL))->e_lfanew);
	Memory = (unsigned char *)malloc(nth->OptionalHeader.SizeOfImage);
	RtlCopyMemory(Memory, (DWORD *)nth->OptionalHeader.ImageBase, nth->OptionalHeader.SizeOfImage);
	printf("MS_STAR:%p\nMS_END:%p\n", (DWORD)nth->OptionalHeader.ImageBase, (DWORD)nth->OptionalHeader.ImageBase+(DWORD)nth->OptionalHeader.SizeOfImage);
	MakeESICheck((DWORD)nth->OptionalHeader.ImageBase, (DWORD)nth->OptionalHeader.ImageBase+(DWORD)nth->OptionalHeader.SizeOfImage);
}


void hello(HINSTANCE hinstDLL){
	int bla;
	Debug(TRUE);
	
	MemoryDump();

	scanf("%d", &bla);
	Debug(FALSE);
	return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hello, hinstDLL, NULL, NULL);

	return TRUE;
}