
--判断如果不是配偶不能使用
function s9000601_canuse(MapID, InstanceID, SkillID, OwnerID, TargetID)
	if role.GetRoleLoverID(MapID, InstanceID,OwnerID) == nil or role.GetRoleLoverID(MapID, InstanceID,OwnerID) ~=  TargetID  then
		return 14
	end
	for i in pairs(Skill_BattleGround) do
		if MapID == Skill_BattleGround[i] then
        		return 23
		end
   	end
	return 0
end
--注册是否可使用
aux.RegisterSkillEvent(9000601, 0, "s9000601_canuse")


--判断如果不是配偶不能使用
function s9000701_canuse(MapID, InstanceID, SkillID, OwnerID, TargetID)
    local LoverID = role.GetRoleLoverID(MapID, InstanceID,OwnerID)
	if LoverID == nil then
		return 7
	else
		--[[for i in pairs(Skill_BattleGround) do
			if MapID == Skill_BattleGround[i] then
					return 23
			end
		end]]
		local Map, Instance = role.GetRoleMapID(LoverID)
		local bool = false
		if Map ~= nil then
			for k,v in pairs(Skill_NormalCRC) do
				if Map == v then
					bool = true
					break
				end
			end
		end
		if bool then
			--[[for i in pairs(Skill_NormalCRC) do
				if MapID == Skill_NormalCRC[i] then
						return 0
				end
			end
			for i in pairs(Skill_NormalCRC) do
				if MapID == Skill_NormalCRC[i] then]]
						return 0
			--	end
			--end
		end

		return 68
	end
end
--注册是否可使用
aux.RegisterSkillEvent(9000701, 0, "s9000701_canuse")

--使用函数
function s9000701_Cast(MapID, InstanceID, SkillID, OwnerID)

    local LoverID = role.GetRoleLoverID(MapID, InstanceID,OwnerID)
	if LoverID ~= nil then
		local Map, Instance = role.GetRoleMapID(LoverID)
		if Map ~= nil then
		    local x, y ,z = unit.GetPosition(Map, Instance, LoverID)
			if role.IsRoleInStatus(OwnerID, 1024) then
				role.CancelRoleStallState(MapID, InstanceID, OwnerID)
			end
            role.RoleGotoNewMap(MapID, InstanceID, OwnerID, Map, x, math.floor(y/50) ,z)
		end
	end

end

aux.RegisterSkillEvent(9000701, 1, "s9000701_Cast")

