
--圣诞时装7天
function I4800028_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then

		role.AddRoleItem(MapID, InstanceID, TargetID, 8330101, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8330102, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8330103, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8330104, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8330111, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8330112, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8330113, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8330114, 1, -1, 8, 420)
	end
end

function I4800028_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800028, 1, "I4800028_GiftBag")
aux.RegisterItemEvent(4800028, 0, "I4800028_CanUseGiftBag")




--青铜回归包

function I4700011_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleExp(MapID, InstanceID, TargetID, 10000000)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221102, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211502, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or Vocation == 2 or Vocation == 6 or Vocation == 7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8017001, 1, -1, 8, 420)
	elseif Vocation == 3 or Vocation == 8 or Vocation == 9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8027001, 1, -1, 8, 420)
	elseif Vocation == 4 or Vocation == 10 or Vocation == 11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8047001, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8037001, 1, -1, 8, 420)
	end
end

function I4700011_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700011, 1, "I4700011_GiftBag")
aux.RegisterItemEvent(4700011, 0, "I4700011_CanUseGiftBag")


--白银回归包

function I4700012_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleExp(MapID, InstanceID, TargetID, 20000000)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221102, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211502, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or Vocation == 2 or Vocation == 6 or Vocation == 7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8017002, 1, -1, 8, 420)
	elseif Vocation == 3 or Vocation == 8 or Vocation == 9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8027002, 1, -1, 8, 420)
	elseif Vocation == 4 or Vocation == 10 or Vocation == 11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8047002, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8037002, 1, -1, 8, 420)
	end
end

function I4700012_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700012, 1, "I4700012_GiftBag")
aux.RegisterItemEvent(4700012, 0, "I4700012_CanUseGiftBag")


--黄金回归包

function I4700013_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleExp(MapID, InstanceID, TargetID, 30000000)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221002, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211502, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800028, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or Vocation == 2 or Vocation == 6 or Vocation == 7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8017002, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8017005, 1, -1, 8, 420)
	elseif Vocation == 3 or Vocation == 8 or Vocation == 9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8027002, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8027005, 1, -1, 8, 420)
	elseif Vocation == 4 or Vocation == 10 or Vocation == 11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8047002, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8047005, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8037002, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8037005, 1, -1, 8, 420)
	end
end

function I4700013_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700013, 1, "I4700013_GiftBag")
aux.RegisterItemEvent(4700013, 0, "I4700013_CanUseGiftBag")

--钻石回归包

function I4700014_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleExp(MapID, InstanceID, TargetID, 40000000)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221002, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211502, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800028, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or Vocation == 2 or Vocation == 6 or Vocation == 7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8017005, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8017005, 1, -1, 8, 420)
	elseif Vocation == 3 or Vocation == 8 or Vocation == 9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8027005, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8027005, 1, -1, 8, 420)
	elseif Vocation == 4 or Vocation == 10 or Vocation == 11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8047005, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8047005, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8037005, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8037005, 1, -1, 8, 420)
	end
end

function I4700014_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700014, 1, "I4700014_GiftBag")
aux.RegisterItemEvent(4700014, 0, "I4700014_CanUseGiftBag")





















