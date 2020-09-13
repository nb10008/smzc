--天雷针地物调查
function tianleizhen_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local Rate = math.random(1, 100)
	local MsgID = msg.BeginMsgEvent()	
	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local i = role.GetRoleItemNum(RoleID, 4010243)--有无引雷针
	if i >= 1 then 
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010225, 1, -1, 8, 420)--获得天雷针
		if Rate <= 60 then
			unit.AddBuff(MapID, InstanceID, RoleID, 6935701, RoleID)--获得BUFF掉50%体力
			msg.AddMsgEvent(MsgID, 13, 0)
			msg.AddMsgEvent(MsgID, 1, 405001)--不幸被天雷击中，损失50%的体力
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		if  Rate <= 5 then
			if MapID == 3424072488 then
				map.MapCreateCreature(MapID, InstanceID, 1093006, x, y, z)--西洲幻林
			elseif MapID == 3424072232 then
				map.MapCreateCreature(MapID, InstanceID, 1093007, x, y, z)--南州迷沼
			elseif MapID == 3424075048 then
				map.MapCreateCreature(MapID, InstanceID, 1093008, x, y, z)--东洲冥山
			end 
			msg.AddMsgEvent(MsgID, 13, 0)
			msg.AddMsgEvent(MsgID, 1, 405002)--罕见的天雷之神出现了，尽快消灭这只魔物
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 405026)--没有引雷针，不能启动天雷针。
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		
	end
	
end

aux.RegisterCreatureEvent(6011023,6,"tianleizhen_OnInvest")

function tianleizhen_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local i = role.GetRoleItemNum(RoleID,4010243)
	if i > 0 then
		role.RemoveFromRole(MapID, InstanceID, RoleID,4010243, 1, 402)
	end

end
aux.RegisterQuestEvent(60000,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60001,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60002,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60003,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60004,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60005,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60006,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60007,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60008,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60009,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60010,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60011,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60012,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60013,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60014,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60015,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60016,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60017,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60018,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60019,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60020,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60021,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60022,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60023,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60024,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60025,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60026,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60027,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60028,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60029,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60030,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60031,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60032,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60033,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60034,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60035,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60036,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60037,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60038,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60039,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60040,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60041,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60042,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60043,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60044,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60045,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60046,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60047,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60048,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60049,11,"tianleizhen_OnComplete")
aux.RegisterQuestEvent(60050,11,"tianleizhen_OnComplete")

