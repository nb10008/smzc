function I4530157_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 8060709, 1, 4, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530158, 1, -1, 8, 420)
end

function I4530157_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530157, 1, "I4530157_GiftBag")
aux.RegisterItemEvent(4530157, 0, "I4530157_CanUseGiftBag")


function I4530158_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 8070709, 1, 4, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530159, 1, -1, 8, 420)
end

function I4530158_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530158, 1, "I4530158_GiftBag")
aux.RegisterItemEvent(4530158, 0, "I4530158_CanUseGiftBag")




function I4530159_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 8080709, 1, 4, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530160, 1, -1, 8, 420)
end

function I4530159_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530159, 1, "I4530159_GiftBag")
aux.RegisterItemEvent(4530159, 0, "I4530159_CanUseGiftBag")




function I4530160_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 8090709, 1, 4, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530161, 1, -1, 8, 420)
end

function I4530160_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530160, 1, "I4530160_GiftBag")
aux.RegisterItemEvent(4530160, 0, "I4530160_CanUseGiftBag")




function I4530161_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 8130711, 1, 4, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530162, 1, -1, 8, 420)
end

function I4530161_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530161, 1, "I4530161_GiftBag")
aux.RegisterItemEvent(4530161, 0, "I4530161_CanUseGiftBag")




function I4530162_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 8110709, 1, 4, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530163, 1, -1, 8, 420)
end

function I4530162_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530162, 1, "I4530162_GiftBag")
aux.RegisterItemEvent(4530162, 0, "I4530162_CanUseGiftBag")




function I4530163_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530164, 1, -1, 8, 420)
	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8160006, 1, 4, 8, 420)
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8170007, 1, 4, 8, 420)
	end
end

function I4530163_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530163, 1, "I4530163_GiftBag")
aux.RegisterItemEvent(4530163, 0, "I4530163_CanUseGiftBag")




function I4530164_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530165, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, 70, -1, 8, 420)
end

function I4530164_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530164, 1, "I4530164_GiftBag")
aux.RegisterItemEvent(4530164, 0, "I4530164_CanUseGiftBag")



function I4530165_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530166, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800041, 5, -1, 8, 420)


end

function I4530165_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530165, 1, "I4530165_GiftBag")
aux.RegisterItemEvent(4530165, 0, "I4530165_CanUseGiftBag")



function I4530166_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, 100, -1, 8, 420)
end

function I4530166_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530166, 1, "I4530166_GiftBag")
aux.RegisterItemEvent(4530166, 0, "I4530166_CanUseGiftBag")
