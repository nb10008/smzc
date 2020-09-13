function huancaiwuqi_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local huancaiwuqi = act.GetActScriptData(55, 1, 10)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206499)		--幻彩武器兑换商:\n    兑换唯一一把幻彩武器包
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100001)		--确定
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if huancaiwuqi == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206496)		--幻彩武器兑换商:\n    已经有人兑换过此物品
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local num = role.GetRoleItemNum(RoleID, 4820262)
			if num < 60000 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206498)		--幻彩武器兑换商:\n    已经有人兑换过此物品
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if role.GetBagFreeSize(RoleID) < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206497) -- "幻彩武器兑换商:\n    你的背包空间不足1
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, 7)   --
					msg.AddMsgEvent(MsgID, 1, 100001)   --"确定"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 9999, 101)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820262, 6, 101)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4830071, 1, -1, 8, 420)
					act.SetActScriptData(55, 1, 10, 1)
					act.SaveActScriptData(55)
				end
			end
		end
	end
end
aux.RegisterCreatureEvent(6040052, 7, "huancaiwuqi_OnTalk")

