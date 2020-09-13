function I4700015_giftbag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 1, -1, 8, 420)
	local n = math.random(1,30)
	if n == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020012, 1, -1, 8, 420)
	end
end
aux.RegisterItemEvent(4700015, 1, "I4700015_giftbag")


function I4700015_canuse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4700015, 0, "I4700015_canuse")



function I4700016_giftbag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400024, 1, -1, 8, 420)
	local n = math.random(1,15)
	if n == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020012, 1, -1, 8, 420)
	end


end
aux.RegisterItemEvent(4700016, 1, "I4700016_giftbag")


function I4700016_canuse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4700016, 0, "I4700016_canuse")
