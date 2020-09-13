--魂晶技能
function s4820104_SkillBook(MapID, InstanceID, TypeID, TargetID)

	role.AddSkill(TargetID, 2505201)

end


aux.RegisterItemEvent(4820104, 1, "s4820104_SkillBook")

function s4820104_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 25052)

	if LearnedSkill ~= nil then
		--提示物品不可使用
		bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4820104, 0, "s4820104_CanUse")

function s4820163_SkillBook(MapID, InstanceID, TypeID, TargetID)
    local time1 = role.GetSkillActiveTime(MapID,InstanceID,TargetID,25040)
	role.RemoveSkill(TargetID, 25040)
	role.AddSkill(TargetID, 2504002)
	role.SetSkillActiveTime(MapID,InstanceID,TargetID,25040,time1)
end


aux.RegisterItemEvent(4820163, 1, "s4820163_SkillBook")

function s4820163_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 25040)

	if LearnedSkill == nil or LearnedSkill >= 2 then
		--提示物品不可使用
		bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4820163, 0, "s4820163_CanUse")

function s4820164_SkillBook(MapID, InstanceID, TypeID, TargetID)
	local time1 = role.GetSkillActiveTime(MapID,InstanceID,TargetID,25040)
	role.RemoveSkill(TargetID, 25040)
	role.AddSkill(TargetID, 2504003)
	role.SetSkillActiveTime(MapID,InstanceID,TargetID,25040,time1)
end


aux.RegisterItemEvent(4820164, 1, "s4820164_SkillBook")

function s4820164_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 25040)

	if LearnedSkill == nil or LearnedSkill ~= 2 then
		--提示物品不可使用
		bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4820164, 0, "s4820164_CanUse")

function s4820165_SkillBook(MapID, InstanceID, TypeID, TargetID)
    local time1 = role.GetSkillActiveTime(MapID,InstanceID,TargetID,25043)
	role.RemoveSkill(TargetID, 25043)
	role.AddSkill(TargetID, 2504302)
	role.SetSkillActiveTime(MapID,InstanceID,TargetID,25043,time1)
end


aux.RegisterItemEvent(4820165, 1, "s4820165_SkillBook")

function s4820165_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 25043)

	if LearnedSkill == nil or LearnedSkill >= 2 then
		--提示物品不可使用
		bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4820165, 0, "s4820165_CanUse")

function s4820166_SkillBook(MapID, InstanceID, TypeID, TargetID)
    local time1 = role.GetSkillActiveTime(MapID,InstanceID,TargetID,25043)
	role.RemoveSkill(TargetID, 25043)
	role.AddSkill(TargetID, 2504303)
	role.SetSkillActiveTime(MapID,InstanceID,TargetID,25043,time1)
end


aux.RegisterItemEvent(4820166, 1, "s4820166_SkillBook")

function s4820166_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 25043)

	if LearnedSkill == nil or LearnedSkill ~= 2 then
		--提示物品不可使用
		bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4820166, 0, "s4820166_CanUse")

function s4820167_SkillBook(MapID, InstanceID, TypeID, TargetID)
    local time1 = role.GetSkillActiveTime(MapID,InstanceID,TargetID,25046)
	role.RemoveSkill(TargetID, 25046)
	role.AddSkill(TargetID, 2504602)
	role.SetSkillActiveTime(MapID,InstanceID,TargetID,25046,time1)
end


aux.RegisterItemEvent(4820167, 1, "s4820167_SkillBook")

function s4820167_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 25046)

	if LearnedSkill == nil or LearnedSkill >= 2 then
		--提示物品不可使用
		bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4820167, 0, "s4820167_CanUse")

function s4820168_SkillBook(MapID, InstanceID, TypeID, TargetID)
    local time1 = role.GetSkillActiveTime(MapID,InstanceID,TargetID,25046)
	role.RemoveSkill(TargetID, 25046)
	role.AddSkill(TargetID, 2504603)
    role.SetSkillActiveTime(MapID,InstanceID,TargetID,25046,time1)
end


aux.RegisterItemEvent(4820168, 1, "s4820168_SkillBook")

function s4820168_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 25046)

	if LearnedSkill == nil or LearnedSkill ~= 2 then
		--提示物品不可使用
		bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4820168, 0, "s4820168_CanUse")


function s4820169_SkillBook(MapID, InstanceID, TypeID, TargetID)
    local time1 = role.GetSkillActiveTime(MapID,InstanceID,TargetID,25049)
	role.RemoveSkill(TargetID, 25049)
	role.AddSkill(TargetID, 2504902)
	role.SetSkillActiveTime(MapID,InstanceID,TargetID,25049,time1)
end


aux.RegisterItemEvent(4820169, 1, "s4820169_SkillBook")

function s4820169_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 25049)

	if LearnedSkill == nil or LearnedSkill >= 2 then
		--提示物品不可使用
		bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4820169, 0, "s4820169_CanUse")

function s4820170_SkillBook(MapID, InstanceID, TypeID, TargetID)
    local time1 = role.GetSkillActiveTime(MapID,InstanceID,TargetID,25049)
	role.RemoveSkill(TargetID, 25049)
	role.AddSkill(TargetID, 2504903)
	role.SetSkillActiveTime(MapID,InstanceID,TargetID,25049,time1)
end


aux.RegisterItemEvent(4820170, 1, "s4820170_SkillBook")

function s4820170_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 25049)

	if LearnedSkill == nil or LearnedSkill ~= 2 then
		--提示物品不可使用
		bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4820170, 0, "s4820170_CanUse")
