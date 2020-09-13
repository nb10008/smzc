
--足迹2
function c6011016_OnInvest(MapID, InstanceID, QuestID, RoleID, CreatureTypeID)
	
	if QuestID == 50033 then 
		local Rate = math.random(1, 100)
			if Rate <= 15 then  --15%出怪 
				local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
					--if  x == 2144.6 and z == 1826.72  then  --武岩城范围
					if  x > 2134 and x < 2154 and  z > 1816 and z < 1836 then
					map.MapCreateCreature(MapID, InstanceID, 1080054, x, y, z)--1080054	三鬼蛮将
					--elseif  x == 1819.66 and z == 2074.22 then  --武岩城郊范围
					elseif  x > 1809 and x < 1928 and z > 2064 and z < 2084 then
					map.MapCreateCreature(MapID, InstanceID, 1080053, x, y, z)--1080053	五鬼蛮将
					--elseif  x == 2142 and z == 1115.86  then  --龙源森范围
					elseif  x > 2132 and x < 2152 and z > 1105 and z < 1125 then  
					map.MapCreateCreature(MapID, InstanceID, 1080049, x, y, z)--1080049	五鬼蛮将
					--elseif  x == 2645.04 and z == 568.66 then  --天峰场范围
					elseif  x > 2635 and x < 2655 and z > 558 and z < 578 then
					map.MapCreateCreature(MapID, InstanceID, 1080048, x, y, z)--1080048	巨型魔琏花
					end
			elseif Rate > 15 and Rate <= 66 then--15%探浅足迹
				role.AddRoleItem(MapID, InstanceID, RoleID, 4030002, 1, -1, 8, 420)
			elseif Rate > 66 and Rate <= 70 then--10%获得装备
				role.AddRoleItem(MapID, InstanceID, RoleID, 3110013, 1, -1, 8, 420)
			elseif Rate > 70 and Rate <=90 then--40%获得少量经验和金钱
				role.AddRoleExp(MapID, InstanceID, RoleID, 3250)
				role.AddRoleSilver(MapID, InstanceID, RoleID, 1640)
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
aux.RegisterQuestEvent(50033, 9, "c6011016_OnInvest")
