--»êÊ¯Ä§Ïä
function I4530179_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end
    return bRet, bIgnore
end

function I4530179_Use(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(100)
	if r < 20 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 1, -1, 8, 420)
	elseif r < 55 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020118, 1, -1, 8, 420)
	elseif r < 80 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020121, 1, -1, 8, 420)
	elseif r < 95 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020124, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020125, 1, -1, 8, 420)
	end
end
aux.RegisterItemEvent(4530179, 0, "I4530179_CanUse")
aux.RegisterItemEvent(4530179, 1, "I4530179_Use")
