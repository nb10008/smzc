--火鸡火腿
function wangzhongwang_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30685) 
	local b = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30686) 
	local c = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30687) 
	local d = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30688) 
	if a == true or b == true or c == true or d == true then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206328)--已接取消灭那些火腿任务！不能接取该任务
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
	else
	return 1
	end

end
aux.RegisterQuestEvent(30681, 4, "wangzhongwang_CheckAccept")
aux.RegisterQuestEvent(30682, 4, "wangzhongwang_CheckAccept")
aux.RegisterQuestEvent(30683, 4, "wangzhongwang_CheckAccept")
aux.RegisterQuestEvent(30684, 4, "wangzhongwang_CheckAccept")

function huoji_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local e = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30681) 
	local f = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30682) 
	local g = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30683) 
	local h = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30684) 
	if e == true or f == true or g == true or h == true then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206329)--已接取找到火鸡任务！不能接取该任务
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
	else
	return 1
	end
end
aux.RegisterQuestEvent(30685, 4, "huoji_CheckAccept")
aux.RegisterQuestEvent(30686, 4, "huoji_CheckAccept")
aux.RegisterQuestEvent(30687, 4, "huoji_CheckAccept")
aux.RegisterQuestEvent(30688, 4, "huoji_CheckAccept")