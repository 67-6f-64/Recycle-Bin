#include"Address.h"

DWORD SendFunction = 0x00478780;
DWORD FaggotReturn = SendFunction+0x10;
DWORD SendHookAddr = 0x01B96CBF;
DWORD SendHookReturn = 0x018A81BC;

DWORD RecvHookPointer = 0x0128E094;
DWORD RecvHookPointer_Org = *(DWORD *)RecvHookPointer;
DWORD RecvHookEsp = 0x0057ED61;