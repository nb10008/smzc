--与食材收购商对话
function mofabu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local num1 = role.GetRoleItemNum(RoleID, 4100201)
	local num2 = role.GetRoleItemNum(RoleID, 4100202)
	local num3 = role.GetRoleItemNum(RoleID, 4100203)
	local num4 = role.GetRoleItemNum(RoleID, 4100204)
	local num5 = role.GetRoleItemNum(RoleID, 4100205)
	local num6 = role.GetRoleItemNum(RoleID, 4100206)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420052)	--介绍兑换魔法布的背景
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 11)		--是否需要兑换魔法布
		msg.AddMsgEvent(MsgID, 1,  420053)	--“我要兑换魔法布”
		msg.AddMsgEvent(MsgID, 21, 10)		--取消返回10
		msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 11 then		--兑换魔法布
		local MsgID = msg.BeginMsgEvent()
		if num1 < 12 and num2 < 12 and num3 < 12 and num4 < 12 and num5 < 12 and num6 < 12 then
			msg.AddMsgEvent(MsgID, 20, 420061)	--“您需要凑齐12个任意果实才可兑换”
		    msg.AddMsgEvent(MsgID, 21, 10)		--用香果兑换
			msg.AddMsgEvent(MsgID, 1,  420009)	--
		else
			msg.AddMsgEvent(MsgID, 20, 420054)	--请选择你用什么兑换
		end
		msg.AddMsgEvent(MsgID, 24, TargetID)
		if num1 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 12)		--用香果兑换
			msg.AddMsgEvent(MsgID, 1,  420055)	--
		end
		if num2 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 13)		--用火桃兑换
			msg.AddMsgEvent(MsgID, 1,  420056)	--
		end
		if num3 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 14)		--用粒粒玉兑换
			msg.AddMsgEvent(MsgID, 1,  420057)	--
		end
		if num4 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 15)		--用水晶眼兑换
			msg.AddMsgEvent(MsgID, 1,  420058)	--
		end
		if num5 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 16)		--用紫梅兑换
			msg.AddMsgEvent(MsgID, 1,  420059)	--
		end
		if num6 >= 12 then
			msg.AddMsgEvent(MsgID, 21, 17)		--用椰米兑换
			msg.AddMsgEvent(MsgID, 1,  420060)	--
		end
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 12 then		--兑换魔法布
		local num = role.GetRoleItemNum(RoleID, 4100201)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100201, 12, 420)	--删除12个香果
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---“对话后提示：“您没有足够的道具”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 13 then		--兑换魔法布
		local num = role.GetRoleItemNum(RoleID, 4100202)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100202, 12, 420)	--删除12个火桃
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---“对话后提示：“您没有足够的道具”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 14 then		--兑换魔法布
		local num = role.GetRoleItemNum(RoleID, 4100203)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100203, 12, 420)	--删除12个粒粒玉
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---“对话后提示：“您没有足够的道具”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 15 then		--兑换魔法布
		local num = role.GetRoleItemNum(RoleID, 4100204)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100204, 12, 420)	--删除12个水晶眼
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---“对话后提示：“您没有足够的道具”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 16 then		--兑换魔法布
		local num = role.GetRoleItemNum(RoleID, 4100205)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100205, 12, 420)	--删除12个紫梅
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---“对话后提示：“您没有足够的道具”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 17 then		--兑换魔法布
		local num = role.GetRoleItemNum(RoleID, 4100206)
		local FreeSize = role.GetBagFreeSize(RoleID)
		if num >= 12 and FreeSize >= 1 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100206, 12, 420)	--删除12个椰米
			role.AddRoleItem(MapID, InstanceID, RoleID, 3050109, 1, -1, 8, 420)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420062)	---“对话后提示：“您没有足够的道具”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 10)		--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5516003, 7, "mofabu_OnTalk")
