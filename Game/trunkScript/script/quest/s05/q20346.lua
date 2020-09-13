--20346调查地物获得（4010198）变化之书,删除（4010197）王者之印
function Q20346_OnInvest(MapID, InstanceID, QuestID, RoleID, CreatureTypeID)

	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize >= 1) then
		--RemoveFromRole(地图ID， 副本ID， 玩家ID，物品TypeID， 物品数量， 记录log类型)
		local i = role.GetRoleItemNum(RoleID, 4010197)
		if i >= 1 then 
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4010197, 1, 100)--100,任务掉落
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010198, 1, 0, 0, 402)
		else
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 404003)
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		end
	elseif (FreeSize < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet, bIgnore
	end


end

--注册
aux.RegisterQuestEvent(20346, 9, "Q20346_OnInvest")
