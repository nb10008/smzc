--找到最合适的副本实例
function Arena_GetOnePerfectMap(MapID, InstanceID, RoleID)

	return GetNowArenaInstanceID(RoleID)

end
aux.RegisterMapEvent("a01",  14, "Arena_GetOnePerfectMap")
aux.RegisterMapEvent("a02",  14, "Arena_GetOnePerfectMap")
aux.RegisterMapEvent("a03",  14, "Arena_GetOnePerfectMap")

--玩家在活动中掉线后上线
function Arena_CanTakeOverWhenOnline(MapID, RoleID)
	--修正玩家的上线地图和坐标todo
	local k = math.random(0,2)
	return 3424073512,ArenaMap[k].ExitPoint.x, ArenaMap[k].ExitPoint.y*51,ArenaMap[k].ExitPoint.z
end

aux.RegisterMapEvent("a01",  12, "Arena_CanTakeOverWhenOnline")
aux.RegisterMapEvent("a02",  12, "Arena_CanTakeOverWhenOnline")
aux.RegisterMapEvent("a03",  12, "Arena_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--找到地图的出口和坐标
function Arena_GetExporMapAndCoord(MapID, InstanceID, RoleID)
    local k = math.random(0,2)
	return 1, 3424073512, ArenaMap[k].ExitPoint.x, ArenaMap[k].ExitPoint.y*51,ArenaMap[k].ExitPoint.z
end

aux.RegisterMapEvent("a01",  13, "Arena_GetExporMapAndCoord")
aux.RegisterMapEvent("a02",  13, "Arena_GetExporMapAndCoord")
aux.RegisterMapEvent("a03",  13, "Arena_GetExporMapAndCoord")

-------------------------------------------------------------------------------------------------------
--检测玩家是否能够进入活动场景
function Arena_CanEnter(MapID, InstanceID, RoleID)

	if ArenaPlayer[RoleID] ~= nil and GetNowArenaInstanceID(RoleID) ~= 0 then
		return 1
	else
	    return 0
	end

end

aux.RegisterMapEvent("a01",  15, "Arena_CanEnter")
aux.RegisterMapEvent("a02",  15, "Arena_CanEnter")
aux.RegisterMapEvent("a03",  15, "Arena_CanEnter")

-------------------------------------------------------------------------------------------------------
--[[活动中无法邀请其它玩家组队
function Arena_CanInviteJoinTeam(MapID, InstanceID)
    return 9
end

aux.RegisterMapEvent("a01", 8, "Arena_CanInviteJoinTeam")
aux.RegisterMapEvent("a02", 8, "Arena_CanInviteJoinTeam")
aux.RegisterMapEvent("a03", 8, "Arena_CanInviteJoinTeam")]]

-------------------------------------------------------------------------------------------------------
--[[活动中玩家无法离开小队
function Arena_CanLeaveTeam(MapID, InstanceID)
    return 10
end

aux.RegisterMapEvent("a01", 9, "Arena_CanLeaveTeam")
aux.RegisterMapEvent("a02", 9, "Arena_CanLeaveTeam")
aux.RegisterMapEvent("a03", 9, "Arena_CanLeaveTeam")]]

-------------------------------------------------------------------------------------------------------
--[[活动中无法移交队长
function Arena_CanChangeLeader(MapID, InstanceID)
    return 11
end

aux.RegisterMapEvent("a01", 10, "Arena_CanChangeLeader")
aux.RegisterMapEvent("a02", 10, "Arena_CanChangeLeader")
aux.RegisterMapEvent("a03", 10, "Arena_CanChangeLeader")]]

-------------------------------------------------------------------------------------------------------
--[[活动中小队无法踢人
function Arena_CanKickMember(MapID, InstanceID)
    return 12
end
aux.RegisterMapEvent("a01", 17, "Arena_CanKickMember")
aux.RegisterMapEvent("a02", 17, "Arena_CanKickMember")
aux.RegisterMapEvent("a03", 17, "Arena_CanKickMember")]]
