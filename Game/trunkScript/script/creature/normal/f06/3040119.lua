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
--[18]记录老五3040119死亡与否


function c3040119_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10081)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040119, 2, "c3040119_OnEnterCombat")






function c3040119_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10082)



	local bmonsterdead = map.GetMapScriptData(MapID,InstanceID,1,18)

	map.SetMapScriptData(MapID, InstanceID, 1, 18, 1)


	if bmonsterdead == 0 then

		map.MapCreateCreature(MapID, InstanceID, 6010033, 390, 15984, 99)

		map.MapCreateCreature(MapID, InstanceID, 5010237, 389, 16056, 118)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100098)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


	end

	if role.GetTrialState(RoleID, 7011) == 1 then
		role.SetTrialCompleteNum(RoleID, 7011, role.GetTrialCompleteNum(RoleID,7011) + 1)
    end
	if role.GetTrialState(RoleID, 7011) == 1 and role.GetTrialCompleteNum(RoleID,7011) >= Trail_maxnum[7011] then
		role.TrialComplete(RoleID,7011)
	end

	local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
    for u,v in pairs(RoleTbl) do
		if role.GetBagFreeSize(v) > 0 and math.random(100) > 70 then
		    role.AddRoleItem(MapID, InstanceID, v, 4820022, 1, 1, 8, 420)
		end
	end

end

aux.RegisterCreatureEvent(3040119, 4, "c3040119_OnDie")
