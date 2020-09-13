--玩家在活动中掉线后上线
function s57_1_CanTakeOverWhenOnline(MapID, RoleID)
	return 52648950, 23, 9991, 24
end
aux.RegisterMapEvent("s57_1",  12, "s57_1_CanTakeOverWhenOnline")

--找到最合适的副本实例
function s57_1GetOnePerfectMap(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s57_1",  14, "s57_1GetOnePerfectMap")

--检测玩家是否能够进入活动场景
function s57_1_CanEnter(MapID, InstanceID, RoleID)
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 80 then
		return 1
	else
		return 0
	end
end
aux.RegisterMapEvent("s57_1",  15, "s57_1_CanEnter")

--找到地图的出口和坐标
function s57_1_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512, 610+math.random(5), 23094, 669+math.random(5)
end
aux.RegisterMapEvent("s57_1",  13, "s57_1_GetExporMapAndCoord")

--触发器
function s57_1_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 1483 then --to s60
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3390518568, 568, 200, 204)
	elseif ObjID == 1491 then --to s57_2
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 18, 200, 478)
	elseif ObjID == 1485 then --to 1
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 489, 200, 77)
	elseif ObjID == 1500 then --to h
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 54, 200, 463)
	elseif ObjID == 1487 then --to 2
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 465, 200, 216)
	elseif ObjID == 1501 then --to h
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 199, 200, 408)
	elseif ObjID == 1489 then --to 3
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 486, 200, 352)
	elseif ObjID == 1503 then --to h
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 390, 200, 430)
	elseif ObjID == 1493 then --to 4
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 489, 200, 448)
	elseif ObjID == 1499 then --to h
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 141, 200, 272)
	end
end
aux.RegisterMapEvent("s57_1",6,"s57_1_OnEnterTrigger")
