---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死

function c3040200_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10110)
	--什么人闯入了王宫，得先过我这一关

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040200, 2, "c3040200_OnEnterCombat")

function c3040200_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	cre.MonsterSay(MapID, InstanceID, TargetID, 10111)
	
	
	map.OpenCloseDoor(MapID, InstanceID, 872, 1)

	--map.OpenCloseDoor(MapID, InstanceID, 879, 1)

	
	

end
aux.RegisterCreatureEvent(3040200, 4, "c3040200_OnDie")
