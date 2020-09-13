function I4040027_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local k = math.random(1000)
	if k <= 400 then
	role.AddRoleSilver(MapID, InstanceID, TargetID, 5000, 102)
	elseif k > 400 and k <= 500 then
	role.AddRoleSilver(MapID, InstanceID, TargetID, 20000, 102)
	elseif k > 500 and k <= 550 then
	role.AddRoleSilver(MapID, InstanceID, TargetID, 40000, 102)
	elseif k > 550 and k <= 750 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4040026, 1, -1, 8, 420)
	elseif k > 750 and k <= 850 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4040026, 2, -1, 8, 420)
	elseif k > 850 and k <= 900 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030104, 1, -1, 8, 420)
	elseif k > 900 and k <= 950 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030105, 1, -1, 8, 420)
	elseif k > 950 and k <= 970 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050012, 5, -1, 8, 420)
	elseif k > 970 and k <= 990 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050017, 5, -1, 8, 420)
	elseif k > 990 and k <= 995 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236, 1, -1, 8, 420)
	elseif k > 995 and k <= 1000 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237, 1, -1, 8, 420)
	end
end



function I4040027_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end



aux.RegisterItemEvent(4040027, 1, "I4040027_GiftBag")
aux.RegisterItemEvent(4040027, 0, "I4040027_CanUse")
