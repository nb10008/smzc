--与母亲节大使	5600056 对话，查看排名领取奖励
function yaojingdashi_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local La = role.GetRoleItemNum(RoleID, 4800090)--蓝色本源
	local Lv = role.GetRoleItemNum(RoleID, 4800089)--绿色本源
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	if curyear == 2011 and curmon <= 5 and curday < 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206470) -- "母亲节大使：\n   母亲节活动于011/04/28 ~ 2011/05/12之间进行。届时会有再来我这里会有活动，并且在七花之海会出现妖精之母。"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
		msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	elseif (curyear == 2011 and curmon == 5 and curday >= 4) or (curyear == 2011 and curmon >= 6) or (curyear > 2011)then
		--if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 30 then
			if TalkIndex == -1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206454) -- "母亲节大使：\n    母亲节活动于011/04/28 ~ 2011/05/12之间进行。\n    每缴交一颗蓝色魂体本源，个人可获得30点灵魂积分，该服务器可获得30点灵魂积分；每缴交一颗蓝绿色魂体本源，个人可获得5点灵魂积分，该服务器可获得5点灵魂积分。\n    前三名的服务器和前三名的玩家均可以活动相应的奖励。"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				--msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["huntijifen"]))
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 206336) -- “查看提交排行”
				msg.AddMsgEvent(MsgID, 21, 9)
				msg.AddMsgEvent(MsgID, 1, 206477) -- “查看本人积分”
				msg.AddMsgEvent(MsgID, 21, 10)
				msg.AddMsgEvent(MsgID, 1, 206481) -- 本服务器总积分
				if curyear == 2011 and curmon == 5 and (curday >= 4 and curday <= 17) then
					msg.AddMsgEvent(MsgID, 21, 4)
					msg.AddMsgEvent(MsgID, 1, 206455)   --"缴一颗蓝色魂体本源"
					msg.AddMsgEvent(MsgID, 21, 5)
					msg.AddMsgEvent(MsgID, 1, 206456)   --"缴一颗绿色魂体本源
				elseif curyear == 2011 and curmon == 5 and curday > 17 then
					msg.AddMsgEvent(MsgID, 21, 7)
					msg.AddMsgEvent(MsgID, 1, 206338)   --"领取排行奖励"
				end
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif TalkIndex == 6 then
				local k = nil
				for i = 0, 4 do
					if act.GetActScriptData(39, 1, i) ~= nil and act.GetActScriptData(39, 1, i) > 0 then
						k = i
					end
				end
				if k == nil then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206457) -- "母亲节大使：\n    因为还没有人缴纳魂体本源，则无法查询排行。"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				elseif k == 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206458) -- "母亲节大使：\n    现在魂体积分最高的玩家是：\n    XX已经获得XX分
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 0))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 5))
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				elseif k == 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206459) -- "母亲节大使：\n    现在魂体积分最高的玩家是：\n    XX已经获得XX分\n    XX已经获得XX分
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 0))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 5))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 1))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 6))
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				elseif k == 2 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206460) -- "母亲节大使：\n    现在魂体积分最高的玩家是：\n     XX已经获得XX分\n    XX已经获得XX分\n    XX已经获得XX分
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 0))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 5))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 1))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 6))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 2))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 7))
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				elseif k == 3 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206461) -- "母亲节大使：\n    现在魂体积分最高的玩家是：\n    XX已经获得XX分\n    XX已经获得XX分\n    XX已经获得XX分\n    XX已经获得XX分
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 0))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 5))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 1))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 6))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 2))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 7))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 3))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 8))
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				elseif k == 4 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206462) -- "母亲节大使：\n    现在魂体积分最高的玩家是：\n    XX已经获得XX分\n    XX已经获得XX分\n    XX已经获得XX分\n    XX已经获得XX分\n    XX已经获得XX分
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 0))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 5))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 1))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 6))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 2))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 7))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 3))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 8))
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(39, 1, 4))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 9))
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				end
			elseif TalkIndex == 4 then
				if La > 0 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800090, 1, 420)
					La_muqinjie(RoleID)--积分
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206463) -- "母亲节大使：\n    想要获得奖励就带更多的蓝色魂体本源来吧！"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206465) -- "母亲节大使：\n    你没有蓝色魂体本源，我可不想把时间浪费在你身上。"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				end
			elseif TalkIndex == 5 then
				if Lv > 0 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800089, 1, 420)
					Lv_muqinjie(RoleID)--积分
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206464) -- "母亲节大使：\n    想要获得奖励就带更多的绿色魂体本源来吧！"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206466) -- "母亲节大使：\n    你没有绿色魂体本源，我可不想把时间浪费在你身上。"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				end
			elseif TalkIndex == 7 then
				local Rank = 0
				for q=0,2 do
					if act.GetActScriptData(39, 1, q) == RoleID then
						Rank = q + 1
					end
				end
				if Rank==0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206467) -- "母亲节大使：\n    前三名里面没有你的名字哦，我没有什么东西可以给你。"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				elseif act.GetActScriptData(39, 1, Rank+9) > 0 then
					 local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206468) -- "母亲节大使：\n    你已经领取过奖励了，想重复领取和不行哦。"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				elseif role.GetBagFreeSize(RoleID) < 3 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206469) -- "母亲节大使：\n    快去清理一下背包吧，不然是装不下奖品的哦。"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				else
					local Vocation = role.GetRoleClass(RoleID)
					if RoleID == act.GetActScriptData(39, 1, 0) then
						if Vocation == 1 or Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9 then
							role.AddRoleItem(MapID, InstanceID, RoleID, 8170110, 1, -1, 8, 420) --冰雪寒灵（粉）
						elseif Vocation == 5 or Vocation == 4 or Vocation == 13 or Vocation == 10 or Vocation == 11 or Vocation == 12 then
							role.AddRoleItem(MapID, InstanceID, RoleID, 8170111, 1, -1, 8, 420) --媚惑使魔（粉）
						end
						role.AddRoleItem(MapID, InstanceID, RoleID, 4090020, 10, -1, 8, 420) --10
					elseif RoleID == act.GetActScriptData(39, 1, 1) then
						if Vocation == 1 or Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9 then
							role.AddRoleItem(MapID, InstanceID, RoleID, 8170110, 1, -1, 8, 420) --冰雪寒灵（粉）
						elseif Vocation == 5 or Vocation == 4 or Vocation == 13 or Vocation == 10 or Vocation == 11 or Vocation == 12 then
							role.AddRoleItem(MapID, InstanceID, RoleID, 8170111, 1, -1, 8, 420) --媚惑使魔（粉）
						end
						role.AddRoleItem(MapID, InstanceID, RoleID, 4090020, 5, -1, 8, 420) --5
					elseif RoleID == act.GetActScriptData(39, 1, 2) then
						if Vocation == 1 or Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9 then
							role.AddRoleItem(MapID, InstanceID, RoleID, 8170110, 1, -1, 8, 420) --冰雪寒灵（粉）
						elseif Vocation == 5 or Vocation == 4 or Vocation == 13 or Vocation == 10 or Vocation == 11 or Vocation == 12 then
							role.AddRoleItem(MapID, InstanceID, RoleID, 8170111, 1, -1, 8, 420) --媚惑使魔（粉）
						end
						role.AddRoleItem(MapID, InstanceID, RoleID, 4090020, 2, -1, 8, 420) --10
					end
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800091, 1, -1, 8, 420) --无以回报的爱
					act.SetActScriptData(39, 1, Rank+9, 1)
					act.SaveActScriptData(39)
				end
			elseif  TalkIndex == 9 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206478) -- 母亲节大使：\n    当前拥有的积分为：
				msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["huntijifen"]))
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			elseif  TalkIndex == 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206478) -- 母亲节大使：\n    当前拥有的积分为：
				msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(39, 1, 20))
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			
			end
		--else
		--	local MsgID = msg.BeginMsgEvent()
		--	msg.AddMsgEvent(MsgID, 20, 206476) -- 您等级不足30级，无法参加此活动。
		--	msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		--	msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
		--	msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
		--	msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		--end
	end
