function I4800238_GiftBag(MapID, InstanceID, TypeID, TargetID)

	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800239, 1, 101)
    local k = math.random(100)
	if k <= 5 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090026, 1, -1, 8, 420)
	elseif k <= 15 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4090027, 1, -1, 8, 420)
	elseif k <= 35 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800240, 1, -1, 8, 420)
	elseif k <= 75 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 3020027, 1, -1, 8, 420)
	elseif k <= 77 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 3020030, 1, -1, 8, 420)
	elseif k <= 87 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4820250, 1, -1, 8, 420)
	elseif k <= 97 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4820047, 1, -1, 8, 420)
	elseif k <= 98 then
	    role.AddRoleSilver(MapID, InstanceID, TargetID, 10000000, 420)
    else
	    role.AddRoleSilver(MapID, InstanceID, TargetID, 5000000, 420)
	end

end

function I4800238_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	local b = role.GetRoleItemNum(TargetID,4800239)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	if(b < 1) then
		--提示玩家背包空间不足
		bRet = 103
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4800238, 1, "I4800238_GiftBag")
aux.RegisterItemEvent(4800238, 0, "I4800238_CanUseGiftBag")
