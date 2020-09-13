--0为是否激活怪物

function q02_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	local lvl = role.GetRoleLevel(MapID, InstanceID,RoleID)
	if lvl <65 and lvl >= 50 then
	------50级到65级调查有作用
		local a = math.random(1,100)
		if a<=15 then
			--15%几率进入副本
			role.RoleGotoNewMap(MapID, InstanceID,RoleID,1826990375,139,0,143)
		elseif a<=40 then
			local x,y,z = unit.GetPosition(MapID, InstanceID,CreatureID)
			if lvl<60 then
				map.MapCreateCreature(MapID, InstanceID, 1091117, x,y,z)
			else
				map.MapCreateCreature(MapID, InstanceID, 1091118, x,y,z)	
			end
		elseif a<=70 then
			role.AddRoleExp(MapID, InstanceID, RoleID, math.random(10000,10000))
		else
			local item2give = {4010009,4010010,4010011,4010009,4080025,4080026,4080027,4080028,4080029,4080030,4080031,4080032}
			role.AddRoleItem(MapID, InstanceID,  RoleID,item2give[math.random(1,12)], 1, 0, 8, 420) return 0
		end	
	end
end
aux.RegisterCreatureEvent(6011002, 6, "q02_OnInvest")


function q02_OnPlayerEnter(MapID, InstanceID,RoleID)
	local activated = map.GetMapScriptData(MapID,InstanceID,1,0)
	if activated == 0 then
		local a = math.random(1,100)
		local lvl = role.GetRoleLevel(MapID, InstanceID,RoleID)
		if lvl >= 50 and lvl <= 59 then
			if a < 33 then
				--------------------刷一个怪
				map.MapCreateCreature(MapID, InstanceID, 1091103, 122,0,165)
			else
				--------------------刷一些怪
				map.MapCreateCreature(MapID, InstanceID, 1091110, 120,0,181)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 125,0,183)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 124,0,176)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 131,0,180)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 122,0,180)
				
				map.MapCreateCreature(MapID, InstanceID, 1091110, 105,0,155)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 100,0,160)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 114,0,154)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 100,0,150)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 110,0,150)

				map.MapCreateCreature(MapID, InstanceID, 1091110, 137,0,168)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 132,0,173)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 142,0,173)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 132,0,163)
				map.MapCreateCreature(MapID, InstanceID, 1091110, 142,0,163)
			
			end
		else
			if a < 33 then
				--------------------刷一个怪
				map.MapCreateCreature(MapID, InstanceID, 1091104, 122,0,165)
			else
				--------------------刷一些怪
				map.MapCreateCreature(MapID, InstanceID, 1091111, 120,0,181)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 125,0,183)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 124,0,176)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 131,0,180)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 122,0,180)
				
				map.MapCreateCreature(MapID, InstanceID, 1091111, 105,0,155)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 100,0,160)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 114,0,154)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 100,0,150)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 110,0,150)

				map.MapCreateCreature(MapID, InstanceID, 1091111, 137,0,168)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 132,0,173)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 142,0,173)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 132,0,163)
				map.MapCreateCreature(MapID, InstanceID, 1091111, 142,0,163)

			end
		end

	end
	map.SetMapScriptData(MapID,InstanceID,1,0,1)

end
aux.RegisterMapEvent("q02", 2, "q02_OnPlayerEnter")


function c1091103_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10049)

	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	map.MapCreateCreature(MapID, InstanceID, 6011007,x, y, z)
	
	
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(1091103, 4, "c1091103_OnDie")

function c1091104_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10049)

	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	map.MapCreateCreature(MapID, InstanceID, 6011008,x, y, z)
	
	
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(1091104, 4, "c1091104_OnDie")
