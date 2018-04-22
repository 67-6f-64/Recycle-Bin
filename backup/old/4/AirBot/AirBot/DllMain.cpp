#include"AirBot.h"

AirWindow AW;
AirMemory AM, DX9M;

char NameList[33][16] = {
	"メラ", "メラミ", "メラゾーマ",
	"ギラ", "ベギラマ", "ベギラゴン",
	"イオ", "イオラ", "イオナズン",
	"ヒャド", "ヒャダルコ", "マヒャド",
	"バギ", "バギマ", "バギクロス",
	"デイン", "ライデイン", "ギガデイン",
	"ザキ", "ザラキ", "ザラキーマ",
	"ホイミ", "ベホイミ", "ベホマ", "ベホマラー", "ベホマズン",
	"キアリー", "キアリク",
	"ザオラル", "ザオリク",
	"ルーラ", "リレミト", "ラナルータ"
};

char TailList[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

DWORD x = 123456789, y = 362436069, z = 521288629, w = GetTickCount();

DWORD xor128(){ 
	DWORD t=(x^(x<<11));
	x=y;
	y=z;
	z=w;
	return(w=(w^(w>>19))^(t^(t>>8))); 
}

void _stdcall NameGenerator(char Name[], DWORD *NameSize){
	DWORD r = xor128()%32;
	int i;
	for(i=0; NameList[r][i]; i++){
		Name[i] = NameList[r][i];
	}
	for(; i < 12; i++){
		Name[i] = TailList[xor128()%26];
	}
	Name[i] = 0;
	*NameSize = 12;
}

DWORD IN_Push;
DWORD IN_Ret;

void _declspec(naked) IN_Hook(){
	_asm{
		mov eax,[esp+0x04]
		mov eax,[eax]
		cmp dword ptr [eax-0x04],0
		jne IN_End
		pushad
		lea ebx,[eax-0x04]
		push ebx
		push eax
		call NameGenerator
		popad
IN_End:
		push -1
		push [IN_Push]
		jmp dword ptr [IN_Ret]
	}
}

void _stdcall AdjustWindow(int cx, int cy){
	AW.AdjustWindow(cx + 200, cy);
}

DWORD RS_Ret;
void _declspec(naked) Resize_Hook(){
	_asm{
		pushad
		push ecx
		push edx
		call AdjustWindow
		popad
		mov eax,[esi+0xB4]
		jmp dword ptr [RS_Ret]
	}
}

void _stdcall WindowHook(HWND hWnd){
	DX9M.Init("GR2D_DX9.DLL");
	DX9M.MemoryWriter(0x7E10, "31 C0 C3");
	DX9M.WriteHook(0x61D2, JMP, Resize_Hook, &RS_Ret, 1);
	AW.EmbedWindow(hWnd);
}

DWORD WC_Ret;
void _declspec(naked) WC_Hook(){
	_asm{
		mov eax,[ecx+0x08]
		pushad
		push eax
		call WindowHook
		popad
		push ebp
		mov ebp,esp
		sub esp,0x0C
		jmp dword ptr [WC_Ret]
	}
}

void Create(HWND hWnd){
}

DWORD Hook_Ret;
void _declspec(naked) Hook(){
	_asm{
		xor eax,eax
		jmp dword ptr [Hook_Ret]
	}
}

AirMemory User32;
AirMemory Maple;

void _stdcall ArgHook(DWORD esp){
}

void AirBot(HINSTANCE hInstance){
	/*
	char Title[256];
	AM.Init();
	IN_Push = *(DWORD *)(0x0096C590 + 3);
	AM.WriteHook(0x0096C590, JMP, IN_Hook, &IN_Ret, 2);
	AM.WriteHook(0x0155BF60, JMP, WC_Hook, &WC_Ret, 1);

	Air::AirPrint(Title, "AirBot:%w", GetCurrentProcessId());
	AW.Init(hInstance, Title, 800 + 200, 600, true, Create);
	TerminateProcess(GetCurrentProcess(), 0);
	*/
	User32.Init("user32.dll");
	User32.FunctionHook("CreateWindowExA", Hook, &Hook_Ret);
	Maple.Init();
	Maple.ArgumentsHook(0x00414590, 7, ArgHook);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateThread(NULL, 1024, (LPTHREAD_START_ROUTINE)AirBot, hinstDLL ,NULL, NULL);

	return TRUE;
}