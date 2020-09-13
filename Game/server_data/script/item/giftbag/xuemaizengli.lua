--ÑªÂöÔùÀñ
function I4820082_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 2) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4820082_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820080, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040, 15, -1, 8, 420)

end

aux.RegisterItemEvent(4820082, 0, "I4820082_CanUse")
aux.RegisterItemEvent(4820082, 1, "I4820082_Use")
