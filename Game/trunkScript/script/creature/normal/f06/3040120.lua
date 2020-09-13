---[0]记录是否刷出地物
---[1]381, 15076, 235坐标的地物  517
---[2]393, 15076, 235坐标的地物  518
---[3]405, 15076, 235坐标的地物  519
---[4]381, 15076, 247坐标的地物  520

---[5]393, 15076, 247坐标的地物  521
---[6]405, 15076, 247坐标的地物  522
---[7]381, 15076, 259坐标的地物  523
---[8]393, 15076, 259坐标的地物  524
---[9]405, 15076, 259坐标的地物  525


--[10]按位记录1至9坐标的地物状态，1表示有buff 0表示无buff


--[11]记录因踩点刷出怪物的数量，上限60

--[12]记录是否解除机关


--[[


function c3040120_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	unvisable={}
	unvisable[1]=map.GetMapScriptData(MapID,InstanceID,1,1)
	unvisable[2]=map.GetMapScriptData(MapID,InstanceID,1,2)
	unvisable[3]=map.GetMapScriptData(MapID,InstanceID,1,3)
	unvisable[4]=map.GetMapScriptData(MapID,InstanceID,1,4)
	unvisable[5]=map.GetMapScriptData(MapID,InstanceID,1,5)
	unvisable[6]=map.GetMapScriptData(MapID,InstanceID,1,6)
	unvisable[7]=map.GetMapScriptData(MapID,InstanceID,1,7)
	unvisable[8]=map.GetMapScriptData(MapID,InstanceID,1,8)
	unvisable[9]=map.GetMapScriptData(MapID,InstanceID,1,9)
	

	for i=1,9 do
		map.MapDeleteCreature(MapID, InstanceID, unvisable[i])
	end
	map.SetMapScriptData(MapID,InstanceID,1,0,0)

	
end
aux.RegisterCreatureEvent(3040120, 4, "c3040120_OnDie")
]]--