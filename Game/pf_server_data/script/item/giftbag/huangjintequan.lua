--恶魔黄金特权包1级
function I4700098_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700098_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5610012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221206, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070080, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070086, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700099, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700098, 1, "I4700098_GiftBag")
aux.RegisterItemEvent(4700098, 0, "I4700098_CanUseGiftBag")



--恶魔黄金特权包20级
function I4700099_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 9) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700099_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070085, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070087, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020081, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090002, 28, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020050, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
end

aux.RegisterItemEvent(4700099, 1, "I4700099_GiftBag")
aux.RegisterItemEvent(4700099, 0, "I4700099_CanUseGiftBag")
