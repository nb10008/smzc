--上层
function c5610071_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444301)				--传送至上一层
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)
		msg.AddMsgEvent(MsgID, 1, 100001)					--“确定
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 100002)				--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648438, 269, 208, 257)
	end
end
aux.RegisterCreatureEvent(5610071, 7, "c5610071_On_Talk")

--玩家在活动中掉线后上线
function s57_4_CanTakeOverWhenOnline(MapID, RoleID)
	return 52648438, 250+math.random(5), 10400, 257+math.random(5)
end
aux.RegisterMapEvent("s57_4",  12, "s57_4_CanTakeOverWhenOnline")

--找到最合适的副本实例
function s57_4GetOnePerfectMap(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s57_4",  14, "s57_4GetOnePerfectMap")

--检测玩家是否能够进入活动场景
function s57_4_CanEnter(MapID, InstanceID, RoleID)
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 80 then
		return 1
	else
		return 0
	end
end
aux.RegisterMapEvent("s57_4",  15, "s57_4_CanEnter")

--找到地图的出口和坐标
function s57_4_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512, 610+math.random(5), 23094, 669+math.random(5)
end
aux.RegisterMapEvent("s57_4",  13, "s57_4_GetExporMapAndCoord")
