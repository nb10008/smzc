--CJ至尊豪华礼包
function I4700071_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700071_Use(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4700072, 1, -1, 8, 420)

end

--CJ至尊豪华包[1级]
function I4700072_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 9) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700072_Use(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070007, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 20,-1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4211502, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4220902, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4300002, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 20, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700073, 1, -1, 8, 420)
end

--CJ至尊豪华包[10级]
function I4700073_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 12) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700073_Use(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 3, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 50,-1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030125, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020049, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020078, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700074, 1, -1, 8, 420)
end

--CJ至尊豪华包[20级]
function I4700074_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 12) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700074_Use(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 3, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 50,-1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030125, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020049, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020078, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700075, 1, -1, 8, 420)
end

--CJ至尊豪华包[30级]
function I4700075_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 13) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700075_Use(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 4, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 50,-1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030125, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020049, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020078, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 20, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 30, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700076, 1, -1, 8, 420)
end

--CJ至尊豪华包[40级]
function I4700076_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 18) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700076_Use(MapID, InstanceID, TypeID, TargetID)
    local roleLevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
    local class = role.GetRoleClass(TargetID)

    role.AddRoleItem(MapID, InstanceID, TargetID, 3030125, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020049, 15, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 30, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 20, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4211502, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4220902, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020078, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700077, 1, -1, 8, 420)

    if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8017004, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8027004, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101204, 1, 4, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8037004, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8047004, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091804, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101204, 1, 4, 8, 420)
    end
end

--CJ至尊豪华包[50级]
function I4700077_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 9) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700077_Use(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 100,-1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 30, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 50, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020078, 4, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700078, 1, -1, 8, 420)
end

--CJ至尊豪华包[60级]
function I4700078_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 28)	then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700078_Use(MapID, InstanceID, TypeID, TargetID)
    local class = role.GetRoleClass(TargetID)

    role.AddRoleItem(MapID, InstanceID, TargetID, 4211504, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4220903, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 50, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 20, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 200, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020081, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030125, 6, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700079, 1, -1, 8, 420)

    if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8160006, 1, 4, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8170007, 1, 4, 8, 420)
    end

    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8017007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8027007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101207, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8017007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8027007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101211, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8037007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8047007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101215, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8037007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8047007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101219, 1, 4, 8, 420)
    end
end

--CJ至尊豪华包[80级]
function I4700079_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 16)	then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700079_Use(MapID, InstanceID, TypeID, TargetID)
    local class = role.GetRoleClass(TargetID)

    role.AddRoleItem(MapID, InstanceID, TargetID, 4540031, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020082, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030128, 8, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4211504, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4220903, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 50, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700080, 1, -1, 8, 420)

    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8017008, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8027008, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101208, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8017008, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8027008, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101212, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8037008, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8047008, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091808, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101216, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8037008, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8047008, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091812, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101220, 1, 4, 8, 420)
    end
end

--CJ至尊豪华包[90级]
function I4700080_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 9)	then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4700080_Use(MapID, InstanceID, TypeID, TargetID)
    local class = role.GetRoleClass(TargetID)

    role.AddRoleItem(MapID, InstanceID, TargetID, 4221208, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 5610013, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4300003, 1, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020088, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030034, 2, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 50, -1, 8, 420)

    if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8160010, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8013010, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8023010, 1, 5, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8170011, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8033010, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8043010, 1, 5, 8, 420)
    end
end


--注册事件
aux.RegisterItemEvent(4700071, 0, "I4700071_CanUse")
aux.RegisterItemEvent(4700071, 1, "I4700071_Use")
aux.RegisterItemEvent(4700072, 0, "I4700072_CanUse")
aux.RegisterItemEvent(4700072, 1, "I4700072_Use")
aux.RegisterItemEvent(4700073, 0, "I4700073_CanUse")
aux.RegisterItemEvent(4700073, 1, "I4700073_Use")
aux.RegisterItemEvent(4700074, 0, "I4700074_CanUse")
aux.RegisterItemEvent(4700074, 1, "I4700074_Use")
aux.RegisterItemEvent(4700075, 0, "I4700075_CanUse")
aux.RegisterItemEvent(4700075, 1, "I4700075_Use")
aux.RegisterItemEvent(4700076, 0, "I4700076_CanUse")
aux.RegisterItemEvent(4700076, 1, "I4700076_Use")
aux.RegisterItemEvent(4700077, 0, "I4700077_CanUse")
aux.RegisterItemEvent(4700077, 1, "I4700077_Use")
aux.RegisterItemEvent(4700078, 0, "I4700078_CanUse")
aux.RegisterItemEvent(4700078, 1, "I4700078_Use")
aux.RegisterItemEvent(4700079, 0, "I4700079_CanUse")
aux.RegisterItemEvent(4700079, 1, "I4700079_Use")
aux.RegisterItemEvent(4700080, 0, "I4700080_CanUse")
aux.RegisterItemEvent(4700080, 1, "I4700080_Use")
