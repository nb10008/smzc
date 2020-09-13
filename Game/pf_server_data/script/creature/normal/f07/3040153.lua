----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记召唤术到第几级了
----scriptdata[2]标记3040141是否死亡
----scriptdata[3]标记3040143是否死亡
----scriptdata[4]标记3040150是否死亡
----scriptdata[5]标记3040151是否死亡
----scriptdata[6]记录3040152的ID
----scriptdata[7]标记3040164是否死亡
----scriptdata[8]标记3040165是否死亡
----scriptdata[9]标记3040166是否死亡
----scriptdata[10]标记是否刷过宝箱


function c3040153_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10106)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040153, 2, "c3040153_OnEnterCombat")

function c3040153_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10107)

	local baoxiang = map.GetMapScriptData(MapID, InstanceID, 1,10)
	if baoxiang ==0 then


		map.MapCreateCreature(MapID, InstanceID, 6010034, 476, 5664, 1790)
		--map.MapCreateCreature(MapID, InstanceID, 6010039, 501, 5664, 1790)

		map.SetMapScriptData(MapID, InstanceID, 1,10,1)
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100117)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


	if role.GetTrialState(RoleID, 8023) == 1 then
		role.SetTrialCompleteNum(RoleID, 8023, role.GetTrialCompleteNum(RoleID,8023) + 1)
    	end
	if role.GetTrialState(RoleID, 8023) == 1 and role.GetTrialCompleteNum(RoleID,8023) >= Trail_maxnum[8023] then
		role.TrialComplete(RoleID,8023)
	end

	local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
    for u,v in pairs(RoleTbl) do
		if role.GetBagFreeSize(v) > 0 and math.random(100) > 70 then
		    role.AddRoleItem(MapID, InstanceID, v, 4820022, 1, 1, 8, 420)
		end
	end
end

aux.RegisterCreatureEvent(3040153, 4, "c3040153_OnDie")


