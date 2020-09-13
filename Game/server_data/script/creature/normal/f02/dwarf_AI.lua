---mapscriptdata[1] 记录进副本后是否已刷出怪物
----[2]记录3040008是否被杀死
----[3]记录3040009是否被杀死
----[4]记录是否有过包围。
----[5]记录是否有埋伏。
----[6]记录3040010是否被杀死

function c3040007_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 10002)
	local TargetID2 = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID2, 1, 30, 20)
end
aux.RegisterCreatureEvent(3040007, 2, "c3040007_OnEnterCombat")



function c3040006_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 10012)
	local TargetID2 = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID2, 1, 30, 20)
end
aux.RegisterCreatureEvent(3040006, 2, "c3040006_OnEnterCombat")



function c3040005_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 10005)
	local TargetID2 = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID2, 1, 30, 20)
end
aux.RegisterCreatureEvent(3040005, 2, "c3040005_OnEnterCombat")



function dwarf_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	----------------50%几率刷出灵魂。
	local temp = math.random(1,100)
	local x,y,z=unit.GetPosition(MapID, InstanceID, TargetID)
	if temp<40 then
		-------------------刷处灵魂喊话
		cre.MonsterSay(MapID, InstanceID, TargetID, 10001)
		if TargetTypeID==3040007 then
			map.MapCreateCreature(MapID, InstanceID, 3040031, x, y, z)
		elseif TargetTypeID==3040006 then
			map.MapCreateCreature(MapID, InstanceID, 3040032, x, y, z)
		elseif TargetTypeID==3040005 then
			map.MapCreateCreature(MapID, InstanceID, 3040033, x, y, z)
		elseif TargetTypeID==3040004 then
			map.MapCreateCreature(MapID, InstanceID, 3040034, x, y, z)
		end
	else
		local boss_1 = map.GetMapScriptData(MapID, InstanceID, 1, 2)
		local boss_2 = map.GetMapScriptData(MapID, InstanceID, 1, 3)
		local boss_3 = map.GetMapScriptData(MapID, InstanceID, 1, 6) 
		if boss_1 ==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10013)
		elseif boss_2==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10014)
		elseif boss_3==0 then
			cre.MonsterSay(MapID, InstanceID, TargetID, 10015)
		else 
			cre.MonsterSay(MapID, InstanceID, TargetID, 10016)
		end	
	end
end

aux.RegisterCreatureEvent(3040004, 4, "dwarf_OnDie")
aux.RegisterCreatureEvent(3040005, 4, "dwarf_OnDie")
aux.RegisterCreatureEvent(3040006, 4, "dwarf_OnDie")
aux.RegisterCreatureEvent(3040007, 4, "dwarf_OnDie")