end
aux.RegisterCreatureEvent(5600056, 7, "yaojingdashi_OnTalk")

function La_muqinjie(RoleID)
    local i = role.GetRoleScriptData(RoleID, 1, RoleDataType["huntijifen"])
    local x = act.GetActScriptData(39, 1, 20)
	if i == nil then
	    i = 0
	end
	for q=0,4 do                       --容错性修改
	    if act.GetActScriptData(39, 1, q) == RoleID then
		    if act.GetActScriptData(39, 1, q+5) >= i then
			    i = act.GetActScriptData(39, 1, q+5)
			end
		end
	end
	i = i + 30
	x = x + 30
	act.SetActScriptData(39, 1, 20, x)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["huntijifen"], i)
	local arrID = {}
	local arrNum = {}
	local bNew = 1
	for l=0,4 do
		arrID[l] = act.GetActScriptData(39, 1, l)
		if arrID[l] == nil then
		    arrID[l] = 0
		end
		if arrID[l] == RoleID  then
			arrNum[l] = i
			bNew = 0
		else
			arrNum[l] = act.GetActScriptData(39, 1, l+5)
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
		act.SetActScriptData(39, 1, j, arrID[j])
		act.SetActScriptData(39, 1, j+5, arrNum[j])
	end

	act.SaveActScriptData(39)

end
function Lv_muqinjie(RoleID)
    local i = role.GetRoleScriptData(RoleID, 1, RoleDataType["huntijifen"])
     local x = act.GetActScriptData(39, 1, 20)
	if i == nil then
	    i = 0
	end
	for q=0,4 do                       --容错性修改
	    if act.GetActScriptData(39, 1, q) == RoleID then
		    if act.GetActScriptData(39, 1, q+5) >= i then
			    i = act.GetActScriptData(39, 1, q+5)
			end
		end
	end
	i = i + 5
	x = x + 5
	act.SetActScriptData(39, 1, 20, x)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["huntijifen"], i)
	local arrID = {}
	local arrNum = {}
	local bNew = 1
	for l=0,4 do
		arrID[l] = act.GetActScriptData(39, 1, l)
		if arrID[l] == nil then
		    arrID[l] = 0
		end
		if arrID[l] == RoleID  then
			arrNum[l] = i
			bNew = 0
		else
			arrNum[l] = act.GetActScriptData(39, 1, l+5)
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
		act.SetActScriptData(39, 1, j, arrID[j])
		act.SetActScriptData(39, 1, j+5, arrNum[j])
	end

	act.SaveActScriptData(39)

end
