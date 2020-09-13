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


function c3040088_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10068)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040088, 2, "c3040088_OnEnterCombat")






function c3040088_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	

	map.SetMapScriptData(MapID, InstanceID, 1, 7, 1)
	
	local boss_04 = map.GetMapScriptData(MapID,InstanceID,1,10)

	local a = map.GetMapScriptData(MapID,InstanceID,1,8)
	if a==0 then

		cre.MonsterSay(MapID, InstanceID, TargetID, 10073)
		cre.MonsterSay(MapID, InstanceID, boss_04, 10072)
		--我的妮特丽，你们谁杀死了他
		
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100090)
		--3040088死亡，3040089发怒
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		unit.AddBuff(MapID,InstanceID,boss_04,5057801,boss_04)
	else
		cre.MonsterSay(MapID, InstanceID, boss_04, 10075)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100089)
		--世界清净了
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		local bchest=map.GetMapScriptData(MapID,InstanceID,1,11)
		if bchest == 0 then
			map.MapCreateCreature(MapID, InstanceID, 6010032, 225, 10325, 816)
			
			map.MapCreateCreature(MapID, InstanceID, 5010236, 215, 10325, 806)
			
			map.SetMapScriptData(MapID, InstanceID, 1, 11, 1)
		end

	end


end

aux.RegisterCreatureEvent(3040088, 4, "c3040088_OnDie")
