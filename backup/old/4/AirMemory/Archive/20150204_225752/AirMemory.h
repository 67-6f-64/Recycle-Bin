#ifndef __AIRMEMORY_H__
#define __AIRMEMORY_H__

#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

#define MaxSize 256

#define CALL 0xE8
#define JMP 0xE9
#define JNE 0x850F
#define JE 0x840F

class AirMemory{
	private:
		typedef struct tagBackupMemory{
			DWORD Address;
			DWORD Size;
			void *Memory;
			tagBackupMemory *Next;
		}BackupMemory;

		DWORD BaseAddress;
		DWORD Memory_Start;
		DWORD Memory_End;
		DWORD MemoryDump;
		bool IsDLL;
		DWORD OldAddr;
		DWORD OldSize;
		DWORD OldProtect;
		BackupMemory MemoryList;

		bool FullAccess(DWORD Address, DWORD Size);
		bool RestoreProtect();
		void AddBackupMemory(DWORD Address, void *Memory, DWORD Size);
		bool DeleteBackupMemory(DWORD Address);
		void MemoryCopy(DWORD Address, void *Memory, DWORD Size);

	public:
		AirMemory();
		void Init(char *ModuleName = NULL);
		void CreateMemoryDump();
		DWORD AobScan(char *Aob, int Result = 0, DWORD StartAddress = 0, DWORD EndAddress = 0);
		DWORD GetAbsoluteAddress(DWORD Address);
		bool MemoryWriter(DWORD Address, char *Code);
		bool RestoreMemory(DWORD Address);
		bool WriteHook(DWORD Address, WORD OpCode, void *Function, DWORD *RetAddr = NULL, DWORD AddNop = 0);
		void GetDumpInfo(DWORD *MS, DWORD *ME, DWORD *MD);
		DWORD GetBaseAddress();
		void PointerHook(DWORD Pointer, void *Function, DWORD *OldFunction);
		DWORD AutoVMHook(DWORD Address, void *Function, DWORD *RetAddr, DWORD MinAddr = 0);
		bool FunctionHook(char *ProcName, void *Function, DWORD *RetAddr);
};

#endif