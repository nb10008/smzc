--国庆祝福礼包
function I4820067_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4820067_Use(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {8220001,1,5,550}
	tbl[2] = {8220201,1,5,450}
	tbl[3] = {4820066,1,-1,550}
	tbl[4] = {4820062,1,-1,550}
	tbl[5] = {3070056,2,-1,1300}
	tbl[6] = {3070060,2,-1,1300}
	tbl[7] = {4400054,1,-1,1300}
	tbl[8] = {4820040,1,-1,1300}
	tbl[9] = {4530112,1,-1,1400}
	tbl[10] = {5600001,1,-1,1300}

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

aux.RegisterItemEvent(4820067, 0, "I4820067_CanUse")
aux.RegisterItemEvent(4820067, 1, "I4820067_Use")


--国庆英雄礼包
function I4820068_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4820068_Use(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {8220001,1,5,1880}
	tbl[2] = {8220201,1,5,2205}
	tbl[3] = {5600004,1,-1,600}
	tbl[4] = {4820062,1,-1,500}
	tbl[5] = {4500002,3,-1,600}
	tbl[6] = {4530107,3,-1,600}
	tbl[7] = {4530108,3,-1,500}
	tbl[8] = {4211403,1,-1,5}
	tbl[9] = {4221004,1,-1,5}
	tbl[10] = {4211103,1,-1,5}
	tbl[11] = {3020082,1,-1,300}
	tbl[12] = {4999004,1,-1,300}
	tbl[13] = {3060058,1,-1,200}
	tbl[14] = {3060059,1,-1,300}
	tbl[15] = {3060060,1,-1,300}
	tbl[16] = {4820039,1,-1,500}
	tbl[17] = {4820038,30,-1,600}
	tbl[18] = {3070052,3,-1,600}

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

aux.RegisterItemEvent(4820068, 0, "I4820068_CanUse")
aux.RegisterItemEvent(4820068, 1, "I4820068_Use")


--十尊锦盒

function I4820071_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4820071_Use(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {5600001,1,-1,1100}
	tbl[2] = {3070056,1,-1,1200}
	tbl[3] = {3070051,1,-1,1200}
	tbl[4] = {3070001,1,-1,1200}
	tbl[5] = {4800131,1,-1,1000}
	tbl[6] = {4530107,1,-1,900}
	tbl[7] = {4530110,1,-1,800}
	tbl[8] = {4211505,1,-1,700}
	tbl[9] = {4220906,1,-1,700}
	tbl[10] = {3090105,1,-1,1200}

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

aux.RegisterItemEvent(4820071, 0, "I4820071_CanUse")
aux.RegisterItemEvent(4820071, 1, "I4820071_Use")



--欢乐礼包

function I4820074_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4820074_Use(MapID, InstanceID, TypeID, TargetID)
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if(level < 1) then
		level = 1
	end
	if level >= 1 and level<40 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	elseif level >= 40 and level < 60 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 4, -1, 8, 420)
	elseif level >= 60 and level < 80 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 8, -1, 8, 420)
	elseif level >= 80 and level < 90 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 12, -1, 8, 420)
	elseif level>= 90 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 16, -1, 8, 420)
	end
end

aux.RegisterItemEvent(4820074, 0, "I4820074_CanUse")
aux.RegisterItemEvent(4820074, 1, "I4820074_Use")





--国庆赠礼[4小时]

function I4820072_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 2) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4820072_Use(MapID, InstanceID, TypeID, TargetID)
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820037, 5, -1, 8, 420)
	if level >= 40 and level < 60 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090001, 1, -1, 8, 420)
	elseif level >= 60 and level < 80 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, 2, -1, 8, 420)
	elseif level >= 80 and level < 90 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, 1, -1, 8, 420)
	elseif level>=90 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, 1, -1, 8, 420)
	end
end

aux.RegisterItemEvent(4820072, 0, "I4820072_CanUse")
aux.RegisterItemEvent(4820072, 1, "I4820072_Use")




--国庆赠礼[6小时]

function I4820073_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 2) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4820073_Use(MapID, InstanceID, TypeID, TargetID)
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 3, -1, 8, 420)
	if level >= 40 and level < 60 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400054, 10, -1, 8, 420)
	elseif level >= 60 and level < 70 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400055, 10, -1, 8, 420)
	elseif level >= 70 and level < 80 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400056, 10, -1, 8, 420)
	elseif level >= 80 and level < 90 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400057, 10, -1, 8, 420)
	elseif level>= 90 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400058, 15, -1, 8, 420)
	end
end

aux.RegisterItemEvent(4820073, 0, "I4820073_CanUse")
aux.RegisterItemEvent(4820073, 1, "I4820073_Use")



--英魂永存

function I4820066_CanUse(MapID, InstanceID, TypeID, TargetID)
    local zhufulibao = role.GetRoleItemNum(TargetID,4820067)
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (zhufulibao < 1) then
		return 32, false
	end
	return 0,false

end

function I4820066_Use(MapID, InstanceID, TypeID, TargetID)
	local zhufulibao = role.GetRoleItemNum(TargetID,4820067)
	local yinghunyongcun = role.GetRoleItemNum(TargetID,4820066)
	if(zhufulibao > 0 and yinghunyongcun >0) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4820068, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4820067, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4820066, 1, 101)
	end
end

aux.RegisterItemEvent(4820066, 0, "I4820066_CanUse")
aux.RegisterItemEvent(4820066, 1, "I4820066_Use")



--灵魂链包

function I4820076_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4820076_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020082, 5, -1, 8, 420)
end

aux.RegisterItemEvent(4820076, 0, "I4820076_CanUse")
aux.RegisterItemEvent(4820076, 1, "I4820076_Use")



--天界染料包

function I4820077_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4820077_Use(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820065, 10, -1, 8, 420)

end

aux.RegisterItemEvent(4820077, 0, "I4820077_CanUse")
aux.RegisterItemEvent(4820077, 1, "I4820077_Use")
