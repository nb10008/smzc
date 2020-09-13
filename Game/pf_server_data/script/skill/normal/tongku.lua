

--痛苦女王技能
function S2013701_Cast(MapID, InstanceID, SkillID, RoleID, TargetID)
    if unit.IsPlayer(TargetID) then

		local maxhp = unit.GetUnitAttValue(MapID, InstanceID, TargetID, 8)
		local nowhp = unit.GetUnitAttValue(MapID, InstanceID, TargetID, 9)
		if nowhp > 0 and maxhp/nowhp >= 2 then
			unit.AddBuff(MapID, InstanceID,TargetID,3503901,RoleID)
		elseif nowhp > 0 then
		    unit.AddBuff(MapID, InstanceID,TargetID,3503801,RoleID)
		end

	end
    return 1
end

aux.RegisterSkillEvent(2013701, 5, "S2013701_Cast")

