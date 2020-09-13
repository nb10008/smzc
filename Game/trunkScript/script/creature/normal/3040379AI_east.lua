--east
function s5941801_Cast(MapID, InstanceID, SkillID, OwnerID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	local cid1 = map.MapCreateColCreature(MapID, InstanceID, 3040388, x+2, y, z-2, 1)
	local cid2 = map.MapCreateColCreature(MapID, InstanceID, 3040389, x-2, y, z+2, 1)
	local tarid = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
	cre.AddEnmity(MapID, InstanceID, cid1, tarid, 2)
	cre.AddEnmity(MapID, InstanceID, cid2, tarid, 2)
	--[[
	local curRoles = {}
	curRoles = cre.GetAroundRole(MapID, InstanceID, OwnerID, 100000, 100000)
	cre.AddEnmity(MapID, InstanceID, cid1, curRoles[math.random(1,#curRoles)], 5)
	cre.AddEnmity(MapID, InstanceID, cid2, curRoles[math.random(1,#curRoles)], 5)
	--]]
end
aux.RegisterSkillEvent(5941801, 1, "s5941801_Cast")
--[[
function c3040388On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateColCreature(MapID, InstanceID, 5600085, x, y, z, 1)
end
aux.RegisterCreatureEvent(3040388, 13, "c3040388On_Disappear")

function c3040388On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateColCreature(MapID, InstanceID, 5600085, x, y, z, 1)
end
aux.RegisterCreatureEvent(3040388, 4, "c3040388On_Die")

function c3040389On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateColCreature(MapID, InstanceID, 5600087, x, y, z, 1)
end
aux.RegisterCreatureEvent(3040389, 13, "c3040389On_Disappear")

function c3040389On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateColCreature(MapID, InstanceID, 5600087, x, y, z, 1)
end
aux.RegisterCreatureEvent(3040389, 4, "c3040389On_Die")
--]]

--ÍÑÕ½¼Óbuff
function c3040379_On_Leave_Combat(MapID, InstanceID, CreatureID)
	unit.AddBuff(MapID, InstanceID, CreatureID, 6105801, CreatureID)
end
aux.RegisterCreatureEvent(3040379, 3, "c3040379_On_Leave_Combat")

function c3040379On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	SQ_boss4 = 1
end
aux.RegisterCreatureEvent(3040379, 4, "c3040379On_Die")
