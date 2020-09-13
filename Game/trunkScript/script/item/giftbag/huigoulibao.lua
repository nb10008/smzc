--回购礼包1%
function I4700101_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700101_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010035, 3, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4080089, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)

end

aux.RegisterItemEvent(4700101, 1, "I4700101_GiftBag")
aux.RegisterItemEvent(4700101, 0, "I4700101_CanUseGiftBag")



--回购礼包10%
function I4700102_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700102_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4700082, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010037, 5, -1, 8, 420)

end

aux.RegisterItemEvent(4700102, 1, "I4700102_GiftBag")
aux.RegisterItemEvent(4700102, 0, "I4700102_CanUseGiftBag")



--回购礼包20%
function I4700103_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700103_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4210202, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010053, 5, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070032, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 20, -1, 8, 420)

end

aux.RegisterItemEvent(4700103, 1, "I4700103_GiftBag")
aux.RegisterItemEvent(4700103, 0, "I4700103_CanUseGiftBag")



--回购礼包30%
function I4700104_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700104_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220202, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030130, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030133, 2, -1, 8, 420)

end

aux.RegisterItemEvent(4700104, 1, "I4700104_GiftBag")
aux.RegisterItemEvent(4700104, 0, "I4700104_CanUseGiftBag")



--回购礼包40%
function I4700105_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700105_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 60, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400054, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070064, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070065, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080089, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700105, 1, "I4700105_GiftBag")
aux.RegisterItemEvent(4700105, 0, "I4700105_CanUseGiftBag")



--回购礼包50%
function I4700106_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700106_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 100, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400054, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050069, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3050070, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090018, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700106, 1, "I4700106_GiftBag")
aux.RegisterItemEvent(4700106, 0, "I4700106_CanUseGiftBag")



--回购礼包60%
function I4700107_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700107_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070077, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530103, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530112, 200, -1, 8, 420)

end

aux.RegisterItemEvent(4700107, 1, "I4700107_GiftBag")
aux.RegisterItemEvent(4700107, 0, "I4700107_CanUseGiftBag")



--回购礼包70%
function I4700108_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700108_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4220803, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211304, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530096, 5, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4400055, 200, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070007, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700108, 1, "I4700108_GiftBag")
aux.RegisterItemEvent(4700108, 0, "I4700108_CanUseGiftBag")



--回购礼包80%
function I4700109_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700109_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070069, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530112, 500, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530097, 3, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530105, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070078, 30, -1, 8, 420)

end

aux.RegisterItemEvent(4700109, 1, "I4700109_GiftBag")
aux.RegisterItemEvent(4700109, 0, "I4700109_CanUseGiftBag")



--回购礼包90%
function I4700110_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 9) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700110_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4220304, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4210103, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400055, 999, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4630201, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700110, 1, "I4700110_GiftBag")
aux.RegisterItemEvent(4700110, 0, "I4700110_CanUseGiftBag")



--回购礼包100%
function I4700111_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 7) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700111_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070073, 99, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400055, 999, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400055, 999, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530097, 60, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530105, 60, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530112, 999, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530112, 501, -1, 8, 420)

end

aux.RegisterItemEvent(4700111, 1, "I4700111_GiftBag")
aux.RegisterItemEvent(4700111, 0, "I4700111_CanUseGiftBag")
