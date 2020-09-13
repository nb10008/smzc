





function xiaoguai80_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	local CreatureNum = map.GetMapScriptData(MapID, InstanceID, 1, 0) + 1
	map.SetMapScriptData(MapID, InstanceID, 1, 0, CreatureNum)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 202003)--”已击杀魔窟喽啰&lt;p1&gt;名，击杀30个后魔窟首领即会现身！“
	msg.AddMsgEvent(MsgID, 9, CreatureNum)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	if CreatureNum == 30 then
		local a = map.GetMapScriptData(MapID, InstanceID, 1, 2)
		map.MapCreateCreature(MapID, InstanceID, 2053001 + (a-30)/5, 42, 288, 128)
	end

	
	
	local freshboss = map.GetMapScriptData(MapID, InstanceID, 1, 10)

	if freshboss == 0 then
		local poss=math.random(1,100)
		
		if poss > 97 then

			map.SetMapScriptData(MapID, InstanceID, 1, 10, 1)
			local boss = math.random(1,21)
			if boss == 1 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss , x, y, z)
			elseif boss == 2 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 3 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 4 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 5 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 6 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 7 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 9 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 10 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 12 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 13 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 14 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 15 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 16 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 17 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 18 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 19 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 20 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			elseif boss == 21 then
				local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
				map.MapCreateCreature(MapID, InstanceID, 2054133+boss, x, y, z)
			end
		end
	end

	
	


end

aux.RegisterCreatureEvent(2051041, 4, "xiaoguai80_OnDie")
aux.RegisterCreatureEvent(2051042, 4, "xiaoguai80_OnDie")
aux.RegisterCreatureEvent(2051043, 4, "xiaoguai80_OnDie")
aux.RegisterCreatureEvent(2051044, 4, "xiaoguai80_OnDie")

