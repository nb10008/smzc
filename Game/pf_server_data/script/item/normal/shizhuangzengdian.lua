--雪山毛皮包[30天]
function I3090024_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8141011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8141012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8141013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8141014, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8142011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8142012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8142013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8142014, 1, -1, 8, 420)
	end
end

function I3090024_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090024, 1, "I3090024_GiftBag")
aux.RegisterItemEvent(3090024, 0, "I3090024_CanUseGiftBag")



--黄金龙纹包[30天]
function I3090025_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8143011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8143012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8143013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8143014, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8144011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8144012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8144013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8144014, 1, -1, 8, 420)
	end
end

function I3090025_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090025, 1, "I3090025_GiftBag")
aux.RegisterItemEvent(3090025, 0, "I3090025_CanUseGiftBag")



--紫幻苍穹包[30天]
function I3090026_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8145011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8145012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8145013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8145014, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8146011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8146012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8146013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8146014, 1, -1, 8, 420)
	end
end

function I3090026_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090026, 1, "I3090026_GiftBag")
aux.RegisterItemEvent(3090026, 0, "I3090026_CanUseGiftBag")



--火舞霓裳包[30天]
function I3090027_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8147011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8147012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8147013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8147014, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8148011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8148012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8148013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8148014, 1, -1, 8, 420)
	end
end

function I3090027_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090027, 1, "I3090027_GiftBag")
aux.RegisterItemEvent(3090027, 0, "I3090027_CanUseGiftBag")



--丛林魅影包[30天]
function I3090028_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8149011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8149012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8149013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8149014, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8150011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8150012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8150013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8150014, 1, -1, 8, 420)
	end
end

function I3090028_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090028, 1, "I3090028_GiftBag")
aux.RegisterItemEvent(3090028, 0, "I3090028_CanUseGiftBag")




--100赠点
function I3090029_zengquan(MapID, InstanceID, TypeID, TargetID)
	--修改赠点
	role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 100)
end
aux.RegisterItemEvent(3090029, 1, "I3090029_zengquan")



--1000赠点
function I3090030_zengquan(MapID, InstanceID, TypeID, TargetID)
	--修改赠点
	role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 1000)
end
aux.RegisterItemEvent(3090030, 1, "I3090030_zengquan")












