--气质测评师
--记录领取奖励 RoleDataType["QiZhiCePing"] = 71 存两位数，左位已领，右位待领
function c5610051On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"]) == 0 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"], 10)
	end
	local done = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"])/10)
	local qizhiSum = role.GetWardrobeDetailInfo(MapID,InstanceID,RoleID)  --获得气质
	local mtable = {}
	mtable[6] = 1000
	mtable[5] = 750
	mtable[4] = 500
	mtable[3] = 350
	mtable[2] = 200
	mtable[1] = 50
	mtable[0] = 0
	for i=6,0,-1 do
		if qizhiSum and qizhiSum >= mtable[i] then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"], done*10+i)
			break
		end
	end
	local can = role.GetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"])%10
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444100) 		--
		msg.AddMsgEvent(MsgID, 21, 4) 			--按钮
		msg.AddMsgEvent(MsgID, 1, 444101) 		--评价我的气质总和
		msg.AddMsgEvent(MsgID, 21, 5)   		--按钮
		msg.AddMsgEvent(MsgID, 1, 444112)   	--离开
		msg.AddMsgEvent(MsgID, 24, CreatureID) 	--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444102) 			--您的背包已满，无法获得奖励，请清理后再来
			msg.AddMsgEvent(MsgID, 21, 5)   			--按钮
			msg.AddMsgEvent(MsgID, 1, 444112)   		--取消
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if can > 0 and done < 7 then
				local MsgID = msg.BeginMsgEvent()
				if done > can then
					msg.AddMsgEvent(MsgID, 20, 444103)			--您当前的气质总和为：xxx，还没有达到下一阶段奖励的领取要求。\n注：第1阶段奖励需要气质50-199才能领取
					msg.AddMsgEvent(MsgID, 9, qizhiSum)
				else
					msg.AddMsgEvent(MsgID, 20, 444104)			--您当前的气质总和为：xxx，可以领取第x阶段的奖励。\n注：第1阶段奖励需要气质50-199才能领取
					msg.AddMsgEvent(MsgID, 9, qizhiSum)
					msg.AddMsgEvent(MsgID, 9, done)
					msg.AddMsgEvent(MsgID, 21, 6) 					--按钮
					msg.AddMsgEvent(MsgID, 1, 444105) 				--领取奖励
				end
				msg.AddMsgEvent(MsgID, 21, 5) 					--按钮
				msg.AddMsgEvent(MsgID, 1, 444112) 				--取消
				msg.AddMsgEvent(MsgID, 24, CreatureID) 			--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif done > 6 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444106) 				--您人中龙凤的称号实至名归！
				msg.AddMsgEvent(MsgID, 21, 5)   				--按钮
				msg.AddMsgEvent(MsgID, 1, 444112)   			--取消
				msg.AddMsgEvent(MsgID, 24, CreatureID) 			--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444103) 			--您当前的气质总和为：xxx，还没有达到下一阶段奖励的领取要求。\n注：第1阶段奖励需要气质50-199才能领取
				msg.AddMsgEvent(MsgID, 9, qizhiSum)
				msg.AddMsgEvent(MsgID, 21, 5)   			--按钮
				msg.AddMsgEvent(MsgID, 1, 444112)   		--取消
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 6 then
		if role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444102) 			--您的背包已满，无法获得奖励，请清理后再来
			msg.AddMsgEvent(MsgID, 21, 5)   			--按钮
			msg.AddMsgEvent(MsgID, 1, 444112)   		--取消
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local s = role.GetRoleSex(RoleID)
			local ntable = {}
			ntable[1] = {3080022,3080023}
			ntable[2] = {3080024,3080025}
			ntable[3] = {3080026,3080027}
			ntable[4] = {3080028,3080029}
			ntable[5] = {3080030,3080031}
			ntable[6] = {3080032,3080033}
			for k,v in pairs(ntable) do
				if done == k and can > k - 1 then
					if s == 1 then
						role.AddRoleItem(MapID, InstanceID, RoleID, v[1], 1, -1, 8, 104)
					else
						role.AddRoleItem(MapID, InstanceID, RoleID, v[2], 1, -1, 8, 104)
					end
					role.SetRoleScriptData(RoleID, 1, RoleDataType["QiZhiCePing"], 10*(k+1))
					break
				end
			end
		end
	end
end
aux.RegisterCreatureEvent(5610051, 7, "c5610051On_Talk")
