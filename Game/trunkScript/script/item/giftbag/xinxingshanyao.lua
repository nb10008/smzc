--新星闪耀包1级
function I4700088_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 7) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700088_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211502, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4220902, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700089, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700088, 1, "I4700088_GiftBag")
aux.RegisterItemEvent(4700088, 0, "I4700088_CanUseGiftBag")



--新星闪耀包10级
function I4700089_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 6) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700089_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700090, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700089, 1, "I4700089_GiftBag")
aux.RegisterItemEvent(4700089, 0, "I4700089_CanUseGiftBag")



--新星闪耀包20级
function I4700090_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 6) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700090_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 9, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 30, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700091, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700090, 1, "I4700090_GiftBag")
aux.RegisterItemEvent(4700090, 0, "I4700090_CanUseGiftBag")



--新星闪耀包30级
function I4700091_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 13) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700091_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020049, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 30, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700092, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700091, 1, "I4700091_GiftBag")
aux.RegisterItemEvent(4700091, 0, "I4700091_CanUseGiftBag")



--新星闪耀包40级
function I4700092_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 15) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700092_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 80, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090001, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 30, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700093, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700092, 1, "I4700092_GiftBag")
aux.RegisterItemEvent(4700092, 0, "I4700092_CanUseGiftBag")



--新星闪耀包50级
function I4700093_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 9) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700093_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020080, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 90, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700094, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700093, 1, "I4700093_GiftBag")
aux.RegisterItemEvent(4700093, 0, "I4700093_CanUseGiftBag")



--新星闪耀包60级
function I4700094_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 15) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700094_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4090002, 28, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020080, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030125, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020050, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 90, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211504, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4220903, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090002, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700095, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700094, 1, "I4700094_GiftBag")
aux.RegisterItemEvent(4700094, 0, "I4700094_CanUseGiftBag")




--新星闪耀包70级
function I4700095_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 9) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700095_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020080, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030125, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020050, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 90, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 20, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070080, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700096, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700095, 1, "I4700095_GiftBag")
aux.RegisterItemEvent(4700095, 0, "I4700095_CanUseGiftBag")




--新星闪耀包80级
function I4700096_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 10) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700096_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020082, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530095, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 90, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 20, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090002, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, 107, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090030, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700097, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700096, 1, "I4700096_GiftBag")
aux.RegisterItemEvent(4700096, 0, "I4700096_CanUseGiftBag")




--新星闪耀包90级
function I4700097_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 11) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700097_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, 207, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070083, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211504, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220903, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530096, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020027, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010026, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010026, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4700097, 1, "I4700097_GiftBag")
aux.RegisterItemEvent(4700097, 0, "I4700097_CanUseGiftBag")

