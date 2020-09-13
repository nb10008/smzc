tianshihulalal = 0 
tianshihuhaha = 0 
--act.GetActScriptData(55, 1, 4)--已领取
--act.GetActScriptData(55, 1, 2)--已领取者ID
function n6040056_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local act55kai = act.GetActScriptData(55, 1, 4)--活动开启
	local act55ren = act.GetActScriptData(55, 1, 2)--第一个到达80级的玩家ID
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206630) -- "天使呼哈哈:\n    新的世界新的开始。\n    只有80级以上并且第一名来到此地的人才能领取奖励，并且开启每日升级活动。
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 4) -- 
		msg.AddMsgEvent(MsgID, 1, 206631) -- 领取奖励
		msg.AddMsgEvent(MsgID, 21, 5) -- 
		msg.AddMsgEvent(MsgID, 1, 206632) -- 查看领取者姓名
		msg.AddMsgEvent(MsgID, 21, 10)   --
		msg.AddMsgEvent(MsgID, 1, 100002)   --"取消"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	elseif TalkIndex == 4 then--580，23510，881 NPC 坐标
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level >= 80 then
			if act55kai ~= 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206633) -- "天使呼哈哈\n    领取仅仅属于你的奖励吧。
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21,6) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			else
				local MsgID = msg.BeginMsgEvent()
				if RoleID ~= act55ren then
					msg.AddMsgEvent(MsgID, 20, 206634) -- "天使呼哈哈\n    这个奖励已经有人领取了，你可以在“查看领取者姓名”中查看玩家姓名
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, 5) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 206632) -- “确定
				else
					msg.AddMsgEvent(MsgID, 20, 206637) -- 天使呼哈哈:\n    你已经领取过奖励了，想重复领取和不行哦。
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				end
				msg.AddMsgEvent(MsgID, 21, 10) -- 
				msg.AddMsgEvent(MsgID, 1, 100002) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206639) -- 
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 10) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206635) -- "天使呼哈哈:\n    xx（玩家名）第一个成为全服80级的玩家，并成功开启本服务器每日升级活动！
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 2, act55ren)
		msg.AddMsgEvent(MsgID, 21, 10)   --
		msg.AddMsgEvent(MsgID, 1, 100001)   --"确定"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	elseif TalkIndex == 6 then
		if role.GetBagFreeSize(RoleID) < 4 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206636) -- "天使呼哈哈:\n    你的背包空间不足3
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 7)   --
			msg.AddMsgEvent(MsgID, 1, 100001)   --"确定"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		else
			local act55ling = act.GetActScriptData(55, 1, 3)
			if act55ling ~=1 then
				act.SetActScriptData(55, 1, 3, 1)
				act.SetActScriptData(55, 1, 2, RoleID)
				act.SetActScriptData(55, 1, 4, 1)
				act.SaveActScriptData(55)	
				tianshihulalal = map.MapCreateColCreature(3424073512, -1, 6040055, 580, 23510, 881, 1, "")
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102,300101)      --天使呼啦啦已经出现在斗战天城<580,881>，现在所有玩家可以去他那里领取每日升级任务啦。
				msg.DispatchWorldMsgEvent(MsgID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4820262, 9999, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3020095, 1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3020097, 1, -1, 8, 420)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 20000000)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206637) -- 天使呼哈哈:\n    你已经领取过奖励了，想重复领取和不行哦。
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, 10) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			end
		end
	end 
end

aux.RegisterCreatureEvent(6040056, 7, "n6040056_OnTalk")

function xinkaifuhuodong_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local high,low = role.GetRoleLevelUpExp(MapID, InstanceID, RoleID)--得到玩家升级所需经验
	local high1,low1 = role.GetRoleExp(MapID, InstanceID, RoleID)--得到玩家当前经验经验
	local x = (high*1000000000 + low) - (high1*1000000000 + low1)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level >= 10 and level <= 84 then
		role.AddRoleExp(MapID, InstanceID, RoleID, x)
	end
end
aux.RegisterQuestEvent(30797, 1, "xinkaifuhuodong_Complete")
function act55_OnTimerMin(actID)
	if tianshihulalal == 0 then
		local act55kai = act.GetActScriptData(55, 1, 4)--活动开启
		if xinkaifuhuodong == 1 then
			if act55kai == 1 then
				tianshihulalal = map.MapCreateColCreature(3424073512, -1, 6040055, 580, 23510, 881, 1, "")
			end
		end
	end
end
aux.RegisterActEvent(55, 4, "act55_OnTimerMin")