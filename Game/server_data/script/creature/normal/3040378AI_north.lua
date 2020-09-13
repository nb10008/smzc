--ÍÑÕ½¼Óbuff
function c3040378_On_Leave_Combat(MapID, InstanceID, CreatureID)
	unit.AddBuff(MapID, InstanceID, CreatureID, 6105801, CreatureID)
end
aux.RegisterCreatureEvent(3040378, 3, "c3040378_On_Leave_Combat")

function c3040378On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	SQ_boss1 = 1
end
aux.RegisterCreatureEvent(3040378, 4, "c3040378On_Die")
