

function s5055301_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371150 then
		cre.MonsterSay(MapID, InstanceID,OwnerID, 10047)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)

		buffID=math.random(1,6)
		if buffID==1 then
			unit.AddBuff(MapID,InstanceID,TargetID,5055201,OwnerID)
			unit.AddBuff(MapID,InstanceID,TargetID,5055301,OwnerID)
		elseif buffID==2 then
			unit.AddBuff(MapID,InstanceID,TargetID,5055202,OwnerID)
			unit.AddBuff(MapID,InstanceID,TargetID,5055302,OwnerID)
		elseif buffID==3 then
			unit.AddBuff(MapID,InstanceID,TargetID,5055203,OwnerID)
			unit.AddBuff(MapID,InstanceID,TargetID,5055303,OwnerID)
		elseif buffID==4 then
			unit.AddBuff(MapID,InstanceID,TargetID,5055204,OwnerID)
			unit.AddBuff(MapID,InstanceID,TargetID,5055304,OwnerID)
		elseif buffID==5 then
			unit.AddBuff(MapID,InstanceID,TargetID,5055205,OwnerID)
			unit.AddBuff(MapID,InstanceID,TargetID,5055305,OwnerID)
		elseif buffID==6 then
			unit.AddBuff(MapID,InstanceID,TargetID,5055206,OwnerID)
			unit.AddBuff(MapID,InstanceID,TargetID,5055306,OwnerID)
		end

    		--同步仇恨给范围为40个格子，高度为20的有方怪物
   		 g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 100, 20)
	end
end
aux.RegisterSkillEvent(5055301, 1, "s5055301_Cast")





function c3040057_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10049)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040057, 2, "c3040057_OnEnterCombat")

function c3040057_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10049)

	map.MapCreateCreature(MapID, InstanceID, 6010031, 272, 5968, 251)
	map.MapCreateCreature(MapID, InstanceID, 5010235, 282, 5968, 261)

	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	if role.GetTrialState(RoleID, 5011) == 1 then
		role.SetTrialCompleteNum(RoleID, 5011, role.GetTrialCompleteNum(RoleID,5011) + 1)
    end
	if role.GetTrialState(RoleID, 5011) == 1 and role.GetTrialCompleteNum(RoleID,5011) >= Trail_maxnum[5011] then
		role.TrialComplete(RoleID,5011)
	end

end

aux.RegisterCreatureEvent(3040057, 4, "c3040057_OnDie")
