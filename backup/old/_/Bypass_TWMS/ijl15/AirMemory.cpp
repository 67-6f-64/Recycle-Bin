#include"AirMemory.h"

namespace Air{
	const char HexArray[] = "0123456789ABCDEF0123456789abcdef";
}

bool Air::FullAccess(DWORD dwAddress, DWORD dwSize){
	DWORD dw;

	return VirtualProtect((void *)dwAddress, dwSize, PAGE_EXECUTE_READWRITE, &dw);
}

bool Air::GetAddresses(LPSTR lpModuleName, DWORD *Memory_Start, DWORD *Memory_End){
	HMODULE hModule;
	MODULEINFO ModInfo;
	HANDLE hProcess = GetCurrentProcess();
	IMAGE_NT_HEADERS *nth;

	hModule = GetModuleHandleA(lpModuleName);

	if(!hModule){
		return false;
	}

	if(lpModuleName){//DLL
		GetModuleInformation(hProcess, hModule, &ModInfo, sizeof(MODULEINFO));
		*Memory_Start = (DWORD)ModInfo.lpBaseOfDll;
		*Memory_End = (DWORD)ModInfo.lpBaseOfDll + (DWORD)ModInfo.SizeOfImage;
	}
	else{//Client
		nth = (IMAGE_NT_HEADERS *)((DWORD)hModule + PIMAGE_DOS_HEADER(hModule)->e_lfanew);
		*Memory_Start = (DWORD)nth->OptionalHeader.ImageBase;
		*Memory_End = (DWORD)nth->OptionalHeader.ImageBase + (DWORD)nth->OptionalHeader.SizeOfImage;
	}

	return true;
}


void Air::MemoryDump(DWORD *Memory, DWORD Memory_Start, DWORD Memory_End){
	BYTE *Dump = (BYTE *)malloc(Memory_End - Memory_Start);
	RtlCopyMemory(Dump, (void *)Memory_Start, Memory_End - Memory_Start);

	*Memory = (DWORD)Dump;
}

bool Air::CreateMemoryDump(DWORD *Memory, DWORD *Memory_Start, DWORD *Memory_End, LPSTR lpModuleName){

	if(!GetAddresses(lpModuleName, Memory_Start, Memory_End)){
		return false;
	}

	MemoryDump(Memory, *Memory_Start, *Memory_End);

	return true;
}


int Air::StringtoByteArray(BYTE *bCode, char *Code){
	int iSize = 0, i, j;

	for(i=0; Code[i]; i++){
		for(j=0; HexArray[j]; j++){
			if(Code[i] == HexArray[j]){
				if(iSize%2 == 0){
					bCode[iSize/2] = (j%0x10)*0x10;
				}
				else{
					bCode[iSize/2] += j%0x10;
				}
				iSize++;
				break;
			}
		}
	}

	if(iSize%2){
		return 0;
	}

	return (iSize/2);
}


bool Air::WriteCode(DWORD dwAddress, char *Code){
	BYTE bCode[ByteArrayMaxSize] = {0};
	int iSize = StringtoByteArray(bCode, Code);

	if(iSize){
		FullAccess(dwAddress, iSize);
		__try{
			RtlCopyMemory((void *)dwAddress, bCode, iSize);
		}
		__except(EXCEPTION_EXECUTE_HANDLER){
			return false;
		}
		return true;
	}

	return false;
}


bool Air::WriteCodeAtAPI(LPSTR lpModuleName, LPSTR lpProcName, char *Code){
	return WriteCode((DWORD)GetProcAddress(GetModuleHandleA(lpModuleName), lpProcName), Code);
}


bool Air::WriteCodeAtModule(LPSTR lpModuleName, DWORD dwPosition, char *Code){
	return WriteCode((DWORD)GetModuleHandleA(lpModuleName) + dwPosition, Code);
}


DWORD Air::GetAPIAddress(LPSTR lpModuleName, LPSTR lpProcName){
	return (DWORD)GetProcAddress(GetModuleHandleA(lpModuleName), lpProcName);
}


int Air::CreateAob(BYTE *bCode, bool *bMask, char *Aob){
	int iSize = 0, i, j;
	for(i=0; Aob[i]; i++){
		if(Aob[i] == '?'){
			FullAccess((DWORD)&Aob[i], 1);
			Aob[i] = '0';
			bMask[iSize/2] = true;
		}
		for(j=0; HexArray[j]; j++){
			if(Aob[i] == HexArray[j]){
				if(iSize%2 == 0){
					bCode[iSize/2] = (j%0x10)*0x10;
				}
				else{
					bCode[iSize/2] += j%0x10;
				}
				iSize++;
				break;
			}
		}
	}

	if(iSize%2){
		return 0;
	}

	return (iSize/2);
}


DWORD Air::AobScan(char *Aob, int iResult, LPSTR lpModuleName){
	DWORD Memory_Start, Memory_End;
	DWORD i;
	int iSize, iMatch, iHit = 0;
	BYTE bCode[ByteArrayMaxSize] = {0};
	bool bMask[ByteArrayMaxSize] = {0};

	if(!GetAddresses(lpModuleName, &Memory_Start, &Memory_End)){
		return 0;
	}

	iSize = CreateAob(bCode, bMask, Aob);

	FullAccess(Memory_Start, Memory_End - Memory_Start);

	__try{
		for(i=Memory_Start; i<Memory_End; i++){
			for(iMatch=0; iMatch<iSize; iMatch++){
				if(bMask[iMatch] == true){
					continue;
				}
				if(*(BYTE *)(i+iMatch)^bCode[iMatch]){
					break;
				}
			}
			if(iMatch == iSize){
				iHit++;
				if(iHit >= iResult){
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


void Air::WriteJump(DWORD dwPrev, DWORD dwNext, DWORD *dwRet, int iNop){
	int i;
	FullAccess(dwPrev, 5 + iNop);
	*(BYTE *)(dwPrev) = 0xE9;
	*(DWORD *)(dwPrev +1) = dwNext - dwPrev -5;
	for(i=0; i<iNop; i++){
		*(BYTE *)(dwPrev + 5 + i) = 0x90;
	}
	if(dwRet){
		*dwRet = dwPrev + 5 + i;
	}
}


void Air::WriteCall(DWORD dwPrev, DWORD dwNext, DWORD *dwRet){
	FullAccess(dwPrev, 5);

	if(dwRet){
		*dwRet = *(DWORD *)(dwPrev+1) + dwPrev + 5;
	}

	*(BYTE *)(dwPrev) = 0xE8;
	*(DWORD *)(dwPrev +1) = dwNext - dwPrev -5;
}


void Air::WriteJumpAtModule(LPSTR lpModuleName, DWORD dwPosition, DWORD dwNext, DWORD *dwRet, int iNop){
	WriteJump((DWORD)GetModuleHandleA(lpModuleName) + dwPosition, dwNext, dwRet, iNop);
}