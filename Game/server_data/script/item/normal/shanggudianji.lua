--�Ϲŵ伮
function I4820035_OnUse(MapID, InstanceID, TypeID, TargetID)
    local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
    if level < 60 then
    role.AddRoleExp(MapID, InstanceID, TargetID, 5000000)
    elseif level >=60 and level < 80 then
    role.AddRoleExp(MapID, InstanceID, TargetID, 10000000)
    elseif level >=80 and level < 90 then
    role.AddRoleExp(MapID, InstanceID, TargetID, 20000000)
    elseif level >=90 and level < 95 then
    role.AddRoleExp(MapID, InstanceID, TargetID, 40000000)
    elseif level >=95 then
    role.AddRoleExp(MapID, InstanceID, TargetID, 80000000)
    end

end

aux.RegisterItemEvent(4820035, 1, "I4820035_OnUse")
