----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040075
----scriptdata[2]标记3040076
----scriptdata[3]标记3040077
----scriptdata[4]标记3040078


function c3040078_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10063)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040078, 2, "c3040078_OnEnterCombat")


function s5054503_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371406 then

		--cre.MonsterSay(MapID, InstanceID, OwnerID, 10038)

		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		map.MapCreateCreature(MapID, InstanceID, 3040090, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040091, x-5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040092, x+5, y, z-5)
		
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--同步仇恨给范围为30个格子，高度为20的有方怪物
   		g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 30, 20)
	end
end
aux.RegisterSkillEvent(5054503, 1, "s5054503_Cast")



function c3040078_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10064)
	

	map.SetMapScriptData(MapID, InstanceID, 1, 4, 1)
	
	local bossdeath_1=map.GetMapScriptData(MapID,InstanceID,1,1)
	local bossdeath_2=map.GetMapScriptData(MapID,InstanceID,1,2)
	local bossdeath_3=map.GetMapScriptData(MapID,InstanceID,1,3)
	
	if bossdeath_1 == 1 and bossdeath_2 == 1 and bossdeath_3 == 1 then
		map.OpenCloseDoor(MapID, InstanceID, 4588, 1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100086)
		--门打开
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100085)
		--门打开
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	
	end
	
	


end

aux.RegisterCreatureEvent(3040078, 4, "c3040078_OnDie")
