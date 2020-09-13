--RoleDataType["received"]
--5600078npc

function c5600078_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local status = role.GetAccountReactiveStatus(MapID, InstanceID, RoleID)
	if status == 1 then
		local rescore = role.GetAccountReactiveScore(MapID, InstanceID, RoleID)
		local gblevel = role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)
		local tempa = 0
		local gbtable = {}
		gbtable[9] = {200,4700196}
		gbtable[8] = {160,4700195}
		gbtable[7] = {120,4700194}
		gbtable[6] = {80,4700193}
		gbtable[5] = {60,4700192}
		gbtable[4] = {35,4700191}
		gbtable[3] = {25,4700190}
		gbtable[2] = {15,4700189}
		gbtable[1] = {5,4700188}
		gbtable[0] = {1,4700187}
		for i=0,9 do
			if rescore >= gbtable[i][1] then
				tempa = i
			end
		end
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			if gblevel <= tempa and rescore > 0 then
				msg.AddMsgEvent(MsgID, 20, 444255)					--尊敬的xxx:您好！感谢您对圣魔之血的一贯支持，经过我们的综合认定，已经将您游戏中的本角色设置为勇士介绍人。在回归活动期间，只要回归勇士在游戏中输入您的专属邀请码，即可为您增加一定的活动积分，届时您可以使用该积分兑换相应的奖励。/n您的专属邀请码为：xxx，您的累积积分为：xxx?
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, role.GetAccountReactiveKey(MapID, InstanceID, RoleID))
				msg.AddMsgEvent(MsgID, 9, rescore)
				msg.AddMsgEvent(MsgID, 9, gblevel+1)
				msg.AddMsgEvent(MsgID, 21, 4)
				msg.AddMsgEvent(MsgID, 1, 206219)					--“领取奖励
			else
				msg.AddMsgEvent(MsgID, 20, 444272)					--尊敬的xxx:您好！感谢您对圣魔之血的一贯支持，经过我们的综合认定，已经将您游戏中的本角色设置为勇士介绍人。在回归活动期间，只要回归勇士在游戏中输入您的专属邀请码，即可为您增加一定的活动积分，届时您可以使用该积分兑换相应的奖励。/n您的专属邀请码为：xxx，您的累积积分为：xxx?
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, role.GetAccountReactiveKey(MapID, InstanceID, RoleID))
				msg.AddMsgEvent(MsgID, 9, rescore)
			end
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 444256)					--“奖励一览
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 444257)					--“积分规则
			msg.AddMsgEvent(MsgID, 21, 7)
			msg.AddMsgEvent(MsgID, 1, 100002)					--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			if role.GetBagFreeSize(RoleID) > 0 then
				if rescore >= 1 and gblevel == 0 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700187, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 5 and gblevel == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700188, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 15 and gblevel == 2 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700189, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 25 and gblevel == 3 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700190, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 35 and gblevel == 4 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700191, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 60 and gblevel == 5 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700192, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 80 and gblevel == 6 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700193, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 120 and gblevel == 7 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700194, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 160 and gblevel == 8 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700195, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				elseif rescore >= 200 and gblevel == 9 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4700196, 1, -1, 8, 420)
					role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 444267)				--已领
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.AddMsgEvent(MsgID, 21, 7)
					msg.AddMsgEvent(MsgID, 1, 100002)				--“取消
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
				--[[
				for k,v in pairs(gbtable) do
					if rescore >= v[1] and gblevel <= k then
						role.AddRoleItem(MapID, InstanceID, RoleID, v[2], 1, -1, 8, 420)
						role.SetAccountReactiveLevel(MapID, InstanceID, RoleID,role.GetAccountReactiveLevel(MapID, InstanceID, RoleID)+1)
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 444267)				--已领
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.AddMsgEvent(MsgID, 21, 7)
						msg.AddMsgEvent(MsgID, 1, 100002)				--“取消
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
					break
				end
				--]]
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444266)				--背包空间不够
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 7)
				msg.AddMsgEvent(MsgID, 1, 100002)			--“取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444261)				--对应奖励
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 444258)				--“礼包内容
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 443012)				--“返回
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 6 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444259)				--积分规则
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 443012)				--“返回
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 8 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444260)				--礼包内容
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 9)
			msg.AddMsgEvent(MsgID, 1, 444270)				--“返回
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 443012)				--“返回
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 9 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444271)				--礼包内容
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 444269)				--“返回
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 443012)				--“返回
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif status == 2 then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444262)					--尊敬的xxx:您好！很高兴您能重新回到广袤的圣魔大陆，圣魔的世界有您更加精彩。如果您有勇士介绍人的邀请码，输入对应的邀请码可以为对方增加一定积分。
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.AddMsgEvent(MsgID, 21, 4)
			msg.AddMsgEvent(MsgID, 1, 444263)					--“输入邀请码
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100002)					--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			if role.GetBagFreeSize(RoleID) > 0 then
				--输入邀请码
				role.PopInputReactiveKey(MapID, InstanceID, RoleID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444266)				--背包空间不够
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 7)
				msg.AddMsgEvent(MsgID, 1, 100002)				--“取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	else
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444265)					--对不起，您不具备此次的活动资格。
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100002)					--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5600078, 7, "c5600078_On_Talk")


function ReactiveKeySuccessed(MapID,InstanceID,RoleID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4700262, 1, -1, 8, 420)
end
aux.RegisterRoleEvent(85, "ReactiveKeySuccessed")
