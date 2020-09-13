--内伤药剂使用判断
function I3010042_CanUseneishangyao(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家的内伤是否大于0
	local neishang = role.GetRoleAttValue(MapID, InstanceID , TargetID , 64 )
	if(neishang == 0) then
		--提示内伤为0无需使用
		bRet = 31
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3010042, 0, "I3010042_CanUseneishangyao")
