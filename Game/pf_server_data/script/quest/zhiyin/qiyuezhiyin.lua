--任务接取时函数
function Q40043_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	--创建活动副本
	map.CreateInstance(1826991655, RoleID)
	role.RoleGotoNewMap(MapID, InstanceID, RoleID, 1826991655, 6236/50, 1, 6970/50)
end

--注册
aux.RegisterQuestEvent(40043, 0, "Q40043_OnAccept")

function q05_PlayerEnter(MapID, InstanceID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 6010061, 6372/50, 32, 8191/50, 1)
end

--注册进入活动地图事件
aux.RegisterMapEvent("q05", 2, "q05_PlayerEnter")

--找到最合适的副本实例
function q05_GetOnePerfectMap(MapID, InstanceID, RoleID)

	return RoleID

end

aux.RegisterMapEvent("q05",  14, "q05_GetOnePerfectMap")

function N5600001_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	400002)		--“妖精妮可：\n    想要和我们缔结契约就要按照我们的规矩办！”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	400003)			--“如何捕捉妖精”
		msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	400004)			--“离开妖精之家”
		msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	400005)			--“结束对话”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	400006)		--“妖精妮可：\n    左键点击妖精灵魄即可开始与妖精缔结契约。缔结契约时，你需要在界面下方的5个道具中选择与上面的提供的道具相吻合的一个，如果选择正确，同时背包中具有该道具，则你与妖精的契约值将会增加。当契约值达到界面上规定的上限之后，你就能得到灵魄中的妖精了。”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100001)			--“确定”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		--删除活动副本
		map.DeleteInstance(1826991655, InstanceID)
	end
end

aux.RegisterCreatureEvent(5600001, 7, "N5600001_OnTalk")

--用30秒怪物控制时间，当计时器使用
function c6010061_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    --删除活动副本
	map.DeleteInstance(1826991655, InstanceID)
end

aux.RegisterCreatureEvent(6010061, 13, "c6010061_OnDisappear")

--玩家在活动中掉线后上线
function q05_CanTakeOverWhenOnline(MapID, RoleID)
	--修正玩家的上线地图和坐标todo
	return 3424071976, 762,  14174, 969
end

aux.RegisterMapEvent("q05",  12, "q05_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--找到地图的出口和坐标
function q05_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424071976, 762,  14174, 969
end

aux.RegisterMapEvent("q05",  13, "q05_GetExporMapAndCoord")

--检测玩家是否能够进入活动场景
function q05_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("q05",  15, "q05_CanEnter")
