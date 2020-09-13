function I4700222_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,1, -1, 8, 420)
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if level >= 60 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500101,5, -1, 8, 420)
	end
	if(chudonglibao == 1) then
		if(level >= 40 and level <= 85) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820028,10, -1, 8, 420)
		end
	end
end

function I4700222_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	local temp =  true
	if(chudonglibao == 1 and FreeSize < 6) then
		temp = false
	end
	if(chudonglibao ~= 1 and FreeSize < 5) then
		temp = false
	end
	if(temp == false) then
		--提示玩家背包空间不足
		bRet = 40
	end
	
	return true 
end

aux.RegisterItemEvent(4700222, 1, "I4700222_GiftBag")
aux.RegisterItemEvent(4700222, 0, "I4700222_CanUseGiftBag")