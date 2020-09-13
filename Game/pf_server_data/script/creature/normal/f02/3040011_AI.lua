---mapscriptdata[1] 记录进副本后是否已刷出怪物
----[2]记录3040008是否被杀死
----[3]记录3040009是否被杀死
----[4]记录是否有过包围。
----[5]记录是否有埋伏。
----[6]记录3040010是否被杀死
----[7]记录宝箱是否只能被刷出一次


function c3040011_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10017)

	map.MapCreateCreature(MapID, InstanceID, 3040006, 338, 5433, 783)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 338, 5433, 788)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 338, 5433, 793)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 338, 5433, 798)



	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为100个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 150, 40)
end

aux.RegisterCreatureEvent(3040011, 2, "c3040011_OnEnterCombat")




function c3040011_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100043)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	local baoxiang = map.GetMapScriptData(MapID, InstanceID, 1,7)
	if baoxiang ==0 then

		map.MapCreateCreature(MapID, InstanceID, 6010044, 419, 5913, 794)
		map.MapCreateCreature(MapID, InstanceID, 5010233, 431, 5877, 806)
		map.SetMapScriptData(MapID, InstanceID, 1,7,1)
	end

	cre.MonsterSay(MapID, InstanceID, TargetID, 10018)

	if role.GetTrialState(RoleID, 3010) == 1 then
		role.SetTrialCompleteNum(RoleID, 3010, role.GetTrialCompleteNum(RoleID,3010) + 1)
    end
	if role.GetTrialState(RoleID, 3010) == 1 and role.GetTrialCompleteNum(RoleID,3010) >= Trail_maxnum[3010] then
		role.TrialComplete(RoleID,3010)
	end

end

aux.RegisterCreatureEvent(3040011, 4, "c3040011_OnDie")



