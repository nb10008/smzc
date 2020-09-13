--新增30装备礼包（天神成长礼包30级）
function I4530177_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, flase
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 4) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4530177_Use(MapID, InstanceID, TypeID, TargetID)
   local class = role.GetRoleClass(TargetID)
    if class==1 or class==2 or class==6 or class==7 or class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8060012, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8070012, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8080012, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8090012, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 or class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8061012, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8071012, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8081012, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8091012, 1, 4, 8, 420)
    end
end

aux.RegisterItemEvent(4530177, 0, "I4530177_CanUse")
aux.RegisterItemEvent(4530177, 1, "I4530177_Use")





--新增50装备礼包（天神成长礼包50级）
function I4530178_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, flase
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 5) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4530178_Use(MapID, InstanceID, TypeID, TargetID)
   local class = role.GetRoleClass(TargetID)
    if class==1 or class==2 or class==6 or class==7 or class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8050013, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8060013, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8070013, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8080013, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8090013, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 or class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8051013, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8061013, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8071013, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8081013, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8091013, 1, 4, 8, 420)
    end
end


aux.RegisterItemEvent(4530178, 0, "I4530178_CanUse")
aux.RegisterItemEvent(4530178, 1, "I4530178_Use")
