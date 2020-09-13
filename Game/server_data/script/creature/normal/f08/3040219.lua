---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死

function c3040219_OnEnterCombat(MapID, InstanceID, CreatureID)

	--cre.MonsterSay(MapID, InstanceID, CreatureID, 10027)
	--天界禁地岂是你们这些人类能进入的
	

end

aux.RegisterCreatureEvent(3040219, 2, "c3040219_OnEnterCombat")

function c3040219_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	---啊，我受伤了。
	
	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	
	map.MapCreateCreature(MapID, InstanceID, 3040222, x, y, z)


end
aux.RegisterCreatureEvent(3040219, 4, "c3040219_OnDie")
