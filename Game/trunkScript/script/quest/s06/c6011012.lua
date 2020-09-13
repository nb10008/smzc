
--足迹1
function c6011012_OnInvest(MapID, InstanceID, QuestID, RoleID, CreatureTypeID)
	
	if QuestID == 50032 then 
		local Rate = math.random(1, 100)
			if Rate <= 15 then  --15%出怪 
				local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
					--if x == 1011.26 and z == 1313.78 then  --曲寨范围
					if x > 1000 and x < 1020 and z > 1303 and z < 1323 then
					map.MapCreateCreature(MapID, InstanceID, 1080044, x, y, z)--巨型幻林野猪（1080044）
					--elseif x == 732.38 and z == 401.1 then  --遗忘古迹范围
					elseif x > 722 and x < 742 and z > 390 and z < 410 then  
					map.MapCreateCreature(MapID, InstanceID, 1080045, x, y, z)--1080045	巨型幻林蝶精
					--elseif x == 1605.84 and z == 436.3 then  --虎啸林范围
					elseif x > 1595 and x < 1615 and z > 426 and z < 446 then
					map.MapCreateCreature(MapID, InstanceID, 1080046, x, y, z)--1080046	三鬼蛮将
					--elseif x == 1470.42 and z == 1297.68  then  --淼峰山范围
					elseif x > 1460 and x < 1480 and z > 1287 and z < 1307  then
					map.MapCreateCreature(MapID, InstanceID, 1080047, x, y, z)--1080047	三鬼蛮将
					end
			
			elseif Rate > 15 and Rate <= 66 then--15%探索足迹
				role.AddRoleItem(MapID, InstanceID, RoleID, 4030001, 1, -1, 8, 420)
			
			elseif Rate > 66 and Rate <= 70 then--10%获得装备
				role.AddRoleItem(MapID, InstanceID, RoleID, 3110013, 1, -1, 8, 420)

			elseif Rate > 70 and Rate <=90 then--40%获得少量经验和金钱
				role.AddRoleExp(MapID, InstanceID, RoleID, 2800)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 1570)

			elseif Rate > 90 and Rate <=100 then--20%获得DEBUFF
				unit.AddBuff(MapID, InstanceID, RoleID, 9003501, RoleID )
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 404004)
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 404002)
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		
	end
	
end

--注册
aux.RegisterQuestEvent(50032, 9, "c6011012_OnInvest")
