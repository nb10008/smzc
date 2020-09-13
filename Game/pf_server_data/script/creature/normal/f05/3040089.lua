----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040075
----scriptdata[2]标记3040076
----scriptdata[3]标记3040077
----scriptdata[4]标记3040078
----scriptdata[5]标记3040086
----scriptdata[6]标记3040087
----scriptdata[7]标记3040088
----scriptdata[8]标记3040089
----scriptdata[9] record 3040088 ID
----scriptdata[10] record 3040089 ID
----scriptdata[11] 宝箱是否刷出


function c3040089_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10069)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040089, 2, "c3040089_OnEnterCombat")






function c3040089_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)




	map.SetMapScriptData(MapID, InstanceID, 1, 8, 1)

	if role.GetTrialState(RoleID, 6013) == 1 then
		role.SetTrialCompleteNum(RoleID, 6013, role.GetTrialCompleteNum(RoleID,6013) + 1)
    end
	if role.GetTrialState(RoleID, 6013) == 1 and role.GetTrialCompleteNum(RoleID,6013) >= Trail_maxnum[6013] then
		role.TrialComplete(RoleID,6013)
	end

	local boss_03 = map.GetMapScriptData(MapID,InstanceID,1,9)

	local a = map.GetMapScriptData(MapID,InstanceID,1,7)
	if a==0 then

		cre.MonsterSay(MapID, InstanceID, boss_03, 10070)
		cre.MonsterSay(MapID, InstanceID, TargetID, 10074)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100091)
		--3040089死亡，3040088沮丧
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		unit.AddBuff(MapID,InstanceID,boss_03,5057802,boss_03)
	else
		cre.MonsterSay(MapID, InstanceID, boss_03, 10075)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100089)
		--世界清净了
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		local bchest=map.GetMapScriptData(MapID,InstanceID,1,11)
		if bchest == 0 then
			map.MapCreateCreature(MapID, InstanceID, 6010032, 225, 10325, 816)
			map.MapCreateCreature(MapID, InstanceID, 6010037, 204, 10325, 797)

			map.MapCreateCreature(MapID, InstanceID, 5010236, 215, 10325, 806)

			map.SetMapScriptData(MapID, InstanceID, 1, 11, 1)
		end

	end

    local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
    for u,v in pairs(RoleTbl) do
		if role.GetBagFreeSize(v) > 0 and math.random(100) > 70 then
		    role.AddRoleItem(MapID, InstanceID, v, 4820022, 1, 1, 8, 420)
		end
	end
end

aux.RegisterCreatureEvent(3040089, 4, "c3040089_OnDie")
