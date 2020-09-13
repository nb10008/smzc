--80赠点
function I3090101_zengquan(MapID, InstanceID, TypeID, TargetID)
	--修改赠点
	role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 80)
end
aux.RegisterItemEvent(3090101, 1, "I3090101_zengquan")

--80赠点和3个金锤
function I3090102_zengquan(MapID, InstanceID, TypeID, TargetID)
	--修改赠点
	role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 80)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510020, 3, -1, 8, 420)
end
aux.RegisterItemEvent(3090102, 1, "I3090102_zengquan")

function I3090102_CanUse(MapID, InstanceID, TypeID, TargetID)

    local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 1 then
           bRet = 40
	end
	--返回
	return bRet, bIgnore

end

aux.RegisterItemEvent(3090102, 0, "I3090102_CanUse")
