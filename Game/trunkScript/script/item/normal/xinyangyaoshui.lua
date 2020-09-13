--×·É±GMµÄ±¦Ïä
function I4530110_Use(MapID, InstanceID, TypeID, TargetID)

    role.ModRoleAttValue(MapID, InstanceID, TargetID, 107, 10)

end

aux.RegisterItemEvent(4530110, 1, "I4530110_Use")


function I4820087_Use(MapID, InstanceID, TypeID, TargetID)

    role.ModRoleAttValue(MapID, InstanceID, TargetID, 107, 1000)

end

aux.RegisterItemEvent(4820087, 1, "I4820087_Use")
