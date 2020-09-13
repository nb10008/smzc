--完成任务
function q30920_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	local y,m,d,h = role.GetRoleCreateTime(MapID, InstanceID, RoleID)
	if y < 11 or (y == 11 and (m < 10 or ( m == 10 and d < 16))) then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4300024, 1, -1, 8, 420)
	elseif (y == 11 and (m > 10 or (m ==10 and d >= 16))) or(y == 12 and (m < 3 or(m == 3 and d <= 16))) then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4300018, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, RoleID, 4300012, 1, -1, 8, 420)
	end
end
aux.RegisterQuestEvent(30920, 1, "q30920_OnComplete")

