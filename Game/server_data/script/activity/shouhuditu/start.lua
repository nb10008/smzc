-- 39200 4245 6800
--开启守卫地图刷怪
function NPC6040043_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 207101)		--选择守卫地图刷怪
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			
		msg.AddMsgEvent(MsgID, 1, 207105)		--刷新
		msg.AddMsgEvent(MsgID, 21, 5)			
		msg.AddMsgEvent(MsgID, 1, 207102)		--开始
		msg.AddMsgEvent(MsgID, 21, 6)			
		msg.AddMsgEvent(MsgID, 1, 207103)		--等待
		msg.AddMsgEvent(MsgID, 21, 7)			
		msg.AddMsgEvent(MsgID, 1, 207104)		--停止
		msg.AddMsgEvent(MsgID, 21, 8)			
		msg.AddMsgEvent(MsgID, 1, 207106)		--继续
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		map.SetMonsterGenState(MapID, InstanceID, 0)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207110)      --守卫地图怪物以刷新
		msg.DispatchWorldMsgEvent(MsgID)
	elseif TalkIndex == 5 then
		map.SetMonsterGenState(MapID, InstanceID, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207107)      --守卫地图怪物以开始
		msg.DispatchWorldMsgEvent(MsgID)
	elseif TalkIndex == 6 then
		map.SetMonsterGenState(MapID, InstanceID, 2)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207108)      --守卫地图怪物以等待
		msg.DispatchWorldMsgEvent(MsgID)
	elseif TalkIndex == 7 then
		map.SetMonsterGenState(MapID, InstanceID, 3)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207109)      --守卫地图怪物以停止
		msg.DispatchWorldMsgEvent(MsgID)
	elseif TalkIndex == 8 then
		map.SetMonsterGenState(MapID, InstanceID, 4)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,207111)      --守卫地图怪物以继续
		msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterCreatureEvent(6040043, 7, "NPC6040043_OnTalk")

