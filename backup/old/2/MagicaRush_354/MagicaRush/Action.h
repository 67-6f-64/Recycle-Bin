#ifndef __ACTION_H__
#define __ACTION_H__

#include<Windows.h>

#define ACTION_DEFAULT 0
#define ACTION_GETREWARD 100
#define ACTION_OPENSHOP 200
#define ACTION_SELLREWARD 300
#define ACTION_CLOSESHOP 400
#define ACTION_CLOSINGSHOP 401

#define ACTION_MESOMAX 1000
#define ACTION_200P 1001
#define ACTION_EXCEPTION -1

#define ACTION_MAPTELEPORT 600
#define ACTION_MAPTELEPORTING 601

#define ACTION_LOGIN 700
#define ACTION_WORLDSELECT 701
#define ACTION_CHARACTERSELECT 702

#define ACTION_EXIT_CLOSINGSHOP 2000
#define ACTION_EXIT 2001

extern DWORD MesoAction;

void gui_update_action(DWORD Action);
void gui_update_meso(ULONGLONG Meso);
#endif