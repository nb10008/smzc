--战场礼包

function I4500116_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if level >= 40 and level <= 49 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090001, 4, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090007, 4, -1, 8, 420)
	elseif level >= 50 and level <= 79 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090002, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090008, 4, -1, 8, 420)
	elseif level >= 80 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090003, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 5, -1, 8, 420)
	end
end
function I4500116_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4500116, 1, "I4500116_GiftBag")
aux.RegisterItemEvent(4500116, 0, "I4500116_CanUseGiftBag")


--接取战斗的荣耀判断

function zhandouderongyao_OnCheackComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,90007)
	if a == false then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 303007)--您身上没有“战斗的荣耀”状态，无法接取任务
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
        else
                return 1
	end
end
aux.RegisterQuestEvent(30714, 4, "zhandouderongyao_OnCheackComplete")

--接取高级官员嘉奖

function gaojiguanyuanjiajiang_OnCheackComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,90042)
	if a == false then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 303008)--您身上没有“战斗的荣耀”状态，无法接取任务
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
	end
	return 1
end
aux.RegisterQuestEvent(30715, 4, "gaojiguanyuanjiajiang_OnCheackComplete")



--接取胜利的荣耀

function shengliderongyao_OnCheackComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,90043)
	if a == false then
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 303009)--您身上没有“战斗的荣耀”状态，无法接取任务
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		return 0
	end
	return 1
end
aux.RegisterQuestEvent(30716, 4, "shengliderongyao_OnCheackComplete")















