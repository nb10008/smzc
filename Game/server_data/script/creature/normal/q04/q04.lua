--0为是否激活怪物

function q04_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	local lvl = role.GetRoleLevel(MapID, InstanceID,RoleID)
	if lvl <101 and lvl >= 80 then
	------80级到100级调查有作用
		local a = math.random(1,100)
		if a<=15 then
			--15%几率进入副本
			role.RoleGotoNewMap(MapID, InstanceID,RoleID,1826991911,139,0,143)
		elseif a<=40 then
			local x,y,z = unit.GetPosition(MapID, InstanceID,CreatureID)
			if lvl<90 then
				map.MapCreateCreature(MapID, InstanceID, 1091120, x,y,z)
			else
				map.MapCreateCreature(MapID, InstanceID, 1091121, x,y,z)	
			end
		elseif a<=70 then
			role.AddRoleExp(MapID, InstanceID, RoleID, math.random(100,5000))
		else
			local item2give = {4010015,4010016,4010017,4010018,4080041,4080042,4080043,4080044,4080045,4080046,4080047,4080048}
			role.AddRoleItem(MapID, InstanceID,  RoleID,item2give[math.random(1,12)], 1, 0, 8, 420) return 0
		end	
	end
end
aux.RegisterCreatureEvent(6011004, 6, "q04_OnInvest")


function q04_OnPlayerEnter(MapID, InstanceID,RoleID)
	local activated = map.GetMapScriptData(MapID,InstanceID,1,0)
	if activated == 0 then
		local a = math.random(1,100)
		local lvl = role.GetRoleLevel(MapID, InstanceID,RoleID)
		if lvl >= 80 and lvl <= 89 then
			if a < 33 then
				--------------------刷一个怪
				map.MapCreateCreature(MapID, InstanceID, 1091106, 114,0,135)
			else
				--------------------刷一些怪
				map.MapCreateCreature(MapID, InstanceID, 1091113, 125,0,117)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 120,0,112)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 130,0,112)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 120,0,122)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 130,0,122)
				
				map.MapCreateCreature(MapID, InstanceID, 1091113, 98,0,122)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 93,0,127)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 93,0,117)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 103,0,127)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 103,0,117)

				map.MapCreateCreature(MapID, InstanceID, 1091113, 113,0,149)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 118,0,147)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 108,0,147)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 118,0,157)
				map.MapCreateCreature(MapID, InstanceID, 1091113, 108,0,157)
			
			end
		else
			if a < 33 then
				--------------------刷一个怪
				map.MapCreateCreature(MapID, InstanceID, 1091107, 114,0,135)
			else
				--------------------刷一些怪
				map.MapCreateCreature(MapID, InstanceID, 1091114, 125,0,117)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 120,0,112)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 130,0,112)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 120,0,122)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 130,0,122)
				
				map.MapCreateCreature(MapID, InstanceID, 1091114, 98,0,122)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 93,0,127)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 93,0,117)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 103,0,127)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 103,0,117)

				map.MapCreateCreature(MapID, InstanceID, 1091114, 113,0,149)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 118,0,147)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 108,0,147)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 118,0,157)
				map.MapCreateCreature(MapID, InstanceID, 1091114, 108,0,157)

			end
		end

	end
	map.SetMapScriptData(MapID,InstanceID,1,0,1)

end
aux.RegisterMapEvent("q04", 2, "q04_OnPlayerEnter")


function c1091106_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10049)

	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	map.MapCreateCreature(MapID, InstanceID, 6011010,x, y, z)
	
	
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(1091106, 4, "c1091106_OnDie")

function c1091107_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10049)

	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	map.MapCreateCreature(MapID, InstanceID, 6011011,x, y, z)
	
	
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(1091107, 4, "c1091107_OnDie")
