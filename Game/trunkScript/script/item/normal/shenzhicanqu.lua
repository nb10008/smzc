--神之残躯
function I4820040_Use(MapID, InstanceID, TypeID, TargetID)
    local k = math.random(30)
    if k <= 1 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4820039, 1, -1, 8, 420)
	end
	local n = math.random(6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820038, n+2, -1, 8, 420)

end

function I4820040_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 2) then
        bRet = 40
    end

    return bRet, bIgnore
end

aux.RegisterItemEvent(4820040, 1, "I4820040_Use")
aux.RegisterItemEvent(4820040, 0, "I4820040_CanUse")

--神之残魂
function I4820080_Use(MapID, InstanceID, TypeID, TargetID)
    local k = math.random(30)
    if k <= 1 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4820079, 1, -1, 8, 420)
	end
	local n = math.random(6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820078, n+2, -1, 8, 420)

end

function I4820080_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 2) then
        bRet = 40
    end

    return bRet, bIgnore
end

aux.RegisterItemEvent(4820080, 1, "I4820080_Use")
aux.RegisterItemEvent(4820080, 0, "I4820080_CanUse")
