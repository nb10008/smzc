

--17173尊享卡
function I4530115_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520020, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520031, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)

	--判断玩家性别

	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then

		role.AddRoleItem(MapID, InstanceID, TargetID, 4210601, 1, -1, 8, 420)
	else

		role.AddRoleItem(MapID, InstanceID, TargetID, 4210701, 1, -1, 8, 420)
	end
end


function I4530115_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530115, 1, "I4530115_GiftBag")
aux.RegisterItemEvent(4530115, 0, "I4530115_CanUseGiftBag")



--多玩尊享卡
function I4530116_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520036, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520038, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520031, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211001, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)

end


function I4530116_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530116, 1, "I4530116_GiftBag")
aux.RegisterItemEvent(4530116, 0, "I4530116_CanUseGiftBag")




--新浪尊享卡
function I4530117_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520031, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)

	--判断玩家性别

	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then

		role.AddRoleItem(MapID, InstanceID, TargetID, 4210801, 1, -1, 8, 420)
	else

		role.AddRoleItem(MapID, InstanceID, TargetID, 4210901, 1, -1, 8, 420)
	end
end


function I4530117_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530117, 1, "I4530117_GiftBag")
aux.RegisterItemEvent(4530117, 0, "I4530117_CanUseGiftBag")




--网易尊享卡
function I4530118_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520026, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520031, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8200004, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)

end


function I4530118_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530118, 1, "I4530118_GiftBag")
aux.RegisterItemEvent(4530118, 0, "I4530118_CanUseGiftBag")




--腾讯尊享卡
function I4530119_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520037, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520039, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520031, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8200007, 1, 3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)

end


function I4530119_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530119, 1, "I4530119_GiftBag")
aux.RegisterItemEvent(4530119, 0, "I4530119_CanUseGiftBag")





--游久尊享卡
function I4530120_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520031, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8200002, 1, 4, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)

end


function I4530120_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530120, 1, "I4530120_GiftBag")
aux.RegisterItemEvent(4530120, 0, "I4530120_CanUseGiftBag")





--三界风情卡
function I4530121_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070034, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070040, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070068, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)

end


function I4530121_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530121, 1, "I4530121_GiftBag")
aux.RegisterItemEvent(4530121, 0, "I4530121_CanUseGiftBag")







