--下层
function c5610070_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444300)				--传送至下一层
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)
		msg.AddMsgEvent(MsgID, 1, 100001)					--“确定
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 100002)				--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52649718, 180, 5, 40)
	end
end
aux.RegisterCreatureEvent(5610070, 7, "c5610070_On_Talk")

--玩家在活动中掉线后上线
function s57_3_CanTakeOverWhenOnline(MapID, RoleID)
	local randomnum = math.random(4)
	if randomnum == 1 then
		return 52648438, 37, 9991, 49
	elseif randomnum == 2 then
		return 52648438, 32, 9991, 470
	elseif randomnum == 3 then
		return 52648438, 478, 9991, 466
	elseif randomnum == 4 then
		return 52648438, 462, 9991, 41
	end
end
aux.RegisterMapEvent("s57_3",  12, "s57_3_CanTakeOverWhenOnline")

--找到最合适的副本实例
function s57_3GetOnePerfectMap(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s57_3",  14, "s57_3GetOnePerfectMap")

--检测玩家是否能够进入活动场景
function s57_3_CanEnter(MapID, InstanceID, RoleID)
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 80 then
		return 1
	else
		return 0
	end
end
aux.RegisterMapEvent("s57_3",  15, "s57_3_CanEnter")

--找到地图的出口和坐标
function s57_3_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512, 610+math.random(5), 23094, 669+math.random(5)
end
aux.RegisterMapEvent("s57_3",  13, "s57_3_GetExporMapAndCoord")

--触发器
function s57_3_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 1165 then --to s57_2
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 248, 200, 252)
	elseif ObjID == 1168 then --to s57_2
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 248, 200, 252)
	elseif ObjID == 1171 then --to s57_2
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 248, 200, 252)
	elseif ObjID == 1174 then --to s57_2
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52648182, 248, 200, 252)
	end
end
aux.RegisterMapEvent("s57_3",6,"s57_3_OnEnterTrigger")
