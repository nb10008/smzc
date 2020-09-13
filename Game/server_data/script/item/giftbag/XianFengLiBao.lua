function XianFeng_GiftBag(MapID, InstanceID, TypeID, TargetID)

	if TypeID == 4510001 then
		--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070027, 3, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4500002, 3, -1, 8, 420)
	    local k = math.random(30)
		if k == 15 then
		    role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 1, -1, 8, 420)
		end
	elseif TypeID == 4510002 then
		--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070027, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4500002, 5, -1, 8, 420)
	    local k = math.random(15)
		if k == 15 then
		    role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 1, -1, 8, 420)
		end
	end
end

function XianFeng_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4510001, 1, "XianFeng_GiftBag")
aux.RegisterItemEvent(4510001, 0, "XianFeng_CanUseGiftBag")
aux.RegisterItemEvent(4510002, 1, "XianFeng_GiftBag")
aux.RegisterItemEvent(4510002, 0, "XianFeng_CanUseGiftBag")


