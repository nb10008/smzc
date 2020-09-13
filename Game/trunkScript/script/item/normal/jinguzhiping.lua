--锢神之瓶

function I4820177_Use(MapID, InstanceID, TypeID, TargetID)
	--获得玩家当前的神魂和魔魂
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,TargetID)
	role.SetRoleSoul(MapID,InstanceID,TargetID,curGodSoul+100,curMonsterSoul)

end
aux.RegisterItemEvent(4820177, 1, "I4820177_Use")
aux.RegisterItemEvent(4820236, 1, "I4820177_Use")


--锢魔之瓶

function I4820178_Use(MapID, InstanceID, TypeID, TargetID)
	--获得玩家当前的神魂和魔魂
	local curGodSoul = 0
	local curMonsterSoul = 0
	curGodSoul,curMonsterSoul = role.GetRoleSoul(MapID,InstanceID,TargetID)
	role.SetRoleSoul(MapID,InstanceID,TargetID,curGodSoul,curMonsterSoul+100)

end
aux.RegisterItemEvent(4820178, 1, "I4820178_Use")
aux.RegisterItemEvent(4820237, 1, "I4820178_Use")
