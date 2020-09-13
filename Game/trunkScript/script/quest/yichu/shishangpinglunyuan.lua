--时尚评论员
--记录领取奖励 RoleDataType["ShiShangPingLun"] = 72 存两位数，左位已领，右位待领
function c5610052On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	local _, level1, level2, level3, suitSum = role.GetWardrobeDetailInfo(MapID,InstanceID,RoleID)
	local collected = math.floor((level1+level2+level3)/suitSum*100)
	local done = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"])/10)
	local otable = {}
	otable[4] = 100
	otable[3] = 80
	otable[2] = 60
	otable[1] = 40
	otable[0] = 0
	for i=4,0,-1 do
		if collected >= otable[i] then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"], done*10+i)
			break
		end
	end
	local can = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"])%10
	--
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444113) 			--
		msg.AddMsgEvent(MsgID, 21, 4) 				--按钮
		msg.AddMsgEvent(MsgID, 1, 444107) 			--评价时装收集成果
		msg.AddMsgEvent(MsgID, 21, 7) 				--按钮
		msg.AddMsgEvent(MsgID, 1, 444114) 			--查询时装状态奖励相关
		msg.AddMsgEvent(MsgID, 21, 5)   			--按钮
		msg.AddMsgEvent(MsgID, 1, 444112)   		--离开
		msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444102) 		--您的背包已满，无法获得奖励，请清理后再来
			msg.AddMsgEvent(MsgID, 21, 5)   		--按钮
			msg.AddMsgEvent(MsgID, 1, 444112)   	--取消
			msg.AddMsgEvent(MsgID, 24, CreatureID) 	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if can > 0 and done < 5 then
				local MsgID = msg.BeginMsgEvent()
				if done >= can then
					msg.AddMsgEvent(MsgID, 20, 444115) 			--您的收集率已经达到xxx%，还没有达到下一阶段奖励的领取要求。注：当收集率超过40%、60%、80%或者达到100%会得到不同的称号奖励。
					msg.AddMsgEvent(MsgID, 9, collected)
				else
					msg.AddMsgEvent(MsgID, 20, 444108) 			--您的收集率已经达到xxx%，可以领取第x阶段的奖励。注：当收集率超过40%、60%、80%或者达到100%会得到不同的称号奖励。
					msg.AddMsgEvent(MsgID, 9, collected)
					msg.AddMsgEvent(MsgID, 9, done+1)
					msg.AddMsgEvent(MsgID, 21, 6) 				--按钮
					msg.AddMsgEvent(MsgID, 1, 444105) 			--领取奖励
				end
				msg.AddMsgEvent(MsgID, 21, -1) 				--按钮
				msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif done > 4 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444109) 			--您已经获得过了最高评价
				msg.AddMsgEvent(MsgID, 21, 5)   			--按钮
				msg.AddMsgEvent(MsgID, 1, 444112)   		--离开
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444115) 			--您的收集率已经达到xxx%，还没有达到下一阶段奖励的领取要求。注：当收集率超过40%、60%、80%或者达到100%会得到不同的称号奖励。
				msg.AddMsgEvent(MsgID, 9, collected)
				msg.AddMsgEvent(MsgID, 9, done+1)
				msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
				msg.AddMsgEvent(MsgID, 1, 444112) 			--取消
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 6 then
		if role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444102) 		--您的背包已满，无法获得奖励，请清理后再来
			msg.AddMsgEvent(MsgID, 21, 5)   		--按钮
			msg.AddMsgEvent(MsgID, 1, 444112)   	--取消
			msg.AddMsgEvent(MsgID, 24, CreatureID) 	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local ptable = {}
			ptable[0] = {40, 3080034}
			ptable[1] = {60, 3080035}
			ptable[2] = {80, 3080036}
			ptable[3] = {100, 3080037}
			for i=0,3 do
				if done == i and collected >= ptable[i][1] then
					role.SetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"], 10*(i+1))
					role.AddRoleItem(MapID, InstanceID, RoleID, ptable[i][2], 1, -1, 8, 104)
					break
				end
			end
		end
	elseif TalkIndex == 7 then
		if done > 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444111) 					--时装状态奖励和衣橱中时装收集数量有关，以下是详细的内容。
			msg.AddMsgEvent(MsgID, 21, -1)   					--按钮
			msg.AddMsgEvent(MsgID, 1, 100002)   				--返回
			msg.AddMsgEvent(MsgID, 24, CreatureID) 				--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444116) 					--您尚未开启衣橱时装收集状态奖励功能。提示：当您时装收集率首次达到40%即可开启状态奖励机制。
			msg.AddMsgEvent(MsgID, 21, -1)   					--按钮
			msg.AddMsgEvent(MsgID, 1, 100002)   				--返回
			msg.AddMsgEvent(MsgID, 24, CreatureID) 				--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5610052, 7, "c5610052On_Talk")
