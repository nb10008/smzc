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


function c3040103_OnEnterCombat(MapID, InstanceID, CreatureID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100093)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10076)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040103, 2, "c3040103_OnEnterCombat")






function c3040103_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10064)
	

	map.SetMapScriptData(MapID, InstanceID, 1, 14, 1)
	
	
	

	map.MapCreateCreature(MapID, InstanceID, 3040108, 209, 14883, 389)
	map.MapCreateCreature(MapID, InstanceID, 3040111, 209, 14883, 380)
	map.MapCreateCreature(MapID, InstanceID, 3040111, 209, 14883, 398)

	map.MapCreateCreature(MapID, InstanceID, 3040109, 209, 14883, 445)
	map.MapCreateCreature(MapID, InstanceID, 3040110, 209, 14883, 436)
	map.MapCreateCreature(MapID, InstanceID, 3040110, 209, 14883, 454)


end

aux.RegisterCreatureEvent(3040103, 4, "c3040103_OnDie")
