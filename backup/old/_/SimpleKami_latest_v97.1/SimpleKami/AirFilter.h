#ifndef __AirFilter_H__
#define __AirFilter_H__

#include"lib/Asm.h"
#include<stdio.h>

namespace AIF{
	int LoadItemList();
	int LoadItemList2();
	void ItemFilter(BOOL Flag);
	void ItemFilter2(BOOL Flag);
}

#endif