--滚雪球调查地物
function xueqiu_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local z = 0
	local y = 0
	local x = role.GetRoleItemNum(RoleID, 4800027)
	local FreeSize = role.GetBagFreeSize(RoleID)
	if FreeSize >= 1 then 
		if x > 0 then

			unit.AddBuff(MapID, InstanceID, RoleID, 6936301, RoleID)
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4800027, 1, 402)
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 206333)--推雪球效果只能持续30分钟，只有找到更多的雪球才能增加效果。堆雪球叠加到10次后即可获得特别奖励。
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		else 
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 206334)--身上没有雪球，调查失败
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		end
			y,z = role.GetBuffLayer(RoleID, 6936301)
		if y > 9 then
			local a = math.random(1,100)
			if a <= 50 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800022, 1, 0, 0, 402)--雪人之心
				unit.CancelBuff(MapID, InstanceID, RoleID, 6936301) 
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 4400048, 1, 0, 0, 402)--水晶球
				unit.CancelBuff(MapID, InstanceID, RoleID, 6936301) 
			end
			
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet, bIgnore
	end
end
aux.RegisterCreatureEvent(6030002, 6, "xueqiu_OnInvest")

