
--新春时装礼包
function shizhuang_CanUse(MapID, InstanceID, TypeID, RoleID)
	local FreeSize = role.GetBagFreeSize(RoleID)
	local bRet
	if (FreeSize < 4) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 4)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet
	end
end
aux.RegisterItemEvent(4800029, 0, "shizhuang_CanUse")

-- 使用
function X_shizhuang_onUse(MapID, InstanceID, TypeID, RoleID)
	local sex = role.GetRoleSex(RoleID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 8360001, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8360002, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8360004, 1, 2, 8, 420)
	elseif sex == 0 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 8360011, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8360012, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8360013, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 8360014, 1, 2, 8, 420)
	end
end
aux.RegisterItemEvent(4800029, 1, "X_shizhuang_onUse")
--情人节 
--任务接取
function QingrenM_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)--男
	local sex = role.GetRoleSex(RoleID)
	
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_quest"])
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_update"])

	if 0 == TotalNum then
		TotalNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_quest"], TotalNum)
	end
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_quest"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_update"], CurTime)
	end
	if sex == 1 then
		if TotalNum > 5 then 
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 206529)--每天只能完成5次该任务！
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		else 
			return 1
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206550)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(30744, 4, "QingrenM_OnComplete")

function QingrenN_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)--女

	
	local sex = role.GetRoleSex(RoleID)
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_quest"])
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_update"])

	if 0 == TotalNum then
		TotalNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_quest"], TotalNum)
	end
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_quest"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_update"], CurTime)
	end
	if sex == 0 then
		if TotalNum > 5 then 
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 206529)--每天只能完成5次该任务！
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		else 
			return 1
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206550)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(30745, 4, "QingrenN_OnComplete")

-- 任务完成
function Qingren_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local Step = role.GetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_quest"])
	if QuestID == 30744 then
		Step = Step + 1
	elseif QuestID == 30745 then
		Step = Step + 1
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_quest"], Step)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level >= 30 and level <= 34 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 4200)
	elseif level >= 35 and level <= 39 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 4940)
	elseif level >= 40 and level <= 44 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 9600)
	elseif level >= 45 and level <= 49 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 13700)
	elseif level >= 50 and level <= 54 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 20400)
	elseif level >= 55 and level <= 59 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 26160)
	elseif level >= 60 and level <= 64 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 35600)
	elseif level >= 65 and level <= 69 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 44600)
	elseif level >= 70 and level <= 74 then	
		role.AddRoleExp(MapID, InstanceID, RoleID, 59200)
	elseif level >= 75 and level <= 79 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 71000)
	elseif level >= 80 and level <= 84 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 90000)
	elseif level >= 85 and level <= 89 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 104000)
	elseif level >= 90 and level <= 94 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 133400)
	elseif level >= 95 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 177000)
	end
end
aux.RegisterQuestEvent(30744, 1, "Qingren_OnComplete")
aux.RegisterQuestEvent(30745, 1, "Qingren_OnComplete")

--任务放弃
function Qingren_OnCancel(MapID, InstanceID, QuestID, RoleID)
	local UpdateTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_update"]))
	local CurTime = tonumber(os.date("%j"))
	if CurTime ~= UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Qingrenjie_quest"], UpdateTime)
	end
end
aux.RegisterQuestEvent(30744, 2, "Qingren_OnCancel")
aux.RegisterQuestEvent(30745, 2, "Qingren_OnCancel")

--制作晶核
function TDGT_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	
	local a = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,60003)
	if a == true then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206530)--您身上已经拥有天之咒，请到时间再来领取
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
	else 
		return 1
	end
end
aux.RegisterQuestEvent(30753, 4, "TDGT_Complete")
function TDGD_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	
	local a = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,60004)
	if a == true then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206531)--您身上已经拥有地之咒，请到时间再来领取
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
	else 
		return 1
	end
end
aux.RegisterQuestEvent(30754, 4, "TDGD_Complete")

function TDGT_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	
	unit.AddBuff(MapID, InstanceID, RoleID, 6000301, RoleID)
end
aux.RegisterQuestEvent(30753, 1, "TDGT_OnComplete")
function TDGD_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	
	unit.AddBuff(MapID, InstanceID, RoleID, 6000401, RoleID)
end
aux.RegisterQuestEvent(30754, 1, "TDGD_OnComplete")

--活动广播
function chunjie_OnTimerMin(actID)
	local curmin = tonumber(os.date("%M"))
	if curmin == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300040)    --春节活动已经开启，通过天地宫守门人可以进入天地宫。
		msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterActEvent(37, 4, "chunjie_OnTimerMin")

function Q30762_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level >= 30 and level <= 34 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 1880000)
	elseif level >= 35 and level <= 39 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 3580000)
	elseif level >= 40 and level <= 44 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 6060000)
	elseif level >= 45 and level <= 49 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 8080000)
	elseif level >= 50 and level <= 54 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 11060000)
	elseif level >= 55 and level <= 59 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 14060000)
	elseif level >= 60 and level <= 64 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 18060000)
	elseif level >= 65 and level <= 69 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 24060000)
	elseif level >= 70 and level <= 74 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 28060000)
	elseif level >= 75 and level <= 79 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 34060000)
	elseif level >= 80 and level <= 90 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 50000000)
	elseif level >90 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 90000000)
	end
end
aux.RegisterQuestEvent(30762, 1, "Q30762_Complete")
function QingrenMan_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)--男
	local sex = role.GetRoleSex(RoleID)
	if sex == 1 then
		return 1
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206550)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(30778, 4, "QingrenMan_OnComplete")
aux.RegisterQuestEvent(30782, 4, "QingrenMan_OnComplete")
aux.RegisterQuestEvent(30783, 4, "QingrenMan_OnComplete")

function QingrenNv_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)--女
	local sex = role.GetRoleSex(RoleID)
	if sex == 0 then
		return 1
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206550)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(30779, 4, "QingrenNv_OnComplete")
aux.RegisterQuestEvent(30780, 4, "QingrenNv_OnComplete")
aux.RegisterQuestEvent(30781, 4, "QingrenNv_OnComplete")

function Qrenjierenwu_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level >= 30 and level <= 34 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 4200)
	elseif level >= 35 and level <= 39 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 4940)
	elseif level >= 40 and level <= 44 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 9600)
	elseif level >= 45 and level <= 49 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 13700)
	elseif level >= 50 and level <= 54 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 20400)
	elseif level >= 55 and level <= 59 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 26160)
	elseif level >= 60 and level <= 64 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 35600)
	elseif level >= 65 and level <= 69 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 44600)
	elseif level >= 70 and level <= 74 then	
		role.AddRoleExp(MapID, InstanceID, RoleID, 59200)
	elseif level >= 75 and level <= 79 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 71000)
	elseif level >= 80 and level <= 84 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 90000)
	elseif level >= 85 and level <= 89 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 104000)
	elseif level >= 90 and level <= 94 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 133400)
	elseif level >= 95 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 177000)
	end
end

aux.RegisterQuestEvent(30778, 1, "Qrenjierenwu_Complete")
aux.RegisterQuestEvent(30779, 1, "Qrenjierenwu_Complete")

function Q30776_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local sex = role.GetRoleSex(RoleID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4800107, 1, -1, 8, 420)
	elseif sex == 0 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4800108, 1, -1, 8, 420)
	end
end

aux.RegisterQuestEvent(30776, 1, "Q30776_Complete")
function Q30776_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local FreeSize = role.GetBagFreeSize(RoleID)
	if (FreeSize < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	else	
		return 1
	end
end
aux.RegisterQuestEvent(30776, 5, "Q30776_OnCheckComplete")

