--0为是否激活怪物

function q01_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	local lvl = role.GetRoleLevel(MapID, InstanceID,RoleID)
	if lvl <50 and lvl >= 30 then
	------30级到50级调查有作用
		local a = math.random(1,100)
		if a<=15 then
			--15%几率进入副本
			role.RoleGotoNewMap(MapID, InstanceID,RoleID,1826990631,139,0,143)
		elseif a<=40 then
			local x,y,z = unit.GetPosition(MapID, InstanceID,CreatureID)
			if lvl<40 then
				map.MapCreateCreature(MapID, InstanceID, 1091115, x,y,z)
			else
				map.MapCreateCreature(MapID, InstanceID, 1091116, x,y,z)	
			end
		elseif a<=70 then
			role.AddRoleExp(MapID, InstanceID, RoleID, math.random(100,5000))
		else
			local item2give = {4010005,4010006,4010007,4010008,4080017,4080018,4080019,4080020,4080021,4080022,4080023,4080024}
			role.AddRoleItem(MapID, InstanceID,  RoleID,item2give[math.random(1,12)], 1, 0, 8, 420) return 0
		end	
	end
end
aux.RegisterCreatureEvent(6011001, 6, "q01_OnInvest")


function q01_OnPlayerEnter(MapID, InstanceID,RoleID)
	local activated = map.GetMapScriptData(MapID,InstanceID,1,0)
	if activated == 0 then
		local a = math.random(1,100)
		local lvl = role.GetRoleLevel(MapID, InstanceID,RoleID)
		if lvl >= 30 and lvl <= 39 then
			if a < 33 then
				--------------------刷一个怪
				map.MapCreateCreature(MapID, InstanceID, 1091101, 122,0,165)
			else
				--------------------刷一些怪
				map.MapCreateCreature(MapID, InstanceID, 1091108, 120,0,181)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 125,0,183)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 124,0,176)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 131,0,180)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 122,0,180)
				
				map.MapCreateCreature(MapID, InstanceID, 1091108, 105,0,155)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 100,0,160)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 110,0,160)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 100,0,150)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 110,0,150)

				map.MapCreateCreature(MapID, InstanceID, 1091108, 137,0,168)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 132,0,173)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 142,0,173)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 132,0,163)
				map.MapCreateCreature(MapID, InstanceID, 1091108, 142,0,163)
			
			end
		else
			if a < 33 then
				--------------------刷一个怪
				map.MapCreateCreature(MapID, InstanceID, 1091102, 122,0,165)
			else
				--------------------刷一些怪
				map.MapCreateCreature(MapID, InstanceID, 1091109, 120,0,181)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 125,0,183)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 124,0,176)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 131,0,180)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 122,0,180)
				
				map.MapCreateCreature(MapID, InstanceID, 1091109, 105,0,155)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 100,0,160)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 110,0,160)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 100,0,150)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 110,0,150)

				map.MapCreateCreature(MapID, InstanceID, 1091109, 137,0,168)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 132,0,173)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 142,0,173)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 132,0,163)
				map.MapCreateCreature(MapID, InstanceID, 1091109, 142,0,163)

			end
		end

	end
	map.SetMapScriptData(MapID,InstanceID,1,0,1)

end
aux.RegisterMapEvent("q01", 2, "q01_OnPlayerEnter")


function c1091101_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10049)

	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	map.MapCreateCreature(MapID, InstanceID, 6011005,x, y, z)
	
	
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(1091101, 4, "c1091101_OnDie")

function c1091102_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10049)

	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	map.MapCreateCreature(MapID, InstanceID, 6011006,x, y, z)
	
	
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(1091102, 4, "c1091102_OnDie")
