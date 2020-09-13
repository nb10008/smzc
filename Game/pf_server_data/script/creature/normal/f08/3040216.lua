---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死

---mapscriptdata[4] 记录boss2刷出第几波小怪

function c3040216_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10114)
	--魔族只要有我在，就屹立不倒。
	map.OpenCloseDoor(MapID, InstanceID, 874, nil)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100125)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040216, 2, "c3040216_OnEnterCombat")


function s5934701_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266370126 then
		cre.MonsterSay(MapID, InstanceID,OwnerID, 10115)

		local wave = map.GetMapScriptData(MapID,InstanceID,1,4)

		if wave < 6 then
			local x,y,z = unit.GetPosition(MapID, InstanceID,OwnerID)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100126)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

			map.MapCreateCreature(MapID, InstanceID, 3040246, x+5, y, z+5)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x-5, y, z+5)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x+5, y, z-5)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x-5, y, z-5)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x+7, y, z)
			map.MapCreateCreature(MapID, InstanceID, 3040246,  x, y, z+7)

			map.SetMapScriptData(MapID,InstanceID,1,4,wave+1)

		end

	end
end
aux.RegisterSkillEvent(5934701, 1, "s5934701_Cast")









function c3040216_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10116)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100127)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	map.MapCreateCreature(MapID, InstanceID, 5011013, 856, 447, 677)
	map.OpenCloseDoor(MapID, InstanceID, 874, 1)




end
aux.RegisterCreatureEvent(3040216, 4, "c3040216_OnDie")



function c3040216_On_Leave_Combat(MapID, InstanceID, CreatureID)
	unit.CancelBuff(MapID, InstanceID, CreatureID, 5934701)
	map.OpenCloseDoor(MapID, InstanceID, 874, 1)
end

aux.RegisterCreatureEvent(3040216, 3, "c3040216_On_Leave_Combat")
