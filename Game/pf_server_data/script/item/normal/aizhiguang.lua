--爱之光
function I4800244_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local Rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local Targetlevel = role.GetRoleLevel(Target_MapID, Target_InstanceID, TargetID)
	role.AddRoleExp(MapID, InstanceID, RoleID, Rolelevel*10)
	role.AddRoleExp(Target_MapID, Target_InstanceID, TargetID, Targetlevel*10)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800245, 1, -1, 8, 420)
end



function I4800244_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4800244, 1, "I4800244_OnUse")
aux.RegisterItemEvent(4800244, 0, "I4800244_CanUse")


