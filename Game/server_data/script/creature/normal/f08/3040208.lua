---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死

function c3040208_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10112)
	--让你们领教下魔族最强大的黑暗魔法
	map.OpenCloseDoor(MapID, InstanceID, 872, nil)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040208, 2, "c3040208_OnEnterCombat")


function c3040208_On_Leave_Combat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 872, 1)

end

aux.RegisterCreatureEvent(3040208, 3, "c3040208_On_Leave_Combat")


function s5054508_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266370126 then
		cre.MonsterSay(MapID, InstanceID,OwnerID, 10113)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100123)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		local wave = map.GetMapScriptData(MapID,InstanceID,1,2)
		if wave == 0 then
			map.MapCreateCreature(MapID, InstanceID, 3040239, 848, 63, 332)
			map.MapCreateCreature(MapID, InstanceID, 3040240, 857, 63, 335)
			map.MapCreateCreature(MapID, InstanceID, 3040239, 866, 63, 331)
			map.MapCreateCreature(MapID, InstanceID, 3040240, 867, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040239, 857, 63, 317)
			map.MapCreateCreature(MapID, InstanceID, 3040240, 849, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040245, 857, 63, 326)

			map.SetMapScriptData(MapID,InstanceID,1,2,wave+1)

		elseif wave == 1 then
			map.MapCreateCreature(MapID, InstanceID, 3040241, 848, 63, 332)
			map.MapCreateCreature(MapID, InstanceID, 3040242, 857, 63, 335)
			map.MapCreateCreature(MapID, InstanceID, 3040241, 866, 63, 331)
			map.MapCreateCreature(MapID, InstanceID, 3040242, 867, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040241, 857, 63, 317)
			map.MapCreateCreature(MapID, InstanceID, 3040242, 849, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040245, 857, 63, 326)


			map.SetMapScriptData(MapID,InstanceID,1,2,wave+1)
		else
			map.MapCreateCreature(MapID, InstanceID, 3040243, 848, 63, 332)
			map.MapCreateCreature(MapID, InstanceID, 3040244, 857, 63, 335)
			map.MapCreateCreature(MapID, InstanceID, 3040243, 866, 63, 331)
			map.MapCreateCreature(MapID, InstanceID, 3040244, 867, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040243, 857, 63, 317)
			map.MapCreateCreature(MapID, InstanceID, 3040244, 849, 63, 321)
			map.MapCreateCreature(MapID, InstanceID, 3040245, 857, 63, 326)


		end

	end
end
aux.RegisterSkillEvent(5054508, 1, "s5054508_Cast")









function c3040208_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100124)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	map.MapCreateCreature(MapID, InstanceID, 3040216, 856, 447, 677)

	map.SetMapScriptData(MapID,InstanceID,1,3,1)

	map.OpenCloseDoor(MapID, InstanceID, 875, 1)
	map.OpenCloseDoor(MapID, InstanceID, 874, 1)
	map.OpenCloseDoor(MapID, InstanceID, 872, 1)



	--map.OpenCloseDoor(MapID, InstanceID, 879, 1)


end
aux.RegisterCreatureEvent(3040208, 4, "c3040208_OnDie")
