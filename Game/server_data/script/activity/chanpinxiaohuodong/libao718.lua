
function I4830106_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221502, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211102, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800029, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830111, 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830106, 1, "I4830106_GiftBag")

function I4830106_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830106, 0, "I4830106_CanUseGiftBag")


function I4830107_GiftBag(MapID, InstanceID, TypeID, TargetID)

	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700159, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700160, 1, -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830107, 1, "I4830107_GiftBag")

function I4830107_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830107, 0, "I4830107_CanUseGiftBag")


function I4830108_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221504, 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830108, 1, "I4830108_GiftBag")

function I4830108_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830108, 0, "I4830108_CanUseGiftBag")


function I4830109_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 9850201, 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830109, 1, "I4830109_GiftBag")

function I4830109_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830109, 0, "I4830109_CanUseGiftBag")


function I4830110_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 9850205, 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830110, 1, "I4830110_GiftBag")

function I4830110_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830110, 0, "I4830110_CanUseGiftBag")


function I4830111_GiftBag(MapID, InstanceID, TypeID, TargetID)

	local class = role.GetRoleClass(TargetID)
	if class == 2 or class == 6 or class == 7 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 8010021,1, -1, 8, 420)
	elseif class == 3 or class == 8 or class == 9 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 8020021,1, -1, 8, 420)
	elseif class == 5 or class == 12 or class == 13 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 8030021,1, -1, 8, 420)
	elseif class == 4 or class == 10 or class == 11 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 8040021,1, -1, 8, 420)
	end


end
aux.RegisterItemEvent(4830111, 1, "I4830111_GiftBag")

function I4830111_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	local class = role.GetRoleClass(TargetID)	--判别玩家职业
	if class == 1 then				--新手不能开启礼包
		--提示玩家职业不符
		bRet = 46
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830111, 0, "I4830111_CanUseGiftBag")




