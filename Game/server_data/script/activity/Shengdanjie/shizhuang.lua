--圣诞时装礼包
function shizhuang_CanUse(MapID, InstanceID, TypeID, RoleID)
	local FreeSize = role.GetBagFreeSize(RoleID)
	if (FreeSize < 4) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 4)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet, bIgnore
	end
end
aux.RegisterItemEvent(4800023, 0, "shizhuang_CanUse")

-- 使用
function shizhuang_onUse(MapID, InstanceID, TypeID, RoleID)
	local sex = role.GetRoleSex(RoleID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 8450001, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8450002, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8450003, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8450004, 1, -1, 8, 420)
	elseif sex == 0 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 8450011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8450012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8450013, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8450014, 1, -1, 8, 420)
	end
end
aux.RegisterItemEvent(4800023, 1, "shizhuang_onUse")
--圣诞祝福
function wangzhongwang_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,69364)
	if a == true then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206326)--您身上已经拥有圣诞的祝福，请到时间再来领取
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
	else
		return 1
	end
end
aux.RegisterQuestEvent(30713, 4, "wangzhongwang_Complete")
function wangzhongwang_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	unit.AddBuff(MapID, InstanceID, RoleID, 6936401, RoleID)
	local QuestID1 = msg.BeginMsgEvent()
	msg.AddMsgEvent(QuestID1, 26, 0)
	msg.AddMsgEvent(QuestID1, 1, 206327)--您已获得圣诞的祝福，目前状态变的更好了
	msg.DispatchRoleMsgEvent(RoleID, QuestID1)
end
aux.RegisterQuestEvent(30713, 1, "wangzhongwang_OnComplete")

--槲寄生经验金钱奖励
function whujisheng_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level >= 30 and level <= 34 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 37500)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 90000)
	elseif level >= 35 and level <= 39 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 75000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 120000)
	elseif level >= 40 and level <= 44 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 120000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 150000)
	elseif level >= 45 and level <= 49 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 180000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 215000)
	elseif level >= 50 and level <= 54 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 300000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 300000)
	elseif level >= 55 and level <= 59 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 450000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 375000)
	elseif level >= 60 and level <= 64 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 675000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 450000)
	elseif level >= 65 and level <= 69 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 1125000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 525000)
	elseif level >= 70 and level <= 74 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 1500000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 600000)
	elseif level >= 75 and level <= 79 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 2250000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 675000)
	elseif level >= 80 and level <= 84 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 3750000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 750000)
	elseif level >= 85 and level <= 89 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 6000000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 775000)
	elseif level >= 90 and level <= 94 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 7500000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 900000)
	elseif level >= 95 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 9000000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 1200000)
	end
end
aux.RegisterQuestEvent(30679, 1, "whujisheng_OnComplete")

--完成圣诞兔子任务给个BUFF
function R30723_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	unit.AddBuff(MapID, InstanceID, RoleID, 6936501, RoleID)--1.5经验buff
end
aux.RegisterQuestEvent(30723, 1, "R30723_OnComplete")
