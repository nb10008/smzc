

#include "StdAfx.h"


#include "container_template.h"
#include "../WorldDefine/container_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "container_restrict.h"
#include "HolySoul_Container.h"

HolySoulContainer::HolySoulContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize)
:EquipContainer(eConType, n16CurSize, n16MaxSize)
{

}

HolySoulContainer::~HolySoulContainer()
{

}