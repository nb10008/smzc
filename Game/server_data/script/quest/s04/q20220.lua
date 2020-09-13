
--20220烟花燃放使用4010190烟花获得4010191信号印记

function Q20220_CanUse0(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false

	--如果不在s04场景，则不能使用
	if MapID ~= 3424071976 then
	    bRet = 43
	else
	    --得到玩家当前坐标
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)

	    if x < 406 or x > 418 or z < 933 or z > 945 then  --如果不在这个坐标范围，则不能使用
	        bRet = 43
	    else
		--背包小于1时不能使用
		local FreeSize = role.GetBagFreeSize(RoleID)
		if (FreeSize < 1) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 40
			return bRet, bIgnore
		end
	    end
	end
	--返回
	return bRet, bIgnore
end

aux.RegisterItemEvent(4010190, 0, "Q20220_CanUse0")


function Q20220_QuestUsable(MapID, InstanceID, TypeID, RoleID)

	role.AddRoleItem(MapID, InstanceID, RoleID, 4010191, 1, 0, 0, 402)
		
end

function Q20220_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
		return bRet, bIgnore
	end
end


--注册
aux.RegisterItemEvent(4010190, 1, "Q20220_QuestUsable")
aux.RegisterItemEvent(4010191, 0, "Q20220_CanUse")

