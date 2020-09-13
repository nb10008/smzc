--role.GetRoleScriptData(RoleID, 1, RoleDataType["IsToday"])
--role.GetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"]) 1000位


--检测接取
function q30335_On_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local CurTime = tonumber(os.date("%j"))
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["IsToday"])
	if CurTime ~= UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["IsToday"], CurTime)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FengZhiShiZhe"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"], role.GetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"])%1000)
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"], role.GetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"])%100)
	end
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["FengZhiShiZhe"]) < 3 then
	    local bool = false
		for i=30336,30349 do
			if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, i) == true then
				bool = true
				break
			end
		end
		if bool then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444014)				--删除速度极限任务之后才能接取此任务
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			return 1
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444015)				--只能接取3次
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(30335, 4, "q30335_On_CheckAccept")

--检测接取
function q30335_On_Accept(MapID, InstanceID, QuestID, RoleID, NPCID)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["FengZhiShiZhe"], role.GetRoleScriptData(RoleID, 1, RoleDataType["FengZhiShiZhe"])+1)
end
aux.RegisterQuestEvent(30335, 0, "q30335_On_Accept")

--s05 d03 s06 d05 d06 s07 s08 d09可用
function i4400603_CanUse(MapID, InstanceID, ItemTypeID, TargetID)

	local a = role.GetRoleQuestNum(TargetID)
	if role.GetBagFreeSize(TargetID) < 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		return 32,false
	elseif MapID ~= 3424073512 then
	    return 47,false
	elseif a >= 30 then
		return 21,false
	else
		return 0,false
	end
end
aux.RegisterItemEvent(4400603, 0, "i4400603_CanUse")

function i4400603_Use(MapID, InstanceID, ItemTypeID, TargetID)
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if rolelevel < 31 then
	    rolelevel = 31
	elseif rolelevel > 100 then
		rolelevel = 100
	end
	local QuestID = math.floor((rolelevel - 31)/5)+ 30336
	role.AddQuest(TargetID, QuestID)
    unit.AddBuff(MapID, InstanceID, TargetID, (61017+QuestID-30336)*100+1, TargetID)
	unit.AddBuff(MapID, InstanceID, TargetID, 6101601, TargetID)
	local HorseID = {4220105,4220206,4220305,4220406,4220505,4220606,4220805,4220905,4221005,4221105,4221209,4221210,4221305,4221405}
    role.AddRoleItem(MapID, InstanceID, TargetID, HorseID[math.random(14)], 1, -1, 8, 420)
end
aux.RegisterItemEvent(4400603, 1, "i4400603_Use")

--完成任务
function q30336_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
    local num = 0
	for i=30336,30349 do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, i) == true then
			num = num + 1
		end
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61016) and num <= 1 then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 6101601)
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61017+QuestID-30336) then
	    unit.CancelBuff(MapID, InstanceID, RoleID, (61017+QuestID-30336)*100+1)
		--给额外奖励
		local Role_exp = {}
		Role_exp[30336] = 12300
		Role_exp[30337] = 20300
		Role_exp[30338] = 26000
		Role_exp[30339] = 35200
		Role_exp[30340] = 52200
		Role_exp[30341] = 67000
		Role_exp[30342] = 74200
		Role_exp[30343] = 82800
		Role_exp[30344] = 92200
		Role_exp[30345] = 104400
		Role_exp[30346] = 118100
		Role_exp[30347] = 134000
		Role_exp[30348] = 153400
		Role_exp[30349] = 176400

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 444016)		--在规定时间内完成速度极限任务，获得额外经验奖励XXX点！
		msg.AddMsgEvent(MsgID, 9, Role_exp[QuestID])
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
        role.AddRoleExp(MapID, InstanceID, RoleID, Role_exp[QuestID])
	end
	--玩家统计
	UpdateRoleData(RoleID, 17, 1)
end
aux.RegisterQuestEvent(30336, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30337, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30338, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30339, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30340, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30341, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30342, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30343, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30344, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30345, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30346, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30347, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30348, 1, "q30336_OnComplete")
aux.RegisterQuestEvent(30349, 1, "q30336_OnComplete")

--删除任务
function q30336_OnDelete(MapID, InstanceID, QuestID, RoleID)
    local num = 0
	for i=30336,30349 do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, i) == true then
			num = num + 1
		end
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61016) and num <= 1 then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 6101601)
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61017+QuestID-30336) then
	    unit.CancelBuff(MapID, InstanceID, RoleID, (61017+QuestID-30336)*100+1)
	end
end
aux.RegisterQuestEvent(30336, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30337, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30338, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30339, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30340, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30341, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30342, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30343, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30344, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30345, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30346, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30347, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30348, 2, "q30336_OnDelete")
aux.RegisterQuestEvent(30349, 2, "q30336_OnDelete")
