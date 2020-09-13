

function s5039003_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371150 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10043)
		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		creatureID=math.random(1,3)
		if creatureID==1 then
			map.MapCreateCreature(MapID, InstanceID, 3040049, x+5, y, z+5)
		elseif creatureID==2 then
			map.MapCreateCreature(MapID, InstanceID, 3040050, x-5, y, z+5)
		elseif creatureID==3 then
			map.MapCreateCreature(MapID, InstanceID, 3040051, x-5, y, z-5)
		end
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--同步仇恨给范围为40个格子，高度为20的有方怪物
   		 g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 40, 20)
	end
end
aux.RegisterSkillEvent(5039003, 1, "s5039003_Cast")



function c3040048_OnEnterCombat(MapID, InstanceID, CreatureID)

	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040048, 2, "c3040048_OnEnterCombat")

function c3040048_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10044)

	map.MapCreateCreature(MapID, InstanceID, 3040056, 88, 5845, 130)
	
	
	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040048, 4, "c3040048_OnDie")