---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死

function c3040228_OnEnterCombat(MapID, InstanceID, CreatureID)

	--cre.MonsterSay(MapID, InstanceID, CreatureID, 10027)
	--什么人闯入了王宫，得先过我这一关

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040228, 2, "c3040228_OnEnterCombat")

function c3040228_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)

	local temp = map.GetMapScriptData(MapID,InstanceID,1,11)
	if(temp == 1) then
		local tianjieBOss = map.MapCreateCreature(MapID, InstanceID, 3040250, 349, 1151, 855)
		local mingjieBOss = map.MapCreateCreature(MapID, InstanceID, 3040251, 330, 1151, 855)
		map.SetMapScriptData(MapID,InstanceID,1,23,tianjieBOss)
		map.SetMapScriptData(MapID,InstanceID,1,24,mingjieBOss)
	end
	map.SetMapScriptData(MapID,InstanceID,1,12,1)

	--map.OpenCloseDoor(MapID, InstanceID, 880, 1)
	map.OpenCloseDoor(MapID, InstanceID, 884, 1)

end
aux.RegisterCreatureEvent(3040228, 4, "c3040228_OnDie")
