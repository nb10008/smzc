--情人节卡片能否使用
function qingrenjiekapian_CanUse(MapID, InstanceID, TypeID, RoleID)
	local qingrenjie_quest = {}
	qingrenjie_quest[1] = {questid = 30906}
	qingrenjie_quest[2] = {questid = 30907}
	qingrenjie_quest[3] = {questid = 30908}
	qingrenjie_quest[4] = {questid = 30909}
	qingrenjie_quest[5] = {questid = 30910}
	qingrenjie_quest[6] = {questid = 30911}
	qingrenjie_quest[7] = {questid = 30913}
	qingrenjie_quest[8] = {questid = 30914}
	qingrenjie_quest[9] = {questid = 30915}
	qingrenjie_quest[10] = {questid = 30916}


	local a = role.GetRoleQuestNum(RoleID)
	local levle = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local QuestID1 = msg.BeginMsgEvent()
	local bRet, bIgnore = 0, false
	if a >= 30 then
		bRet = 21
	else
		if levle < 40 then
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 100008)--等级不足，无法使用物品
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			bRet = 32
		elseif levle >= 40 then
			for i = 1,#qingrenjie_quest do
				local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, qingrenjie_quest[i].questid)
				if quest == true then
					local QuestID1 = msg.BeginMsgEvent()
					msg.AddMsgEvent(QuestID1, 13, 0)
					msg.AddMsgEvent(QuestID1, 1, 210005)--您已经接取了任务
					msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					bRet = 32
				end
			end
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4800258, 0, "qingrenjiekapian_CanUse")

-- 使用
function qingrenjiekapian_onUse(MapID, InstanceID, TypeID, RoleID)
	local qingrenjie_quest = {}
	qingrenjie_quest = {}
	qingrenjie_quest[1] = {questid = 30906}
	qingrenjie_quest[2] = {questid = 30907}
	qingrenjie_quest[3] = {questid = 30908}
	qingrenjie_quest[4] = {questid = 30909}
	qingrenjie_quest[5] = {questid = 30910}
	qingrenjie_quest[6] = {questid = 30911}
	qingrenjie_quest[7] = {questid = 30913}
	qingrenjie_quest[8] = {questid = 30914}
	qingrenjie_quest[9] = {questid = 30915}
	qingrenjie_quest[10] = {questid = 30916}
	role.AddQuest(RoleID,qingrenjie_quest[math.random(10)].questid)
end
aux.RegisterItemEvent(4800258, 1, "qingrenjiekapian_onUse")




--怪异药水使用

function guaiyiyaoshui_onUse(MapID, InstanceID, TypeID, RoleID)
	local buff_ID = {2049101,2046701,2046801,2048201,2048301,2048401,2048501,2046601}
	unit.AddBuff(MapID, InstanceID,RoleID,buff_ID[math.random(#buff_ID)],RoleID)
end
aux.RegisterItemEvent(4800259, 1, "guaiyiyaoshui_onUse")
