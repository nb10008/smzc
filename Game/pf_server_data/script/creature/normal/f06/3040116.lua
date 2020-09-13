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

--[13]记录是否刷出老一3040103
--[14]记录老一3040103死亡与否
--[15]记录老二3040108死亡与否
--[16]记录老三3040109死亡与否
--[17]记录老四3040116死亡与否
--[18]记录老四3040119死亡与否



function c3040116_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10079)

end

aux.RegisterCreatureEvent(3040116, 2, "c3040116_OnEnterCombat")





function c3040116_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	
	local a = map.GetMapScriptData(MapID,InstanceID,1,0)
	cre.MonsterSay(MapID, InstanceID, TargetID, 10080)
	local bsmallchest = map.GetMapScriptData(MapID,InstanceID,1,17)
	
	map.SetMapScriptData(MapID,InstanceID,1,17,1)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100096)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	if bsmallchest == 0 then

		map.MapCreateCreature(MapID, InstanceID, 3040119, 390, 15984, 99)
		map.MapCreateCreature(MapID, InstanceID, 6010038, 394, 15057, 279)
	end

	if a==0 then

		map.MapCreateCreature(MapID, InstanceID, 5010030, 395, 15076, 274)
		---刷出说明NPC

		map.SetMapScriptData(MapID,InstanceID,1,0,1)

		local triggle={}
		triggle[1] = map.MapCreateCreature(MapID, InstanceID, 6010046, 381, 15076, 235)
		triggle[2] = map.MapCreateCreature(MapID, InstanceID, 6010046, 393, 15076, 235)
		triggle[3] = map.MapCreateCreature(MapID, InstanceID, 6010046, 405, 15076, 235)

		triggle[4] = map.MapCreateCreature(MapID, InstanceID, 6010046, 381, 15076, 247)
		triggle[5] = map.MapCreateCreature(MapID, InstanceID, 6010046, 393, 15076, 247)
		triggle[6] = map.MapCreateCreature(MapID, InstanceID, 6010046, 405, 15076, 247)

		triggle[7] = map.MapCreateCreature(MapID, InstanceID, 6010046, 381, 15076, 259)
		triggle[8] = map.MapCreateCreature(MapID, InstanceID, 6010046, 393, 15076, 259)
		triggle[9] = map.MapCreateCreature(MapID, InstanceID, 6010046, 405, 15076, 259)


		map.SetMapScriptData(MapID,InstanceID,1,1,triggle[1])
		map.SetMapScriptData(MapID,InstanceID,1,2,triggle[2])
		map.SetMapScriptData(MapID,InstanceID,1,3,triggle[3])
		map.SetMapScriptData(MapID,InstanceID,1,4,triggle[4])
		map.SetMapScriptData(MapID,InstanceID,1,5,triggle[5])
		map.SetMapScriptData(MapID,InstanceID,1,6,triggle[6])
		map.SetMapScriptData(MapID,InstanceID,1,7,triggle[7])
		map.SetMapScriptData(MapID,InstanceID,1,8,triggle[8])
		map.SetMapScriptData(MapID,InstanceID,1,9,triggle[9])

	
		
		
		for i=1,9 do
			local rand_state=math.random(1,2)
			if rand_state==1 then
				unit.AddBuff(MapID,InstanceID,triggle[i],5055801,triggle[i])

				local status=map.GetMapScriptData(MapID,InstanceID,1,10)
				
				map.SetMapScriptData(MapID,InstanceID,1,10,status+math.pow(2,i-1))

				
			end
		end
		
		local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
		if objtarget == 0 then
			local MsgID = msg.BeginMsgEvent()
			--msg.AddMsgEvent(MsgID, 102, 100080)    
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

			map.OpenCloseDoor(MapID, InstanceID, 441, 1)
			map.SetMapScriptData(MapID,InstanceID,1,12,1)
		end



	end


	
end
aux.RegisterCreatureEvent(3040116, 4, "c3040116_OnDie")
