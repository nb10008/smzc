function c6011055_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 210034)	--让我们一起来见证这对新人永恒的爱情吧！\n(当拥有状态<color=0xffffff00>见证永恒<color=0xffffffff>时，才可以领取奖励<color=0xff00a2ff>新婚礼包<color=0xffffffff>。)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)
		msg.AddMsgEvent(MsgID, 1, 100001)--“确定”
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 100002)--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if role.IsRoleHaveBuff(MapID, InstanceID, RoleID,69409) == true then
			local number = role.GetBagFreeSize(RoleID)
				if number >= 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700276, 1, -1, 8, 420)
					unit.CancelBuff(MapID,InstanceID,RoleID, 6940901)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 210019)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 210035)	--只有处于[见证永恒]状态时，才能领取新婚礼包！
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(6011055, 7, "c6011055_On_Talk")
