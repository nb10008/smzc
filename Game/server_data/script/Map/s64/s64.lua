
function s64_OnPlayerEnter(MapID, InstanceID, RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
end
aux.RegisterMapEvent("s64", 2, "s64_OnPlayerEnter")


--找到最合适的副本实例
function s64_GetOnePerfectMap(MapID, InstanceID, RoleID)
	return curMSIns
end
aux.RegisterMapEvent("s64",  14, "s64_GetOnePerfectMap")

--玩家在活动中掉线后上线
function s64_CanEnterWhenOnline(MapID, RoleID)
	--修正玩家的上线地图和坐标todo
	return 3424073512,610,23094,669
end

aux.RegisterMapEvent("s64",  12, "s64_CanEnterWhenOnline")

-------------------------------------------------------------------------------------------------------
--找到地图的出口和坐标
function s64_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3390518312,213,2543,726
end

aux.RegisterMapEvent("s64",  13, "s64_GetExporMapAndCoord")

--检测玩家是否能够进入活动场景
function s64_CanEnter(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s64",  15, "s64_CanEnter")
