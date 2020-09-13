ganenlibao_Times = 0
longshenxianlingId = 0

function ganenjieshuaxin()
	longshenxianlingId = map.MapCreateColCreature(3424073512, -1, 5610087,1023,23692,782, 1)
	local MsgID1 = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID1, 102, 300071)--圣恩仙灵已经出现在斗战天城（1023,782）处，快去领取祈福仙令吧
	msg.DispatchBroadcast(MsgID1,-1,-1,-1)
	act.SetActScriptData(49, 1, 3,0)
	act.SetActScriptData(49, 1, 4,0)
	act.SetActScriptData(49, 1, 5,0)
end

function ganenjieshanchu()
	map.MapDeleteCreature(3424073512, -1, longshenxianlingId)
end
--圣恩仙灵对话
function shengenxianling_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curhour = tonumber(os.date("%H"))--小时
	local curmin = tonumber(os.date("%M"))--分钟
	local theDays = tonumber(os.date("%j"))
	local xianling = role.GetRoleScriptData(RoleID, 1, RoleDataType["ganenqifuxianling"])
	local libao = role.GetRoleScriptData(RoleID, 1, RoleDataType["ganenqifulibao"])
    if TalkIndex == -1 and (curhour < 21 or (curhour== 21 and curmin < 10))then
        local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 20, 206904)   --对话
	    msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
	    msg.AddMsgEvent(MsgID, 21,	4)			--领取仙令
	    msg.AddMsgEvent(MsgID, 1,206905)			--“领取仙令”
	    msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
	    msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == -1 and curhour == 21 and curmin >= 10 then
        local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 20, 206904)   --对话
	    msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
	    msg.AddMsgEvent(MsgID, 21,	5)			--领取礼包
	    msg.AddMsgEvent(MsgID, 1,206906)			--“领取礼包”
	    msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
	    msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if(theDays == xianling) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206908)   --对话
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local freeSize = role.GetBagFreeSize(RoleID)
			if(freeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 100003)   --对话
				msg.AddMsgEvent(MsgID, 9,	1)
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800194, 1, -1, 8, 420)
				unit.AddBuff(MapID, InstanceID, RoleID,6105701,RoleID)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["ganenqifuxianling"],theDays)
			end
		end
	elseif TalkIndex == 5 then
		if(theDays == libao) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206911)   --对话
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local num = role.GetRoleItemNum(RoleID, 4800194)
			if(num == nil or num == 0) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206910)   --对话
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif(role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61057)) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206909)   --对话
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif(role.GetBagFreeSize(RoleID) < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 100003)   --对话
				msg.AddMsgEvent(MsgID, 9,	1)
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
				if(level < 50) then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800195, 1, -1, 8, 420)
					ganenlibao_Times = ganenlibao_Times+1
				else
					ganenlibao_Times = ganenlibao_Times+1
					local change = libaopanduan(RoleID)
						if(change == 3) then
							role.AddRoleItem(MapID, InstanceID, RoleID, 4700285, 1, -1, 8, 420)
							local MsgID2 = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID2, 102, 206914)--玩家&lt;p1&gt;的诚心感动天神，获得金龙祈福礼包！
							msg.AddMsgEvent(MsgID2, 2, RoleID)
							msg.DispatchBroadcast(MsgID2,-1,-1,-1)
						elseif(change == 2) then
							role.AddRoleItem(MapID, InstanceID, RoleID, 4700286, 1, -1, 8, 420)
						elseif(change == 1) then
							role.AddRoleItem(MapID, InstanceID, RoleID, 4800196, 1, -1, 8, 420)
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, 4800195, 1, -1, 8, 420)
						end
				end
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800194, num, 101)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["ganenqifulibao"],theDays)
			end
		end
	end

end
aux.RegisterCreatureEvent(5610087, 7, "shengenxianling_On_Talk")

function libaopanduan(RoleID)
	local jinlongqifulibao = act.GetActScriptData(49, 1, 3)
	local yinlongqifulibao = act.GetActScriptData(49, 1, 4)
	local ganenlibao = act.GetActScriptData(49, 1, 5)
	local temp = math.random(10)
	local change = 0
	if (ganenlibao_Times <= 10) then
		if(temp ==ganenlibao_Times ) then
			if(jinlongqifulibao < 3) then
				change = 3
				act.SetActScriptData(49, 1, 3, jinlongqifulibao+1)
			elseif(jinlongqifulibao >= 3 and yinlongqifulibao < 10) then
				change = 2
				act.SetActScriptData(49, 1, 4, yinlongqifulibao+1)
			elseif(yinlongqifulibao >= 10 and ganenlibao < 20) then
				change = 1
				act.SetActScriptData(49, 1, 5, ganenlibao+1)
			end
		else
			if(yinlongqifulibao < 10) then
				change = 2
				act.SetActScriptData(49, 1, 4, yinlongqifulibao+1)
			elseif(yinlongqifulibao >=10 and ganenlibao < 20) then
				change = 1
				act.SetActScriptData(49, 1, 5, ganenlibao+1)
			end
		end
	elseif(ganenlibao_Times%10 == 0) then
		if(jinlongqifulibao < 3) then
			change = 3
			act.SetActScriptData(49, 1, 3, jinlongqifulibao+1)
		elseif(jinlongqifulibao >= 3 and yinlongqifulibao < 10) then
			change = 2
			act.SetActScriptData(49, 1, 4, yinlongqifulibao+1)
		elseif(yinlongqifulibao >= 10 and ganenlibao < 20) then
			change = 1
			act.SetActScriptData(49, 1, 5, ganenlibao+1)
		end
	end
	return change
end
