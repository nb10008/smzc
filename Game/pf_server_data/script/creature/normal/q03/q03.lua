--0为是否激活怪物

function q03_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	local lvl = role.GetRoleLevel(MapID, InstanceID,RoleID)
	if lvl <80 and lvl >= 65 then
	------65级到80级调查有作用
		local a = math.random(1,100)
		if a<=15 then
			--15%几率进入副本
			role.RoleGotoNewMap(MapID, InstanceID,RoleID,1826990119,139,0,143)
		elseif a<=40 then
			local x,y,z = unit.GetPosition(MapID, InstanceID,CreatureID)
			if lvl<70 then
				map.MapCreateCreature(MapID, InstanceID, 1091118, x,y,z)
			else
				map.MapCreateCreature(MapID, InstanceID, 1091119, x,y,z)	
			end
		elseif a<=70 then
			role.AddRoleExp(MapID, InstanceID, RoleID, math.random(20000,150000))
		else
			local item2give = {4010012,4010013,4010014,4010012,4080033,4080034,4080035,4080036,4080037,4080038,4080039,4080040}
			role.AddRoleItem(MapID, InstanceID,  RoleID,item2give[math.random(1,12)], 1, 0, 8, 420) return 0
		end	
	end
end
aux.RegisterCreatureEvent(6011003, 6, "q03_OnInvest")


function q03_OnPlayerEnter(MapID, InstanceID,RoleID)
	local activated = map.GetMapScriptData(MapID,InstanceID,1,0)
	if activated == 0 then
		local a = math.random(1,100)
		local lvl = role.GetRoleLevel(MapID, InstanceID,RoleID)
		if lvl >= 65 and lvl <= 69 then
			if a < 33 then
				--------------------刷一个怪
				map.MapCreateCreature(MapID, InstanceID, 1091104, 121,0,127)
			else
				--------------------刷一些怪
				map.MapCreateCreature(MapID, InstanceID, 1091111, 104,0,113)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 110,0,118)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 110,0,108)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 100,0,118)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 100,0,108)
				
				map.MapCreateCreature(MapID, InstanceID, 1091111, 121,0,148)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 116,0,153)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 126,0,153)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 116,0,143)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 126,0,143)

				map.MapCreateCreature(MapID, InstanceID, 1091111, 131,0,106)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 136,0,111)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 136,0,101)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 126,0,111)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 126,0,101)
			
			end
		
		else
			if a < 33 then
				--------------------刷一个怪
				map.MapCreateCreature(MapID, InstanceID, 1091105, 121,0,127)
			else
				--------------------刷一些怪
				map.MapCreateCreature(MapID, InstanceID, 1091112, 104,0,113)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 110,0,118)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 110,0,108)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 100,0,118)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 100,0,108)
				
				map.MapCreateCreature(MapID, InstanceID, 1091112, 121,0,148)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 116,0,153)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 126,0,153)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 116,0,143)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 126,0,143)

				map.MapCreateCreature(MapID, InstanceID, 1091112, 131,0,106)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 136,0,111)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 136,0,101)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 126,0,111)
				map.MapCreateCreature(MapID, InstanceID, 1091112, 126,0,101)

			end
		end

	end
	map.SetMapScriptData(MapID,InstanceID,1,0,1)

end
aux.RegisterMapEvent("q03", 2, "q03_OnPlayerEnter")


function c1091105_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10049)

	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	map.MapCreateCreature(MapID, InstanceID, 6011009,x, y, z)
	
	
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(1091105, 4, "c1091105_OnDie")


