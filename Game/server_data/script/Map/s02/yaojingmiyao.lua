--ID 3060006  妖精秘药，兑换成：ID 3070023 妖精秘药
function miyaoduihuanshang_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 400007)		--妖精秘药兑换人：你需要兑换新的妖精秘药么？
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100001)		--确定
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 400008)		--妖精秘药兑换人：请选择兑换方式
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 6)			--兑换10个
		msg.AddMsgEvent(MsgID, 1, 400009)		--
		msg.AddMsgEvent(MsgID, 21, 7)			--兑换5个
		msg.AddMsgEvent(MsgID, 1, 400010)		--
		msg.AddMsgEvent(MsgID, 21, 8)			--兑换1个
		msg.AddMsgEvent(MsgID, 1, 400011)		--
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		if role.GetRoleItemNum(RoleID, 3060006) < 10 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 400012)		--妖精秘药兑换人：不足10个，无法10个一组进行兑换
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 3060006, 10, 101)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3070023, 10, -1, 8, 420)
		end
	elseif TalkIndex == 7 then
		if role.GetRoleItemNum(RoleID, 3060006) < 5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 400013)		--妖精秘药兑换人：不足5个，无法5个一组进行兑换
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 3060006, 5, 101)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3070023, 5, -1, 8, 420)
		end
	elseif TalkIndex == 8 then
		if role.GetRoleItemNum(RoleID, 3060006) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 400014)		--妖精秘药兑换人：不足1个，无法1个一组进行兑换
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 3060006, 1, 101)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3070023, 1, -1, 8, 420)
		end
	end
end
aux.RegisterCreatureEvent(5610115, 7, "miyaoduihuanshang_OnTalk")

