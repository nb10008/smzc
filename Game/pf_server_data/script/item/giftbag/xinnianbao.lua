
--新年礼袋
function I4800018_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k = math.random(1,10000)
	if k >= 1 and k < 2000 then
		role.AddRoleSilver(MapID, InstanceID, TargetID, 2011, 420)
	elseif k > 2000 and k<= 2200 then
		role.AddRoleSilver(MapID, InstanceID, TargetID, 60000, 420)
	elseif k > 2200 and k<= 2250 then
		role.AddRoleSilver(MapID, InstanceID, TargetID, 660000, 420)
	elseif k > 2250 and k<= 3450 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800003, 2, -1, 8, 420)
	elseif k > 3450 and k<= 4050 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800003, 4, -1, 8, 420)
	elseif k > 4050 and k<= 4850 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090012, 1, -1, 8, 420)
	elseif k > 4850 and k<= 5050 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090007, 1, -1, 8, 420)
	elseif k > 5050 and k<= 5850 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090013, 1, -1, 8, 420)
	elseif k > 5850 and k<= 6050 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090008, 1, -1, 8, 420)
	elseif k > 6050 and k<= 6850 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090014, 1, -1, 8, 420)
	elseif k > 6850 and k<= 7050 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 1, -1, 8, 420)
	elseif k > 7050 and k<= 7250 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050012, 1, -1, 8, 420)
	elseif k > 7250 and k<= 7450 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050017, 1, -1, 8, 420)
	elseif k > 7450 and k<= 7650 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050022, 1, -1, 8, 420)
	elseif k > 7650 and k<= 7750 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050013, 1, -1, 8, 420)
	elseif k > 7750 and k<= 7850 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050018, 1, -1, 8, 420)
	elseif k > 7850 and k<= 7950 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050023, 1, -1, 8, 420)
	elseif k > 7950 and k<= 8200 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400004, 1, -1, 8, 420)
	elseif k > 8200 and k<= 8450 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400005, 1, -1, 8, 420)
	elseif k > 8450 and k<= 8700 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400007, 1, -1, 8, 420)
	elseif k > 8700 and k<= 8950 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400010, 1, -1, 8, 420)
	elseif k > 8950 and k<= 9200 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400013, 1, -1, 8, 420)
	elseif k > 9200 and k<= 9450 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400016, 1, -1, 8, 420)
	elseif k > 9450 and k<= 9700 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400019, 1, -1, 8, 420)
	elseif k > 9700 and k<= 9950 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400022, 1, -1, 8, 420)
	elseif k > 9950 and k<= 10000 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 1, -1, 8, 420)
	end


end

function I4800018_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800018, 1, "I4800018_GiftBag")
aux.RegisterItemEvent(4800018, 0, "I4800018_CanUseGiftBag")






--新年大礼包
function I4800019_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleSilver(MapID, InstanceID, TargetID, 660000, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800103, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800003, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800025, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 5, -1, 8, 420)
end

function I4800019_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800019, 1, "I4800019_GiftBag")
aux.RegisterItemEvent(4800019, 0, "I4800019_CanUseGiftBag")




