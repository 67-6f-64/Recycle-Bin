#include"AirCRC.h"

namespace Air{
	bool FullAccess(DWORD dwAddress, DWORD dwSize);
	bool GetAddresses(LPSTR lpModuleName, DWORD *Memory_Start, DWORD *Memory_End);
	void MemoryDump(DWORD *Memory, DWORD Memory_Start, DWORD Memory_End);

	void VMCRC1(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End);
	void VMCRC2(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End);
	void VMCRC3(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End);
	void VMCRC4(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End);
	void MSCRC1(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End);
}

class MemoryWriter{
	private:
		DWORD dwMS;//ämï€ÇµÇΩÉÅÉÇÉä
		DWORD dwCA;//èëÇ´çûÇ›êÊ
		DWORD dwHS;//CRC
		DWORD dwHE;
		DWORD dwFR;
		DWORD dwJM;
		DWORD dwCS;

	public:
		void Init(DWORD dwAddr, DWORD dwCodeSize, DWORD dwMeS, DWORD dwMeE);
		void Write1(BYTE bCode);
		void Write2(WORD wCode);
		void Write3(DWORD dwCode);
		void Write4(DWORD dwCode);
		void Copy(DWORD dwSource, DWORD dwSize);
		void Finish();
		void WriteJump(DWORD dwPrev, DWORD dwNext);
};

void MemoryWriter::Init(DWORD dwAddr, DWORD dwCodeSize, DWORD dwMeS, DWORD dwMeE){
	dwCA = (DWORD)new BYTE[0x100];
	dwMS = dwCA;
	Air::FullAccess(dwCA, 0x100);
	Air::FullAccess(dwAddr, 0x100);

	dwHS = dwAddr;

	int i = 0;

	while(1){
		if(*(BYTE *)(dwHS + i) == 0xE8 || *(BYTE *)(dwHS + i) == 0xE9){
			dwHE = dwHS + i;
			dwFR = dwHE + 5;
			dwCS = dwCodeSize;
			dwJM = *(DWORD *)(dwHE+ 1) + dwHE + 5;
			if((dwJM < dwMeE) && (dwJM > dwMeS)){
				break;
			}
		}
		i++;
	}
}

void MemoryWriter::Write1(BYTE bCode){
	*(BYTE *)(dwCA) = bCode;
	dwCA++;
}

void MemoryWriter::Write2(WORD wCode){
	*(WORD *)(dwCA) = wCode;
	dwCA += 2;
}

void MemoryWriter::Write3(DWORD dwCode){
	Copy((DWORD)&dwCode, 3);
}

void MemoryWriter::Write4(DWORD dwCode){
	*(DWORD *)(dwCA) = dwCode;
	dwCA += 4;
}

void MemoryWriter::Copy(DWORD dwSource, DWORD dwSize){
	RtlCopyMemory((void *)dwCA, (void *)dwSource, dwSize);
	dwCA += dwSize;
}

void MemoryWriter::WriteJump(DWORD dwPrev, DWORD dwNext){
	*(BYTE *)(dwPrev) = 0xE9;
	*(DWORD *)(dwPrev + 1) = dwNext - dwPrev - 5;
}

