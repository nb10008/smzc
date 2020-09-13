

--龙年传令官
function c5610100_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	if (TalkIndex == -1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444701)				--金秋十月，举国同庆，我可是给三界带来了很多祝福，赶紧选择你喜欢的吧，晚了可就没有了。（50级以下的小朋友就别凑热闹了，先去升级吧）
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)				--
		msg.AddMsgEvent(MsgID, 1, 444702)			--“每日报到”
		msg.AddMsgEvent(MsgID, 21, 5)				--
		msg.AddMsgEvent(MsgID, 1, 444703)			--“领取奖励”
		msg.AddMsgEvent(MsgID, 21, 6)				--
		msg.AddMsgEvent(MsgID, 1, 444704)			--“兑换道具”
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
			msg.AddMsgEvent(MsgID, 20, 444705)				--今天你已经完成报到了，如果你连续报到三天，就可以领取奖励了
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 10)				--查看报到情况
			msg.AddMsgEvent(MsgID, 1, 444706)
			msg.AddMsgEvent(MsgID, 21, -1)				--
			msg.AddMsgEvent(MsgID, 1, 100001)			--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			local lastDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["clgbaodao"])
			local curDate = tonumber(os.date("%j"))
			local lastDays = role.GetRoleScriptData(RoleID, 1, RoleDataType["baodaotianshu"])
			local totalDays = role.GetRoleScriptData(RoleID, 1, RoleDataType["jileijingyan"])
			if(curDate == lastDate) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444243)				--今天你不是报到过一次了吗？看你年纪轻轻的，脑袋怎么不好使？
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 10)				--查看报到情况
				msg.AddMsgEvent(MsgID, 1, 444706)
				msg.AddMsgEvent(MsgID, 21, -1)				--
				msg.AddMsgEvent(MsgID, 1, 100001)			--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if curDate - lastDate > 1 then
					lastDays = 1
				elseif curDate - lastDate == 1 or curDate == 0 then
					lastDays = lastDays + 1
				end
				role.SetRoleScriptData(RoleID, 1, RoleDataType["clgbaodao"], curDate)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["baodaotianshu"],lastDays)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["jileijingyan"],totalDays+1)
			end
		end
	elseif TalkIndex == 5 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local lastDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["clgbaodao"])
		local curDate = tonumber(os.date("%j"))
		local totalDays = role.GetRoleScriptData(RoleID, 1, RoleDataType["jileijingyan"])
		local lastdays = role.GetRoleScriptData(RoleID, 1, RoleDataType["baodaotianshu"])
		if level < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444242)  			--等你到了50级，才能参加这个活动，赶紧去升级吧。
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif lastdays < 3 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444707)       --连续报到不足三天，无法领取经验！
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--“确定”
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif totalDays <= 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444708)       --你没有可领取的经验，一边凉快去！
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--“确定”
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32,false
		else
			role.AddRoleExp(MapID, InstanceID, RoleID, totalDays*every_Exp[level])
			role.SetRoleScriptData(RoleID, 1, RoleDataType["clgjingyan"],curDate)
			role.AddRoleItem(MapID, InstanceID, RoleID,4830023, totalDays*3, -1, 8, 35)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["jileijingyan"],0)
		end
	elseif TalkIndex == 6 then
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
			msg.AddMsgEvent(MsgID, 20, 444709)  			--都是好东西，来选一个吧！
			msg.AddMsgEvent(MsgID, 21, 7)
			msg.AddMsgEvent(MsgID, 1, 444710)		--“兑换+13附魂石[绑定]”
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 444711)		--“兑换+15附魂石[绑定]”
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif(TalkIndex == 7) then
		local bangding = role.GetRoleItemNum(RoleID, 3020116)
		local bubangding = role.GetRoleItemNum(RoleID, 3020027)
		local duihuansilver = role.GetRoleSilver(MapID, InstanceID,RoleID)
		if(bangding + bubangding < 5) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444712)       --材料不足，无法兑换
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--“确定”
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif duihuansilver < 1500000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444713)       --金钱不足，无法兑换
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--“确定”
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32,false
		else
			if bangding >= 5 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020116, 5, 101)
			elseif bangding >= 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020116, bangding, 101)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020027, 5-bangding, 101)
			end
			role.DecRoleSilver(MapID, InstanceID, RoleID, 1500000, 101)
 			role.AddRoleItem(MapID, InstanceID, RoleID,3020119, 1, -1, 8, 35)
		end
	elseif(TalkIndex == 8) then
		local bangding = role.GetRoleItemNum(RoleID, 3020119)
		local bubangding = role.GetRoleItemNum(RoleID, 3020030)
		local duihuansilver = role.GetRoleSilver(MapID, InstanceID,RoleID)
		if(bangding + bubangding < 5) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444712)       --材料不足，无法兑换
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--“确定”
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif duihuansilver < 8000000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444713)       --金钱不足，无法兑换
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100001)		--“确定”
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32,false
		else
			if bangding >= 5 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020119, 5, 101)
			elseif bangding >= 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020119, bangding, 101)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3020030, 5-bangding, 101)
			end
			role.DecRoleSilver(MapID, InstanceID, RoleID, 8000000, 101)
 			role.AddRoleItem(MapID, InstanceID, RoleID,3020121, 1, -1, 8, 35)
		end
	elseif(TalkIndex == 10) then
		local lastdays = role.GetRoleScriptData(RoleID, 1, RoleDataType["baodaotianshu"])
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444714)  			--连续报到天数为：天
		msg.AddMsgEvent(MsgID, 9, lastdays)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 100001)		--“取消”
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5610100, 7, "c5610100_On_Talk")
