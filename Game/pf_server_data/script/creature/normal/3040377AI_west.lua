--west
function s5940301_Cast(MapID, InstanceID, SkillID, OwnerID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	local cid = map.MapCreateColCreature(MapID, InstanceID, 3040387, x, y, z, 1)
	cre.AddEnmity(MapID, InstanceID, cid, cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID), 2)
	--[[
	unit.AddBuff(MapID, InstanceID, cid, 6937701, cid)
	local curRoles = {}
	curRoles = cre.GetAroundRole(MapID, InstanceID, OwnerID, 100000, 100000)
	for k in pairs(curRoles) do
		local theClass = role.GetRoleClass(curRoles[k])
		if theClass == 12 or theClass == 13 then
			cre.AddEnmity(MapID, InstanceID, cid, curRoles[k], 2)
		elseif theClass == 10 or theClass == 11 then
			cre.AddEnmity(MapID, InstanceID, cid, curRoles[k], 2)
		elseif theClass == 8 or theClass == 9 then
			cre.AddEnmity(MapID, InstanceID, cid, curRoles[k], 2)
		else
			local curid = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
			cre.AddEnmity(MapID, InstanceID, cid, curid, 2)
		end
		break
	end
	--]]
end
aux.RegisterSkillEvent(5940301, 1, "s5940301_Cast")

--ÍÑÕ½¼Óbuff
function c3040376_On_Leave_Combat(MapID, InstanceID, CreatureID)
	unit.AddBuff(MapID, InstanceID, CreatureID, 6105801, CreatureID)
end
aux.RegisterCreatureEvent(3040376, 3, "c3040376_On_Leave_Combat")

function c3040376On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	SQ_boss2 = 1
end
aux.RegisterCreatureEvent(3040376, 4, "c3040376On_Die")
