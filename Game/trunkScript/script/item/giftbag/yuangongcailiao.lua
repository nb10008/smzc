--员工礼包（85级）
function I4800150_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, flase
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 22) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800150_Use(MapID, InstanceID, TypeID, TargetID)
   local class = role.GetRoleClass(TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 999, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, 999, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 100, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4999005, 300, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090026, 20,  -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020052, 99, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020017, 20,  -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030343, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030348, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030048, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020035, 6, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090011, 99, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4090010, 99, -1, 8, 420)
    if class==1 or class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8130905, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8121903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8110903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8110903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030308, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030308, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030384, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030026, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090028, 10, -1, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8130905, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8121903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8110903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8110903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030308, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030308, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030384, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030026, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090029, 10, -1, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8130906, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8122903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8111903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8111903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030328, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030328, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030384, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030026, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090030, 10, -1, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8130906, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8122903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8111903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8111903, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030328, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030328, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030374, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030020, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090031, 10, -1, 8, 420)
    end
end

aux.RegisterItemEvent(4800150, 0, "I4800150_CanUse")
aux.RegisterItemEvent(4800150, 1, "I4800150_Use")





--员工礼包（90级）
function I4800168_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, flase
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 25) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800168_Use(MapID, InstanceID, TypeID, TargetID)
   local class = role.GetRoleClass(TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4700149, 200, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020037, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020013, 50, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060060, 200,  -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3060058, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030117, 60,  -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040024, 99, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040024, 99, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040024, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040059, 99, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040059, 99, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040059, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030349, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030349, 10, -1, 8, 420)
    if class==1 or class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030380, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030309, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030309, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030049, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030027, 10, -1, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030380, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030309, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030309, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030049, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030027, 10, -1, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030400, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030329, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030329, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030049, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030022, 10, -1, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030400, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030329, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030329, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030049, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030022, 10, -1, 8, 420)
    end
end


aux.RegisterItemEvent(4800168, 0, "I4800168_CanUse")
aux.RegisterItemEvent(4800168, 1, "I4800168_Use")
