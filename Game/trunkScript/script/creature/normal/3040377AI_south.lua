--ÍÑÕ½È¥buff
function c3040377_On_Leave_Combat(MapID, InstanceID, CreatureID)
	if cre.IsHaveBuff(MapID, InstanceID, CreatureID, 69386) == true then
		unit.CancelBuff(MapID, InstanceID, CreatureID, 6938501)
		unit.CancelBuff(MapID, InstanceID, CreatureID, 6938601)
	end
	unit.AddBuff(MapID, InstanceID, CreatureID, 6105801, CreatureID)
end
aux.RegisterCreatureEvent(3040377, 3, "c3040377_On_Leave_Combat")

function c3040377On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	SQ_boss3 = 1
end
aux.RegisterCreatureEvent(3040377, 4, "c3040377On_Die")
