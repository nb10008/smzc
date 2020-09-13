--回城术等级判断
function S1012401_CanUsehuicheng(MapID, InstanceID, SkillID , RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家等级是否小于10级
	local level = role.GetRoleLevel(MapID, InstanceID , RoleID )
	if(level < 10) then
		--提示内伤为0无需使用
		bRet = 29
	end
	return bRet, bIgnore
end

aux.RegisterSkillEvent(1012401, 0, "S1012401_CanUsehuicheng")



