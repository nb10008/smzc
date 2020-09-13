----scriptdata[1]记录3040018是否死亡
----scriptdata[2]记录3040019是否死亡
----scriptdata[3]记录是否刷过4个幻象
----scriptdata[4]记录3040020是否死亡，死亡几次
----scriptdata[5]记录3040021是否死亡，死亡几次
----scriptdata[6]记录3040024是否死亡
----scriptdata[7]记录3040025是否死亡
----scriptdata[8]标记宝箱
----scriptdata[9]标记进副本是否刷怪

function c3040022_OnEnterCombat(MapID, InstanceID, CreatureID)


	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040022, 2, "c3040022_OnEnterCombat")

function c3040022_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10035)

	local baoxiang = map.GetMapScriptData(MapID, InstanceID, 1,8)
	if baoxiang ==0 then

		map.MapCreateCreature(MapID, InstanceID, 6010045, 1073, 1508, 245)
		map.MapCreateCreature(MapID, InstanceID, 5010234, 1083, 1508, 255)

		--map.MapCreateCreature(MapID, InstanceID, 6010030, 1073, 1508, 245)
		--map.MapCreateCreature(MapID, InstanceID, 6010030, 696, 2621, 953)
		--map.MapCreateCreature(MapID, InstanceID, 6010030, 676, 2621, 943)

		map.SetMapScriptData(MapID, InstanceID, 1,8,1)
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100050)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	if role.GetTrialState(RoleID, 4012) == 1 then
		role.SetTrialCompleteNum(RoleID, 4012, role.GetTrialCompleteNum(RoleID,4012) + 1)
    end
	if role.GetTrialState(RoleID, 4012) == 1 and role.GetTrialCompleteNum(RoleID,4012) >= Trail_maxnum[4012] then
		role.TrialComplete(RoleID,4012)
	end
end

aux.RegisterCreatureEvent(3040022, 4, "c3040022_OnDie")
