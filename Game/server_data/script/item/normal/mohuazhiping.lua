--Ä§»¯Ö®Æ¿
function mohuazhipingCanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 3424073512 then
	    bRet = 43
	end
    return bRet, bIgnore
end
aux.RegisterItemEvent(4810006, 0, "mohuazhipingCanUse")

function mohuazhipingUse(MapID, InstanceID, ItemID, TargetID)
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local cid = map.MapCreateColCreature(MapID, InstanceID, 1099001, x, y, z, 1, "")
	unit.AddBuff(MapID, InstanceID, cid, 6100002, cid)
end
aux.RegisterItemEvent(4810006, 1, "mohuazhipingUse")
