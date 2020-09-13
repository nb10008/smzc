function I4800279_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(100)
	local temp = math.random(100)


	if r <= 60 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3110007, 1, -1, 8, 420)
	elseif r <= 95 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400024, 1, -1, 8, 420)
	elseif r <= 100 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090026, 1, -1, 8, 420)
	end


	role.AddRoleItem(MapID, InstanceID, TargetID, 4820229, 1, -1, 8, 420)

	if 	 temp <= 10 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800266, 5, -1, 8, 420)
	elseif temp <= 20 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800266, 4, -1, 8, 420)
	elseif temp <= 50 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800266, 3, -1, 8, 420)
	elseif	temp <= 80 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800266, 2, -1, 8, 420)
	elseif temp <= 100 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800266, 1, -1, 8, 420)
	end

end

function I4800279_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800279, 1, "I4800279_GiftBag")
aux.RegisterItemEvent(4800279, 0, "I4800279_CanUseGiftBag")
