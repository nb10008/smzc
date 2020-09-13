


function Trail_BOSS_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    role.NotifyFBBS(RoleID,6,TargetTypeID)

    local Trail = {}
	Trail[1081001] = 2008
	Trail[1081002] = 2009
	Trail[1081020] = 2010
	Trail[1081003] = 3008
	Trail[1081004] = 3009
	Trail[1081021] = 3011
	Trail[1081005] = 4009
	Trail[1081006] = 4010
	Trail[1081022] = 4011
	Trail[3040022] = 4012
	Trail[1081007] = 5008
	Trail[1081008] = 5009
	Trail[1081023] = 5010
	Trail[1081009] = 6010
	Trail[1081010] = 6011
	Trail[1081024] = 6012
	Trail[1081011] = 7007
	Trail[1081012] = 7008
	Trail[1081013] = 7009
	Trail[1081025] = 7010
	Trail[1081014] = 8019
	Trail[1081015] = 8020
	Trail[1081016] = 8021
	Trail[1081026] = 8022
	Trail[1081017] = 9029
	Trail[1081018] = 9030
	Trail[1081019] = 9031
	Trail[1081027] = 9032
	Trail[1081028] = 9033

	if role.GetTrialState(RoleID, Trail[TargetTypeID]) == 1 then
		role.SetTrialCompleteNum(RoleID, Trail[TargetTypeID], role.GetTrialCompleteNum(RoleID,Trail[TargetTypeID]) + 1)
    end
	if role.GetTrialState(RoleID, Trail[TargetTypeID]) == 1 and role.GetTrialCompleteNum(RoleID,Trail[TargetTypeID]) >= Trail_maxnum[Trail[TargetTypeID]] then
		role.TrialComplete(RoleID, Trail[TargetTypeID])
	end


end

aux.RegisterCreatureEvent(1081001,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081002,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081020,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081003,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081004,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081021,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081005,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081006,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081022,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081007,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081008,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081023,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081009,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081010,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081024,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081011,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081012,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081013,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081025,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081014,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081015,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081016,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081026,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081017,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081018,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081019,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081027,4,"Trail_BOSS_OnDie")
aux.RegisterCreatureEvent(1081028,4,"Trail_BOSS_OnDie")
