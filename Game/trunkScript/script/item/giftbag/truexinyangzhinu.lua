--白银信仰之怒


function I4700036_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(1,10000)
			if a >= 1 and a <= 615 		then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530101, 1, -1, 8, 420)
				elseif a > 615 and a<= 1215 		then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530104, 1, -1, 8, 420)
				elseif a >	1215	and	a<=	2425	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080097, 1, -1, 8, 420)
				elseif	a >	2425	and	a<=	3425	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530108, 5, -1, 8, 420)
				elseif	a >	3425	and	a<=	3738	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090020, 1, -1, 8, 420)
				elseif	a >	3738	and	a<=	3773	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030119, 1, -1, 8, 420)
				elseif	a > 3773	and	a<=	4273	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600005, 1, -1, 8, 420)
				elseif	a > 4273	and	a<=	4278	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 5610013, 1, -1, 8, 420)
				elseif	a > 4278	and	a<=	5298	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070019, 3, -1, 8, 420)
				elseif	a >	5298	and	a<=	5668	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600008, 1, -1, 8, 420)
				elseif	a >	5668	and	a<=	7218	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090010, 5, -1, 8, 420)
				elseif	a > 7218 	and	a<=	8418	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500002, 7, -1, 8, 420)
				elseif	a > 8418	and	a<=	8419	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020039, 1, -1, 8, 420)
				elseif	a > 8419	and	a<=	8420	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020044, 1, -1, 8, 420)
				elseif	a > 8420	and	a<=	10000	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090011, 5, -1, 8, 420)
			end
	end

function I4700036_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700036, 1, "I4700036_GiftBag")
aux.RegisterItemEvent(4700036, 0, "I4700036_CanUseGiftBag")




--聚宝盆




function I4700037_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(1,1000)
			if a >= 1 and a <= 90 		then
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600008, 1, -1, 8, 420)
				elseif a > 90 and a<= 220 		then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070023, 1, -1, 8, 420)
				elseif a >	220	and	a<=	320	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070019, 2, -1, 8, 420)
				elseif	a >	320	and	a<=	390	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020052, 1, -1, 8, 420)
				elseif	a >	390	and	a<=	550	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500002, 6, -1, 8, 420)
				elseif	a >	550	and	a<=	585	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4999003, 1, -1, 8, 420)
				elseif	a > 585	and	a<=	735	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070052, 8, -1, 8, 420)
				elseif	a > 735	and	a<=	736	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 5610014, 1, -1, 8, 420)
				elseif	a > 736	and	a<=	810	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400024, 1, -1, 8, 420)
				elseif	a >	810	and	a<=	1000	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020007, 1, -1, 8, 420)
			end
	end

function I4700037_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700037, 1, "I4700037_GiftBag")
aux.RegisterItemEvent(4700037, 0, "I4700037_CanUseGiftBag")












