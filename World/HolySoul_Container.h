//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: HolySoul.h
// author: zxzhang
// actor:
// data: 2011-6-8
// last:
// brief: 元神装备容器
//-----------------------------------------------------------------------------

#ifndef  HOLYSOUL_Container_H
#define  HOLYSOUL_Container_H

class HolySoulContainer : public EquipContainer
{
public:
	HolySoulContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize);
	virtual ~HolySoulContainer();
};



#endif