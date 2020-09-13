

--春节使者
function c5610099_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	local chunjieshizhe_start = false
	if curyear == 2012 and curmon == 1 and curday >=17 and curday <= 31  then
		chunjieshizhe_start = true
	else
		chunjieshizhe_start = false
	end
	if (chunjieshizhe_start == true and TalkIndex == -1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444601)				--金秋十月，举国同庆，我可是给三界带来了很多祝福，赶紧选择你喜欢的吧，晚了可就没有了。（50级以下的小朋友就别凑热闹了，先去升级吧）
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)				--
		msg.AddMsgEvent(MsgID, 1, 444602)			--“领取签到牌”
		msg.AddMsgEvent(MsgID, 21, 5)				--
		msg.AddMsgEvent(MsgID, 1, 444603)			--“兑换奖励”
		msg.AddMsgEvent(MsgID, 21, 30)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif (chunjieshizhe_start == false and TalkIndex == -1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444611)				--活动已经结束了，赶紧把你的经验领走吧，下周我可要回去了。
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 5)				--
		msg.AddMsgEvent(MsgID, 1, 444603)			--“兑换奖励”
		msg.AddMsgEvent(MsgID, 21, 30)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444242)  			--等你到了50级，才能参加这个活动，赶紧去升级吧。
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444604)				--今天的签到牌你已经领取了，积攒足够的签到牌可以兑换各种道具，明天别忘了来领取。
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100001)			--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			local lastDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["xinchunbaodao"])
			local curDate = tonumber(os.date("%j"))
			if(curDate == lastDate) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444605)				--今天的签到牌你已经领取过了，明天再来吧
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, -1)				--
				msg.AddMsgEvent(MsgID, 1, 100001)			--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if role.GetBagFreeSize(RoleID) < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
					msg.AddMsgEvent(MsgID, 9, 1)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					return 32,false
				else
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800261, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["xinchunbaodao"],curDate)
				end
			end
		end
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444606)				--请选择你要兑换的签到牌数量：
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 6)				--
		msg.AddMsgEvent(MsgID, 1, 444607)			--“14个春节签到牌”
		msg.AddMsgEvent(MsgID, 21, 7)				--
		msg.AddMsgEvent(MsgID, 1, 444608)			--“7个春节签到牌”
		msg.AddMsgEvent(MsgID, 21, 8)				--
		msg.AddMsgEvent(MsgID, 1, 444609)			--“3个春节签到牌”
		msg.AddMsgEvent(MsgID, 21, -1)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		local itemNum = role.GetRoleItemNum(RoleID, 4800261)
		if itemNum < 14 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444610)				--签到牌数量不足，无法兑换
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetBagFreeSize(RoleID) < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
				msg.AddMsgEvent(MsgID, 9, 5)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 32,false
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800261, 14, 101)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090011, 10, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090010, 10, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820109, 8, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820047, 10, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820046, 10, -1, 8, 420)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 30000000, 101)
			end
		end

	elseif(TalkIndex == 7) then
		local itemNum = role.GetRoleItemNum(RoleID, 4800261)
		if itemNum < 7 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444610)				--签到牌数量不足，无法兑换
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetBagFreeSize(RoleID) < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
				msg.AddMsgEvent(MsgID, 9, 5)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 32,false
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800261, 7, 101)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090011, 5, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090010, 5, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820109, 3, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820047, 5, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820046, 5, -1, 8, 420)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 8000000, 101)
			end
		end
	elseif(TalkIndex == 8) then
		local itemNum = role.GetRoleItemNum(RoleID, 4800261)
		if itemNum < 3 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444610)				--签到牌数量不足，无法兑换
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100002)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetBagFreeSize(RoleID) < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
				msg.AddMsgEvent(MsgID, 9, 5)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 32,false
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800261, 3, 101)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090011, 2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4090010, 2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820109, 1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820047, 2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820046, 2, -1, 8, 420)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 2000000, 101)
			end
		end
	end
end
aux.RegisterCreatureEvent(5610099, 7, "c5610099_On_Talk")

