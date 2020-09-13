function s57_2_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 1732 then --to s57_1
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648950, 315, 200, 294)
	elseif ObjID == 1739 then --to 1
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 386, 200, 72)
	elseif ObjID == 1749 then --to h
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 484, 200, 64)
	elseif ObjID == 1741 then --to 2
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 90, 200, 62)
	elseif ObjID == 1752 then --to h
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 424, 200, 178)
	elseif ObjID == 1744 then --to 3
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 306, 200, 437)
	elseif ObjID == 1755 then --to h
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 427, 200, 276)
	elseif ObjID == 1746 then --to 4
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 194, 200, 292)
	elseif ObjID == 1758 then --to h
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 446, 200, 367)
	elseif ObjID == 1760 then
		local randomnum = math.random(4)
		if randomnum == 1 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648438, 37, 199, 49)
		elseif randomnum == 2 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648438, 32, 199, 470)
		elseif randomnum == 3 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648438, 478, 199, 466)
		elseif randomnum == 4 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648438, 462, 199, 41)
		end
	end
end
aux.RegisterMapEvent("s57_2",6,"s57_2_OnEnterTrigger")

--玩家在活动中掉线后上线
function s57_2_CanTakeOverWhenOnline(MapID, RoleID)
	return 52648182, 17, 10000, 477
end
aux.RegisterMapEvent("s57_2",  12, "s57_2_CanTakeOverWhenOnline")

--找到最合适的副本实例
function s57_2GetOnePerfectMap(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s57_2",  14, "s57_2GetOnePerfectMap")

--检测玩家是否能够进入活动场景
function s57_2_CanEnter(MapID, InstanceID, RoleID)
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 80 then
		return 1
	else
		return 0
	end
end
aux.RegisterMapEvent("s57_2",  15, "s57_2_CanEnter")

--找到地图的出口和坐标
function s57_2_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512, 610+math.random(5), 23094, 669+math.random(5)
end
aux.RegisterMapEvent("s57_2",  13, "s57_2_GetExporMapAndCoord")
