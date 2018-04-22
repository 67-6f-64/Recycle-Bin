#include"AirMemory.h"

namespace Air{
	DWORD ReadAob(char *Aob, BYTE *b, bool *mask);
	int MakeBYTEArray(BYTE *b, char *Code);
}


DWORD Air::ReadAob(char *Aob, BYTE *b, bool *mask){
	DWORD dwSize = 0;
	int i, j;
	BYTE temp;
	BYTE rb;
	i = 0;
	j = 0;

	while(Aob[j]){
		rb = (BYTE)Aob[j];
		if(i%2 == 0){
			temp = 4;//high
		}
		else{
			temp = 0;//low
		}

		if(rb >= '0' && rb <= '9'){
			b[i/2] += (rb - '0') << temp;
			i++;
		}
		else if(rb >= 'A' && rb <= 'F'){
			b[i/2] += (rb - 'A' + 0x0A) << temp;
			i++;
		}
		else if(rb >= 'a' && rb <= 'f'){
			b[i/2] += (rb - 'a' + 0x0A) << temp;
			i++;
		}
		else if(rb == '?'){
			b[i/2] = 0xFF;
			mask[i/2] = true;
			i++;
		}
		j++;
	}

	return (i%2 == 0) ? (i/2) : -1;
}

int Air::MakeBYTEArray(BYTE *b, char *Code){
	BYTE wb, temp;
	int i, j;
	i = 0;
	j = 0;
	while(Code[j]){
		wb = (BYTE)Code[j];
		if(i%2 == 0){
			temp = 4;//high
		}
		else{
			temp = 0;//low
		}

		if(wb >= '0' && wb <= '9'){
			b[i/2] += (wb - '0') << temp;
			i++;
		}
		else if(wb >= 'A' && wb <= 'F'){
			b[i/2] += (wb - 'A' + 0x0A) << temp;
			i++;
		}
		else if(wb >= 'a' && wb <= 'f'){
			b[i/2] += (wb - 'a' + 0x0A) << temp;
			i++;
		}
		j++;
	}

	return (i%2 == 0) ? (i/2) : -1;
}

AirMemory::AirMemory(){
	BaseAddress = NULL;
	Memory_Start = NULL;
	Memory_End = NULL;
	MemoryDump = NULL;
	IsDLL = false;
	MemoryList.Next = NULL;
}

void AirMemory::Init(char *ModuleName){
	HMODULE hModule = NULL;
	MODULEINFO ModInfo;
	HANDLE hProcess = GetCurrentProcess();
	IMAGE_NT_HEADERS *nth;

	if(ModuleName){//DLL
		while(!(hModule = GetModuleHandleA(ModuleName))){
			Sleep(100);
		}
		GetModuleInformation(hProcess, hModule, &ModInfo, sizeof(MODULEINFO));
		Memory_Start = (DWORD)ModInfo.lpBaseOfDll;
		Memory_End = (DWORD)ModInfo.lpBaseOfDll + (DWORD)ModInfo.SizeOfImage;
		IsDLL = true;
	}
	else{//Client
		hModule = GetModuleHandleA(NULL);
		nth = (IMAGE_NT_HEADERS *)((DWORD)hModule + PIMAGE_DOS_HEADER(hModule)->e_lfanew);
		Memory_Start = (DWORD)nth->OptionalHeader.ImageBase;
		Memory_End = (DWORD)nth->OptionalHeader.ImageBase + (DWORD)nth->OptionalHeader.SizeOfImage;
	}
	BaseAddress = Memory_Start;
}


void AirMemory::CreateMemoryDump(){
	BYTE *NewMemory = (BYTE *)malloc(Memory_End - Memory_Start);
	RtlCopyMemory(NewMemory, (DWORD *)Memory_Start, Memory_End - Memory_Start);
	MemoryDump = (DWORD)NewMemory;
}

bool AirMemory::FullAccess(DWORD dwAddr, DWORD dwSize){
	if(VirtualProtect((DWORD *)dwAddr, dwSize, PAGE_EXECUTE_READWRITE, &OldProtect)){
		OldAddr = dwAddr;
		OldSize = dwSize;
		return true;
	}
	return false;
}

bool AirMemory::RestoreProtect(){
	DWORD old;
	return VirtualProtect((DWORD *)OldAddr, OldSize, OldProtect, &old);
}

