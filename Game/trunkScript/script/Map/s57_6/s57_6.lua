--玩家在活动中掉线后上线
function s57_6_CanTakeOverWhenOnline(MapID, RoleID)
	return 52649206, 211, 20, 476
end
aux.RegisterMapEvent("s57_6",  12, "s57_6_CanTakeOverWhenOnline")

--找到地图的出口和坐标
function s57_6_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512, 610+math.random(5), 23094, 669+math.random(5)
end
aux.RegisterMapEvent("s57_6",  13, "s57_6_GetExporMapAndCoord")

--找到最合适的副本实例
function s57_6GetOnePerfectMap(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s57_6",  14, "s57_6GetOnePerfectMap")

--检测玩家是否能够进入活动场景
function s57_6_CanEnter(MapID, InstanceID, RoleID)
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 80 then
		return 1
	else
		return 0
	end
end
aux.RegisterMapEvent("s57_6",  15, "s57_6_CanEnter")

--触发器
function s57_6_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 743 then --to s57_5
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52649974, 390, 1, 274)
	elseif ObjID == 746 then --to s57_7
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52649462, 621, 0, 135)
	end
end
aux.RegisterMapEvent("s57_6",6,"s57_6_OnEnterTrigger")
