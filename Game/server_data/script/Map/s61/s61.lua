function s61_OnPlayerEnter(MapID, InstanceID, RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if act.GetActIsStart(53) and level >= 40 then
		local sex = role.GetRoleSex(RoleID)
		if sex == 1 then
			unit.AddBuff(MapID, InstanceID, RoleID,6940102,RoleID) --40级以上玩家每次上线获得当爱在靠近buff
		elseif sex == 0 then
			unit.AddBuff(MapID, InstanceID, RoleID,6940202,RoleID) --40级以上玩家每次上线获得当爱在靠近buff
		end
	end
end
--注册
aux.RegisterMapEvent("s61", 2, "s61_OnPlayerEnter")



function s61_OnPlayerLeave(MapID, InstanceID, RoleID)
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID,69401) then
		unit.CancelBuff(MapID,InstanceID,TargetID,6940102)
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID,69402) then
		unit.CancelBuff(MapID,InstanceID,TargetID,6940202)
	end
end
--注册
aux.RegisterMapEvent("s61", 3, "s61_OnPlayerLeave")


--找到最合适的副本实例
function s61_GetOnePerfectMap(MapID, InstanceID, RoleID)

	return 1

end
aux.RegisterMapEvent("s61",  14, "s61_GetOnePerfectMap")

--玩家在活动中掉线后上线
function s61_CanEnterWhenOnline(MapID, RoleID)
	--修正玩家的上线地图和坐标todo
	return 3424073512,610,23094,669
end

aux.RegisterMapEvent("s61",  12, "s61_CanEnterWhenOnline")

-------------------------------------------------------------------------------------------------------
--找到地图的出口和坐标
function s61_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512,610,23094,669
end

aux.RegisterMapEvent("s61",  13, "s61_GetExporMapAndCoord")

--检测玩家是否能够进入活动场景
function s61_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("s61",  15, "s61_CanEnter")