DWORD AirMemory::AobScan(char *Aob, int Result){
	BYTE b[MaxSize] = {0};
	bool mask[MaxSize] = {0};
	DWORD dwSize = Air::ReadAob(Aob, b, mask);
	DWORD i, j, ResultCount = 0;

	if(dwSize < 1){
		return -1;
	}

	__try{
		for(i=Memory_Start; i<Memory_End; i++){
			for(j=0; j<dwSize; j++){
				if(mask[j]){
					continue;
				}
				if(b[j] != *(BYTE *)(i + j)){
					break;
				}
			}
			if(j == dwSize){
				ResultCount++;
				if(ResultCount >= Result){
					return i;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return 0;
	}

	return 0;
}

bool AirMemory::MemoryWriter(DWORD Address, char *Code){
	BYTE Memory[MaxSize] = {0};
	DWORD Size = Air::MakeBYTEArray(Memory, Code);

	if(Size < 1){
		return false;
	}

	if(IsDLL){
		if(Address < BaseAddress){
			Address = Address + BaseAddress;
		}
	}
	AddBackupMemory(Address, Memory, Size);
	return true;
}

bool AirMemory::RestoreMemory(DWORD Address){
	if(IsDLL){
		if(Address < BaseAddress){
			Address = Address + BaseAddress;
		}
	}
	return DeleteBackupMemory(Address);
}

bool AirMemory::WriteHook(DWORD dwPrev, WORD OpCode, void(*dwNext)(), DWORD *RetAddr, DWORD dwAdd){
	DWORD dwSize;
	if(OpCode < 0x100){
		dwSize = 5;
	}
	else{
		dwSize = 6;
	}

	if(IsDLL){
		if(dwPrev < BaseAddress){
			dwPrev = dwPrev + BaseAddress;
		}
	}

	FullAccess(dwPrev, dwSize + dwAdd);
	__try{
		if(OpCode < 0x100){
			*(BYTE *)(dwPrev) = OpCode;
			*(DWORD *)(dwPrev + 1) = (DWORD)dwNext - dwPrev - dwSize;
		}
		else{
			*(WORD *)(dwPrev) = OpCode;
			*(DWORD *)(dwPrev + 2) = (DWORD)dwNext - dwPrev - dwSize;
		}
		for(int i = 0; i<dwAdd; i++){
			*(BYTE *)(dwPrev + dwSize + i) = 0x90;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return false;
	}
	RestoreProtect();

	if(RetAddr){
		*RetAddr = dwPrev + dwSize + dwAdd;
	}
	return true;
}

void AirMemory::GetDumpInfo(DWORD *MS, DWORD *ME, DWORD *MD){
	*MS = Memory_Start;
	*ME = Memory_End;
	*MD = MemoryDump;
}

DWORD AirMemory::GetBaseAddress(){
	return BaseAddress;
}

void AirMemory::PointerHook(DWORD dwPointer, void(*NewFunction)(), DWORD *OldFunction){
	if(IsDLL){
		if(dwPointer < BaseAddress){
			dwPointer = dwPointer + BaseAddress;
		}
	}
	FullAccess(dwPointer, 4);//v1.01
	*OldFunction = *(DWORD *)dwPointer;
	*(DWORD *)dwPointer = (DWORD)NewFunction;
	RestoreProtect();//v1.01
}

DWORD AirMemory::AutoVMHook(DWORD dwFunction, void(*dwNext)(), DWORD *RetAddr, DWORD dwMinAddr){
	DWORD i, dwVMSection, dwVMSection_Ret;

	if(IsDLL){
		if(dwFunction < BaseAddress){
			dwFunction = dwFunction + BaseAddress;
		}
	}

	for(i=dwFunction; ;i++){
		if(*(BYTE *)i == JMP){
			dwVMSection = *(DWORD *)(i + 1) + i + 5;
			if((dwVMSection > Memory_Start) && (dwVMSection < Memory_End)){
				break;
			}
		}
	}

	for(i=dwVMSection; ;i++){
		switch(*(BYTE *)i){
		case JMP:
			dwVMSection_Ret =  *(DWORD *)(i + 1) + i + 5;
			if((dwVMSection_Ret > Memory_Start) && (dwVMSection_Ret < Memory_End)){
				if(dwMinAddr && i < dwMinAddr){
					i = dwVMSection_Ret;
					break;
				}
				WriteHook(i, JMP, dwNext);
				*RetAddr = dwVMSection_Ret;
				return i;
			}
			break;

		case CALL:
			dwVMSection_Ret =  *(DWORD *)(i + 1) + i + 5;
			if((dwVMSection_Ret > Memory_Start) && (dwVMSection_Ret < Memory_End)){
				if(dwMinAddr && i < dwMinAddr){
					i = dwVMSection_Ret;
					break;
				}
				WriteHook(i, CALL, dwNext);
				*RetAddr = dwVMSection_Ret;
				return i;
			}
			break;

		default:
			break;
		}
	}
}

void AirMemory::AddBackupMemory(DWORD Address, void *Memory, DWORD Size){
	BackupMemory *p = (BackupMemory *)malloc(sizeof(BackupMemory));

	p->Address = Address;
	p->Size = Size;
	p->Memory = malloc(Size);
	MemoryCopy((DWORD)p->Memory, (void *)p->Address, p->Size);//ƒƒ‚ƒŠ•Û‘¶
	MemoryCopy(p->Address, Memory, p->Size);//ƒƒ‚ƒŠ‘‚«ž‚Ý

	if(!MemoryList.Next){
		p->Next = NULL;
		MemoryList.Next = p;
	}
	else{
		p->Next = MemoryList.Next;
		MemoryList.Next = p;
	}
}


bool AirMemory::DeleteBackupMemory(DWORD Address){
	BackupMemory *p, *d;

	for(p=MemoryList.Next; p; p=p->Next){
		if(p->Next){
			if(p->Next->Address = Address){
				d = p->Next;
				MemoryCopy(d->Address, d->Memory, d->Size);//ƒƒ‚ƒŠ‚ð–ß‚·
				p->Next = p->Next->Next;
				free(d->Memory);
				free(d);
				return true;
			}
		}
	}
	return false;
}


void AirMemory::MemoryCopy(DWORD Address, void *Memory, DWORD Size){
	__try{
		FullAccess(Address, Size);
		RtlCopyMemory((void *)Address, Memory, Size);
		RestoreProtect();
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return;
	}
}