
--与5600015对话
function n5600015_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	if curhour >= 14 and curhour <= 16 then
		curhour = 1
	elseif curhour >= 17 and curhour <= 20 then
		curhour = 2
	elseif curhour >= 21 and curhour <= 23 then
		curhour = 3
	else
		curhour = 4
	end
	local time1 = curmon*1000 + curday *10 + curhour
	if curhour ~= 4 and time1 ~= role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin_time"]) then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin_time"], time1)
	end
	if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206335) -- "新年宴会供应商：\n    每日14点至14点30分、17点至17点30分、21点至21点30分，新年宴会供应将会开始，届时就可以缴纳新年大餐来获得各种奖励。新年大餐可在新年壁炉处制作。\n    提交新年大餐可以获得新年宝箱的奖励，每个时间段提交新年大餐最多的3个人还会获得特殊的额外奖励。\n 本次活动你提交的新年大餐： XX个。"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"]))
		msg.AddMsgEvent(MsgID, 21, 5) -- 确定按钮
		msg.AddMsgEvent(MsgID, 1, 206336) -- “查看提交排行”
		if Xinniandayan_start == 1 then
			msg.AddMsgEvent(MsgID, 21, 4)   --取消按钮
			msg.AddMsgEvent(MsgID, 1, 206337)   --"缴纳新年大餐"
		else
		    msg.AddMsgEvent(MsgID, 21, 6)   --取消按钮
			msg.AddMsgEvent(MsgID, 1, 206338)   --"领取排行奖励"
		end
		msg.AddMsgEvent(MsgID, 21, 10)   --取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)   --"取消"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
    elseif TalkIndex == 4 then
	    if Xinniandayan_start == 1 then
			local a = role.GetRoleItemNum(RoleID, 4800017)
			if a > 0 then
			    if role.GetBagFreeSize(RoleID) < 1 then
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206339) -- "新年宴会供应商：\n    难道你不想要提交新年大餐的报酬吗？快去清理一下背包再来吧！"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				else
				    role.AddRoleItem(MapID, InstanceID, RoleID, 4800018, 1, -1, 8, 420)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800017, 1, 420)
					unit.AddBuff(MapID, InstanceID, RoleID, 2053901, RoleID)
					HuoDeXinnianshipin(RoleID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206340) -- "新年宴会供应商：\n    想要获得奖励就带更多的新年大餐来吧！"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206341) -- "新年宴会供应商：\n    你没有新年大餐，我可不想把时间浪费在你身上。"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			end
		end
    elseif TalkIndex == 5 then
	    local k = nil
		for i = 0, 4 do
		    if act.GetActScriptData(36, 1, i) ~= nil and act.GetActScriptData(36, 1, i) > 0 then
			    k = i
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206342) -- "新年宴会供应商：\n    现在还没有人缴纳过新年大餐。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206343) -- "新年宴会供应商：\n    现在提交新年大餐最多的玩家是：\n    XXX        XXX个新年大餐
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206344) -- "新年宴会供应商：\n    现在提交新年大餐最多的玩家是：\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 6))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206345) -- "新年宴会供应商：\n    现在提交新年大餐最多的玩家是：\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 7))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 3 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206346) -- "新年宴会供应商：\n    现在提交新年大餐最多的玩家是：\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 7))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 8))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 4 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206347) -- "新年宴会供应商：\n    现在提交新年大餐最多的玩家是：\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐\n    XXX        XXX个新年大餐
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 7))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 8))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(36, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(36, 1, 9))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end
	elseif TalkIndex == 6 then
	    local Rank = 0
        for q=0,2 do
			if act.GetActScriptData(36, 1, q) == RoleID then
				Rank = q + 1
			end
		end
        if Rank==0 then
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206348) -- "新年宴会供应商：\n    前三名里面没有你的名字哦，我没有什么东西可以给你。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
        elseif act.GetActScriptData(36, 1, Rank+9) > 0 then
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206349) -- "新年宴会供应商：\n    你已经领取过奖励了，想重复领取和不行哦。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
        elseif role.GetBagFreeSize(RoleID) < 1 then
        	local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206350) -- "新年宴会供应商：\n    快去清理一下背包吧，不然是装不下奖品的哦。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4800019, 1, -1, 8, 420)
			act.SetActScriptData(36, 1, Rank+9, 1)
			act.SaveActScriptData(36)
		end
    end
end

aux.RegisterCreatureEvent(5600015, 7, "n5600015_OnTalk")

function HuoDeXinnianshipin(RoleID)
    local i = role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"])
	if i == nil then
	    i = 0
	end
	for q=0,4 do                       --容错性修改
	    if act.GetActScriptData(36, 1, q) == RoleID then
		    if act.GetActScriptData(36, 1, q+5) >= i then
			    i = act.GetActScriptData(36, 1, q+5)
			end
		end
	end
	i = i + 1
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"], i)

	--[[if i % 100 == 0 and i ~= 0 and i ~= nil then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 2370)  --<玩家名>在本次活动中已经交纳了XXX个新年大餐，真是无人能比！
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 9, i)
		msg.DispatchWorldMsgEvent(MsgID)
	end]]

	local arrID = {}
	local arrNum = {}
	local bNew = 1
	for l=0,4 do
		arrID[l] = act.GetActScriptData(36, 1, l)
		if arrID[l] == nil then
		    arrID[l] = 0
		end
		if arrID[l] == RoleID  then
			arrNum[l] = i
			bNew = 0
		else
			arrNum[l] = act.GetActScriptData(36, 1, l+5)
			if arrNum[l] == nil then
		        arrNum[l] = 0
		    end
		end
	end

	if bNew == 1 then
		arrID[5] = RoleID;
		arrNum[5] = i;
	else
		arrID[5] = 0;
		arrNum[5] = 0;
	end

	for m=0, 4 do
		for n=m+1,5 do
			if arrNum[n] > arrNum[m] then
				local temp = arrNum[m]
				arrNum[m] = arrNum[n]
				arrNum[n] = temp
				temp = arrID[m]
				arrID[m] = arrID[n]
				arrID[n] = temp
			end
		end
	end

	for j=0,4 do
		act.SetActScriptData(36, 1, j, arrID[j])
		act.SetActScriptData(36, 1, j+5, arrNum[j])
	end

	act.SaveActScriptData(36)

end












