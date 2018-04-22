#ifndef __SimpleKami_H__
#define __SimpleKami_H__

#include"lib/Asm.h"
#include<stdio.h>
#include<math.h>

namespace Kami{
	extern int RangeX;
	extern int RangeY;
	extern int CCval;
	extern int Action;
	extern BOOL Breath;
	extern BOOL CC;;
	extern int Channel;
	extern int CCTime;
	extern int oldCCval;
}

namespace Kami{
	void WRYYY(BOOL Flag);
	void BreathTimer();
	void CCTimer();
}

#endif