void MemoryWriter::Finish(){
	Copy(dwHS + dwCS, dwHE - dwHS - dwCS);

	
	if(*(BYTE *)(dwHE) == 0xE8){
		Write1(0x68);
		Write4(dwFR);
	}

	Write1(0xE9);
	Write4(dwJM - dwCA - 5 +1);
	
	WriteJump(dwHS, dwMS);

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

void Air::VMCRC1(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End){
	MemoryWriter MW;
	
	MW.Init(dwAddress, 2, Memory_Start, Memory_End);
	MW.Write1(0x52);
	MW.Write2(0xFA81);
	MW.Write4(Memory_Start);
	MW.Write2(0x01472);
	MW.Write2(0xFA81);
	MW.Write4(Memory_End);
	MW.Write2(0x0C77);
	MW.Write2(0xEA81);
	MW.Write4(Memory_Start);
	MW.Write2(0xC281);
	MW.Write4(Memory);
	MW.Write2(0x0232);//xor al,[edx]
	MW.Write1(0x5A);
	MW.Finish();
}

void Air::VMCRC2(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End){
	MemoryWriter MW;
	
	MW.Init(dwAddress, 3, Memory_Start, Memory_End);
	MW.Write1(0x50);
	MW.Write1(0x3D);
	MW.Write4(Memory_Start);
	MW.Write2(0x01172);
	MW.Write1(0x3D);
	MW.Write4(Memory_End);
	MW.Write2(0x0A77);
	MW.Write1(0x2D);
	MW.Write4(Memory_Start);
	MW.Write1(0x05);
	MW.Write4(Memory);
	MW.Write3(0x008B66);//mov ax,[eax]
	MW.Write4(0x24048966);
	MW.Write1(0x58);
	MW.Finish();
}

void Air::VMCRC3(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End){
	MemoryWriter MW;
	
	MW.Init(dwAddress, 2, Memory_Start, Memory_End);
	MW.Write1(0x3D);
	MW.Write4(Memory_Start);
	MW.Write2(0x01172);
	MW.Write1(0x3D);
	MW.Write4(Memory_End);
	MW.Write2(0x0A77);
	MW.Write1(0x2D);
	MW.Write4(Memory_Start);
	MW.Write1(0x05);
	MW.Write4(Memory);
	MW.Write2(0x008B);//mov eax,[eax]
	MW.Finish();
}

void Air::VMCRC4(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End){
	MemoryWriter MW;
	
	MW.Init(dwAddress, 2, Memory_Start, Memory_End);
	MW.Write1(0x52);
	MW.Write2(0xFA81);
	MW.Write4(Memory_Start);
	MW.Write2(0x01472);
	MW.Write2(0xFA81);
	MW.Write4(Memory_End);
	MW.Write2(0x0C77);
	MW.Write2(0xEA81);
	MW.Write4(Memory_Start);
	MW.Write2(0xC281);
	MW.Write4(Memory);
	MW.Write2(0x028A);//mov al,[edx]
	MW.Write1(0x5A);
	MW.Finish();
}

void Air::MSCRC1(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End){
	MemoryWriter MW;
	
	MW.Init(dwAddress, 2, Memory_Start, Memory_End);
	MW.Write1(0x51);
	MW.Write2(0xF981);
	MW.Write4(Memory_Start);
	MW.Write2(0x01472);
	MW.Write2(0xF981);
	MW.Write4(Memory_End);
	MW.Write2(0x0C77);
	MW.Write2(0xE981);
	MW.Write4(Memory_Start);
	MW.Write2(0xC181);
	MW.Write4(Memory);
	MW.Write2(0x118A);//mov dl,[ecx]
	MW.Write1(0x59);
	MW.Finish();
}

void Air::BypassCRC(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End){

	switch(*(WORD *)(dwAddress)){
		case 0x0232://xor al,[edx]
			VMCRC1(dwAddress, Memory, Memory_Start, Memory_End);
			break;

		case 0x8B66://mov ax,[eax]
			VMCRC2(dwAddress,Memory, Memory_Start, Memory_End);
			break;

		case 0x008B://mov eax,[eax]
			VMCRC3(dwAddress,Memory, Memory_Start, Memory_End);
			break;

		case 0x028A://mov al,[edx]
			VMCRC4(dwAddress,Memory, Memory_Start, Memory_End);
			break;

		case 0x118A://mov dl,[ecx]
			MSCRC1(dwAddress, Memory, Memory_Start, Memory_End);
			break;

		default:
			MessageBoxA(NULL, "Undefined CRC", "ERROR", NULL);
			break;
	}

}