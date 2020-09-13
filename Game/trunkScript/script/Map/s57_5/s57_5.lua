--玩家在活动中掉线后上线
function s57_5_CanTakeOverWhenOnline(MapID, RoleID)
	return 52649974, 440, 1, 768
end
aux.RegisterMapEvent("s57_5",  12, "s57_5_CanTakeOverWhenOnline")

--找到地图的出口和坐标
function s57_5_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512, 610+math.random(5), 23094, 669+math.random(5)
end
aux.RegisterMapEvent("s57_5",  13, "s57_5_GetExporMapAndCoord")

--找到最合适的副本实例
function s57_5_1GetOnePerfectMap(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s57_5",  14, "s57_5_1GetOnePerfectMap")

--检测玩家是否能够进入活动场景
function s57_5_CanEnter(MapID, InstanceID, RoleID)
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 80 then
		return 1
	else
		return 0
	end
end
aux.RegisterMapEvent("s57_5",  15, "s57_5_CanEnter")

--触发器
function s57_5_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 673 then --to s60
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3390518568, 140, 200, 601)
	elseif ObjID == 672 then --to s57_6
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52649206, 211, 1, 476)
	end
end
aux.RegisterMapEvent("s57_5",6,"s57_5_OnEnterTrigger")
