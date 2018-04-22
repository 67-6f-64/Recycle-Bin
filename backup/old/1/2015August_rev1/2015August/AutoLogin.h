#ifndef __AUTOLOGIN_H__
#define __AUTOLOGIN_H__

void AutoLogin_Hook();
void UpdatePacketAL(SPacket *p);

extern bool bAutoLogin;
extern DWORD LoginPacketCount;

void ReplyPacketAL(RPacket *rp);

#endif