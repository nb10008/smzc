-- 活动第一位记录当前服务器开启漂流瓶的总数
local luomandaochuansongren = 0
local qingrenjieduihuanquan = 0
function qingrenjie2012_OnStart(actID)
	luomandaochuansongren = map.MapCreateCreature(3424073512, -1, 5610102, 506,23510,758)
	qingrenjieduihuanquan = map.MapCreateCreature(3424073512, -1, 5600037, 505,23510,767)
	map.CreateInstance(3390518312, 1)
end
aux.RegisterActEvent(53, 2, "qingrenjie2012_OnStart")

--活动结束
function qingrenjie2012_OnEnd(actID)
	if luomandaochuansongren ~= 0 and luomandaochuansongren ~= nil then
		map.MapDeleteCreature(3424073512, -1, luomandaochuansongren)
		map.MapDeleteCreature(3424073512, -1, qingrenjieduihuanquan)
		luomandaochuansongren = nil
	end
	map.DeleteInstance(3390518312, 1)
end
aux.RegisterActEvent(53, 3, "qingrenjie2012_OnEnd")

function c5610102_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 210002)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)			--
		msg.AddMsgEvent(MsgID, 1, 210003)			--“确定”
		msg.AddMsgEvent(MsgID, 21, 10)			--
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level >= 40 then
			if role.GetRoleSilver(MapID, InstanceID, RoleID) >= 1000 then
				role.DecRoleSilver(MapID, InstanceID, RoleID,1000, 420)
				if math.random(2) == 1 then
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3390518312,454,2836,256)
				else
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3390518312,444,2661,1023)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1004)
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 10)			--
				msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 210004)
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 10)			--
			msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5610102, 7, "c5610102_On_Talk")
