--灵魂之心5
function I4800151_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800151_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local linghun_table = {}
	linghun_table[1] = 3100095
	linghun_table[2] = 3100105
	linghun_table[3] = 3100115
	linghun_table[4] = 3100125
	linghun_table[5] = 3100135
	linghun_table[6] = 3100145
	linghun_table[7] = 3100155
	linghun_table[8] = 3100165
	local number = math.random(1,8)
	role.AddRoleItem(MapID, InstanceID, TargetID, linghun_table[number], 1, -1, 8, 420)
end

aux.RegisterItemEvent(4800151, 1, "I4800151_GiftBag")
aux.RegisterItemEvent(4800151, 0, "I4800151_CanUseGiftBag")

--灵魂之心6
function I4800152_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800152_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local linghun_table = {}
	linghun_table[1] = 3100096
	linghun_table[2] = 3100106
	linghun_table[3] = 3100116
	linghun_table[4] = 3100126
	linghun_table[5] = 3100136
	linghun_table[6] = 3100146
	linghun_table[7] = 3100156
	linghun_table[8] = 3100166
	local number = math.random(1,8)
	role.AddRoleItem(MapID, InstanceID, TargetID, linghun_table[number], 1, -1, 8, 420)
end

aux.RegisterItemEvent(4800152, 1, "I4800152_GiftBag")
aux.RegisterItemEvent(4800152, 0, "I4800152_CanUseGiftBag")

--灵魂之心7
function I4800153_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800153_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local linghun_table = {}
	linghun_table[1] = 3100097
	linghun_table[2] = 3100107
	linghun_table[3] = 3100117
	linghun_table[4] = 3100127
	linghun_table[5] = 3100137
	linghun_table[6] = 3100147
	linghun_table[7] = 3100157
	linghun_table[8] = 3100167
	local number = math.random(1,8)
	role.AddRoleItem(MapID, InstanceID, TargetID, linghun_table[number], 1, -1, 8, 420)
end

aux.RegisterItemEvent(4800153, 1, "I4800153_GiftBag")
aux.RegisterItemEvent(4800153, 0, "I4800153_CanUseGiftBag")

--灵魂之心8
function I4800154_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800154_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local linghun_table = {}
	linghun_table[1] = 3100098
	linghun_table[2] = 3100108
	linghun_table[3] = 3100118
	linghun_table[4] = 3100128
	linghun_table[5] = 3100138
	linghun_table[6] = 3100148
	linghun_table[7] = 3100158
	linghun_table[8] = 3100168
	local number = math.random(1,8)
	role.AddRoleItem(MapID, InstanceID, TargetID, linghun_table[number], 1, -1, 8, 420)
end

aux.RegisterItemEvent(4800154, 1, "I4800154_GiftBag")
aux.RegisterItemEvent(4800154, 0, "I4800154_CanUseGiftBag")

--灵魂之心9
function I4800155_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800155_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local linghun_table = {}
	linghun_table[1] = 3100099
	linghun_table[2] = 3100109
	linghun_table[3] = 3100119
	linghun_table[4] = 3100129
	linghun_table[5] = 3100139
	linghun_table[6] = 3100149
	linghun_table[7] = 3100159
	linghun_table[8] = 3100169
	local number = math.random(1,8)
	role.AddRoleItem(MapID, InstanceID, TargetID, linghun_table[number], 1, -1, 8, 420)
end

aux.RegisterItemEvent(4800155, 1, "I4800155_GiftBag")
aux.RegisterItemEvent(4800155, 0, "I4800155_CanUseGiftBag")

--灵魂之心10
function I4800156_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800156_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local linghun_table = {}
	linghun_table[1] = 3100100
	linghun_table[2] = 3100110
	linghun_table[3] = 3100120
	linghun_table[4] = 3100130
	linghun_table[5] = 3100140
	linghun_table[6] = 3100150
	linghun_table[7] = 3100160
	linghun_table[8] = 3100170
	local number = math.random(1,8)
	role.AddRoleItem(MapID, InstanceID, TargetID, linghun_table[number], 1, -1, 8, 420)
end

aux.RegisterItemEvent(4800156, 1, "I4800156_GiftBag")
aux.RegisterItemEvent(4800156, 0, "I4800156_CanUseGiftBag")











