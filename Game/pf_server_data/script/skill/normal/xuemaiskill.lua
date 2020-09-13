--判断如果体力值低于10%，焚煞不可使用
function fensha_CanCast(MapID, InstanceID, SkillID, OwnerID, TargetID)
	local fullHp = role.GetRoleAttValue(MapID, InstanceID, OwnerID,8)
	local curHp = role.GetRoleAttValue(MapID, InstanceID, OwnerID,9)
	if curHp/fullHp < 0.1 then
		return 17
	end
	local skillLevel1 = role.IsLearnedSkill(OwnerID,30109)
	local skillLevel = role.IsLearnedSkill(OwnerID,30110)
	if skillLevel1 ~= nil then
		unit.AddBuff(MapID, InstanceID, OwnerID, 3700700+skillLevel1, OwnerID)
	end
	if skillLevel ~= nil then
		unit.AddBuff(MapID, InstanceID, OwnerID, 3700900+skillLevel, OwnerID)
	end
	return 0
end
--注册是否可使用
aux.RegisterSkillEvent(3010901, 0, "fensha_CanCast")
aux.RegisterSkillEvent(3010902, 0, "fensha_CanCast")
aux.RegisterSkillEvent(3010903, 0, "fensha_CanCast")
aux.RegisterSkillEvent(3010904, 0, "fensha_CanCast")
aux.RegisterSkillEvent(3010905, 0, "fensha_CanCast")
aux.RegisterSkillEvent(3010906, 0, "fensha_CanCast")
aux.RegisterSkillEvent(3010907, 0, "fensha_CanCast")
aux.RegisterSkillEvent(3010908, 0, "fensha_CanCast")
aux.RegisterSkillEvent(3010909, 0, "fensha_CanCast")
aux.RegisterSkillEvent(3010910, 0, "fensha_CanCast")
