#ifndef __GUI_H__
#define __GUI_H__

#include"AirWindow.h"

void gui(HINSTANCE hinstDLL);

extern bool bDo;

void gui_update_map(DWORD MapID);
void gui_update_channel(BYTE Channel);
void gui_update_meso(DWORD Meso1, DWORD Meso2);
void gui_update_map2();
void gui_update_name(char *Name);

extern bool bTeleporting;
extern bool bExpExploit;
extern bool bSelfSelect;

#endif