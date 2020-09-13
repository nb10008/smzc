function huancaiwuqi_GiftBag(MapID, InstanceID, TypeID, TargetID)
	
	local class = role.GetRoleClass(TargetID)
	if class == 2 or class == 6 or class == 7 then--长兵
		role.AddRoleItem(MapID, InstanceID, TargetID,8019006, 1, -1, 8, 420)
	elseif class == 3 or class == 8 or class == 9 then--弓
		role.AddRoleItem(MapID, InstanceID, TargetID, 8029006, 1, -1, 8, 420)
	elseif class == 4 or class == 10 or class == 11 then--琴
		role.AddRoleItem(MapID, InstanceID, TargetID, 8049006, 1, -1, 8, 420)
	elseif class == 5 or class == 12 or class == 13 then--法器
		role.AddRoleItem(MapID, InstanceID, TargetID, 8039006, 1, -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830071, 1, "huancaiwuqi_GiftBag")
function huancaiwuqi_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if class == 1 then
		bRet = 32
	else
		if(FreeSize < 1) then
			--提示玩家背包空间不足
			bRet = 40
		end
		
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830071, 0, "huancaiwuqi_CanUseGiftBag")