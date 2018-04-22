#include<Windows.h>
#include<stdio.h>

class AirProtect{
	private:
		DWORD AirTable;
	public:
		bool AirCave(DWORD dwKey);
		DWORD InitAirTable(DWORD dwSize);
		AirProtect();
};

AirProtect::AirProtect(){
	AirTable = NULL;
	//LPBYTE bMemory = new BYTE[0x05*0x02*0x10];
}

bool _declspec(naked) AirProtect::AirCave(DWORD dwKey){
	_asm{
		mov eax,0x00000001
		ret 0x0004
	}
}

DWORD AirProtect::InitAirTable(DWORD dwSize){
	BYTE *bMemory = new BYTE[0x05*dwSize];

	DWORD SwitchTable[0x10] = {(DWORD)IsDebuggerPresent, (DWORD)FatalExit};


	bMemory[0x00] = 0xE8;
	*(DWORD *)(&bMemory[0x01]) = SwitchTable[0]-(DWORD)&bMemory[0x00]-5;//Function


	AirTable = (DWORD)&bMemory[0];

	return AirTable;
}

void _declspec(naked) AntiDebug(){
	/*_asm{
		pushad
		xor esp,esp
		popad
		jmp FatalExit
	}*/
	printf("Hello World!\n");
	_asm{
		ret
	}
}

DWORD rwbwerbn = ((DWORD)AntiDebug^0x1337) - (0x1337 << 13);

char ce[0x100] = "Cheat Engine";
void  SearchDebugger(){
	_asm{
		mov eax,fs:[0x18]
		mov eax,[eax+0x30]
		movzx eax,[eax+0x02]
		test eax,eax
		//je Ending
		push ce
		push eax
		call FindWindowA
		test eax,eax
		je Ending
		mov eax,[rwbwerbn]
		mov ebx,0x1337
		rol ebx,13
		add eax,ebx
		xor eax,0x1337
		call eax
Ending:
		ret
	}

}

int main(){
	int d;
	printf("AirProtect\n\n");

	//AirProtect Air;

	//printf("AirTable:%p\n", Air.InitAirTable(0x10));
	while(1){
	SearchDebugger();
	Sleep(100);
	}
	printf("Success\n\n");
	scanf("%d", &d);
	return 0;
}