--指引任务，妖精喂养
function R20026(MapID, InstanceID, skillID, RoleID)
	
	local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 20026)
	if quest == true then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 20026, 2)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["zuoqirenwu"], 1)
		return true
	end
	if MapID == 3390517288 then
	 	local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 211188)--奥运岛内不可以使用骑乘之术
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 44
	end
end

function q20026_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local R20026 = role.GetRoleScriptData(RoleID, 1, RoleDataType["zuoqirenwu"])
	if R20026 == 1 then
		return 1
	end
end

aux.RegisterSkillEvent(9000101, 1, "R20026")
aux.RegisterQuestEvent(20026, 5, "q20026_Complete")