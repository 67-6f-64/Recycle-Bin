#ifndef __GUI_PE_H__
#define __GUI_PE_H__

void gui_pe(HINSTANCE hInstance);
void _stdcall AddLog(DWORD Type, DWORD dwReturn, DWORD Length, BYTE *Packet, char *Format);

#endif