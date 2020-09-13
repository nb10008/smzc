--原神出现：20653
--需求1：调查岩浆池6010026，消除身上的灵魂结晶4010167，获得净化的结晶4010168。


--任务NPC对话函数
function x20653_OnInvest(MapID, InstanceID, questID, RoleID,CreatureTypeID)
	if CreatureTypeID == 6010026 then
	   role.RemoveFromRole(MapID, InstanceID, RoleID, 4010167, 1, 102)
	   role.AddRoleItem(MapID, InstanceID, TargetID, 4010168, 1, 0, 0, 300)
	end
	
end

--注册
aux.RegisterQuestEvent(20653, 9, "x20653_OnInvest")



