
--20349绿色盎然，使用4010192种子种出雪之花6011018，对其采集获得4010193鲜花 天雪镇里面

function Q20349_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	
	--如果不在s05场景，则不能使用
	if MapID ~= 3424071720 then
	    bRet = 43
	else
	    --得到玩家当前坐标
		local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	
		if x < 2478 or x > 2775 or z < 2524 or z > 2914 then  --如果不在这个坐标范围，则不能使用
			bRet = 43
		else
			local FreeSize = role.GetBagFreeSize(RoleID)
			if(FreeSize < 1) then
			--提示玩家背包空间不足
				bRet = 40
			end
		end
	end
	--返回
	return bRet, bIgnore
end

aux.RegisterItemEvent(4010192, 0, "Q20349_CanUse")

function Q20349_QuestUsable(MapID, InstanceID, TypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 6011018, x+1, y, z+1)
	
end

function Q20349_OnInvest(MapID, InstanceID, QuestID, RoleID, CreatureTypeID)

	role.AddRoleItem(MapID, InstanceID, RoleID, 4010193, 1, 0, 0, 402)
end


--注册
aux.RegisterItemEvent(4010192, 1, "Q20349_QuestUsable")
aux.RegisterQuestEvent(20349, 9, "Q20349_OnInvest")
