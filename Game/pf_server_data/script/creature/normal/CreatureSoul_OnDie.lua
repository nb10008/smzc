--怪物死亡，增加神魂魔魂
function CreatureLingMu_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    local a,b = role.GetRoleSoul(MapID,InstanceID,RoleID)
	if TargetTypeID <= 1100181 then
	    if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
			role.SetRoleSoul(MapID,InstanceID,RoleID,a+2*shengwangjiabei,b)
		else
		    role.SetRoleSoul(MapID,InstanceID,RoleID,a+2,b)
		end
	else
	    if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
			role.SetRoleSoul(MapID,InstanceID,RoleID,a,b+2*shengwangjiabei)
		else
		    role.SetRoleSoul(MapID,InstanceID,RoleID,a,b+2)
		end
	end
end

aux.RegisterCreatureEvent(1100168, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100169, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100170, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100171, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100172, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100173, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100174, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100175, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100176, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100177, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100178, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100179, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100180, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100181, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100182, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100183, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100184, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100185, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100186, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100187, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100188, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100189, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100190, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100191, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100192, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100193, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100194, 4, "CreatureLingMu_OnDie")
aux.RegisterCreatureEvent(1100195, 4, "CreatureLingMu_OnDie")
