function n6040147_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650015) -- 圣灵秘药兑换商：\n    我这里可以用各种圣灵秘药兑换妖精魂核
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4) 
		msg.AddMsgEvent(MsgID, 1, 650016) -- 圣灵秘药【试用版】
		msg.AddMsgEvent(MsgID, 21, 20)
		msg.AddMsgEvent(MsgID, 1, 650033) --圣灵秘药
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 650017) -- 高效圣灵秘药
		msg.AddMsgEvent(MsgID, 21, 6)
		msg.AddMsgEvent(MsgID, 1, 650018) --强效圣灵秘药
		msg.AddMsgEvent(MsgID, 21, 7)
		msg.AddMsgEvent(MsgID, 1, 650019) --极效圣灵秘药
		msg.DispatchRoleMsgEvent(RoleID, MsgID) 
	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650028) -- 圣灵秘药兑换商：\n    圣灵秘药【试用版】\n    100个：兑换<I>妖精魂核[90]</I>\n    50个：兑换<I>妖精魂核[80]</I>\n    20个：兑换<I>妖精魂核[60]</I>\n
		msg.AddMsgEvent(MsgID, 24, TargetID) 
		msg.AddMsgEvent(MsgID, 21, 8)
		msg.AddMsgEvent(MsgID, 1, 650020) -- 兑换100个
		msg.AddMsgEvent(MsgID, 21, 9)
		msg.AddMsgEvent(MsgID, 1, 650021) -- 兑换50个
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650023) --兑换20个
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650029) -- 圣灵秘药兑换商：\n    高效圣灵秘药\n    50个：兑换<I>妖精魂核[95]</I>\n    30个：兑换<I>妖精魂核[90]</I>\n    10个：兑换<I>妖精魂核[85]</I>\n
		msg.AddMsgEvent(MsgID, 24, TargetID) 
		msg.AddMsgEvent(MsgID, 21, 11)
		msg.AddMsgEvent(MsgID, 1, 650021) -- 兑换50个
		msg.AddMsgEvent(MsgID, 21, 12)
		msg.AddMsgEvent(MsgID, 1, 650022) --兑换30个
		msg.AddMsgEvent(MsgID, 21, 13)
		msg.AddMsgEvent(MsgID, 1, 650024) --兑换10个
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650030) -- 圣灵秘药兑换商：\n    强效圣灵秘药\n    20个：兑换<I>妖精魂核[95]</I>\n    10个：兑换<I>妖精魂核[85]</I>\n    5个：兑换<I>妖精魂核[80]</I>\n
		msg.AddMsgEvent(MsgID, 24, TargetID) 
		msg.AddMsgEvent(MsgID, 21, 14)
		msg.AddMsgEvent(MsgID, 1, 650023) -- 兑换20个
		msg.AddMsgEvent(MsgID, 21, 15)
		msg.AddMsgEvent(MsgID, 1, 650024) --兑换10个
		msg.AddMsgEvent(MsgID, 21, 16)
		msg.AddMsgEvent(MsgID, 1, 650025) --兑换5个
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 7 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650031) -- 圣灵秘药兑换商：\n    极效圣灵秘药\n    10个：兑换<I>妖精魂核[100]</I>\n    5个：兑换<I>妖精魂核[99]</I>\n    1个：兑换<I>妖精魂核[85]</I>\n
		msg.AddMsgEvent(MsgID, 24, TargetID) 
		msg.AddMsgEvent(MsgID, 21, 17)
		msg.AddMsgEvent(MsgID, 1, 650024) -- 兑换10个
		msg.AddMsgEvent(MsgID, 21, 18)
		msg.AddMsgEvent(MsgID, 1, 650025) --兑换5个
		msg.AddMsgEvent(MsgID, 21, 19)
		msg.AddMsgEvent(MsgID, 1, 650026) --兑换1个
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 20 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650034) -- 圣灵秘药兑换商：\n    圣灵秘药\n    100个：兑换<I>妖精魂核[90]</I>\n    50个：兑换<I>妖精魂核[80]</I>\n    20个：兑换<I>妖精魂核[60]</I>\n
		msg.AddMsgEvent(MsgID, 24, TargetID) 
		msg.AddMsgEvent(MsgID, 21, 21)
		msg.AddMsgEvent(MsgID, 1, 650020) -- 兑换100个
		msg.AddMsgEvent(MsgID, 21, 22)
		msg.AddMsgEvent(MsgID, 1, 650021) -- 兑换50个
		msg.AddMsgEvent(MsgID, 21, 23)
		msg.AddMsgEvent(MsgID, 1, 650023) --兑换20个
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 8 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500001)
			if a < 100 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500204,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500001, 100, 420)
			end
		end
	elseif TalkIndex == 9 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500001)
			if a < 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500202,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500001, 50, 420)
			end
		end
	elseif TalkIndex == 10 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500001)
			if a < 20 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500201,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500001, 20, 420)
			end
		end
	elseif TalkIndex == 11 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500003)
			if a < 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500205,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500003, 50, 420)
			end
		end
	elseif TalkIndex == 12 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500003)
			if a < 30 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500204,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500003, 30, 420)
			end
		end
	elseif TalkIndex == 13 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500003)
			if a < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500203,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500003, 10, 420)
			end
		end
	elseif TalkIndex == 14 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500004)
			if a < 20 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500205,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500004, 20, 420)
			end
		end
	elseif TalkIndex == 15 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500004)
			if a < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500203,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500004, 10, 420)
			end
		end
	elseif TalkIndex == 16 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500004)
			if a < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500202,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500004, 5, 420)
			end
		end
	elseif TalkIndex == 17 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500005)
			if a < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500207,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500005, 10, 420)
			end
				
		end
	elseif TalkIndex == 18 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500005)
			if a < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500206,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500005, 5, 420)
			end
		end
	elseif TalkIndex == 19 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500005)
			if a < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500203,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500005, 1, 420)
			end
		end
	elseif TalkIndex == 21 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500002)
			if a < 100 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500204,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500002, 100, 420)
			end
		end
	elseif TalkIndex == 22 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500002)
			if a < 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500202,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500002, 50, 420)
			end
		end
	elseif TalkIndex == 23 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 650027) -- 圣灵秘药兑换商：\n    背包空间不足
			msg.AddMsgEvent(MsgID, 24, TargetID) --
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local a = role.GetRoleItemNum(RoleID, 3500002)
			if a < 20 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 650032) -- 圣灵秘药兑换商：\n    所需材料不足
				msg.AddMsgEvent(MsgID, 24, TargetID) --
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 3500201,1, -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3500002, 20, 420)
			end
		end

	end

end
aux.RegisterCreatureEvent(6040147, 7, "n6040147_OnTalk")
