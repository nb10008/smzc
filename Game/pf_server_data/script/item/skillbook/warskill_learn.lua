--技能书学习
--技能书效果
function warskill_learn(MapID, InstanceID, TypeID, TargetID)
	if TypeID%10 == 1 then
		role.AddSkill(TargetID, TypeID-420700)
	else
		role.RemoveSkill(TargetID, math.floor((TypeID-420701)/100))
		role.AddSkill(TargetID, TypeID-420700)
	end
end
aux.RegisterItemEvent(4620801, 1, "warskill_learn")
aux.RegisterItemEvent(4620802, 1, "warskill_learn")
aux.RegisterItemEvent(4620803, 1, "warskill_learn")
aux.RegisterItemEvent(4620804, 1, "warskill_learn")
aux.RegisterItemEvent(4620805, 1, "warskill_learn")
aux.RegisterItemEvent(4620901, 1, "warskill_learn")
aux.RegisterItemEvent(4620902, 1, "warskill_learn")
aux.RegisterItemEvent(4620903, 1, "warskill_learn")
aux.RegisterItemEvent(4620904, 1, "warskill_learn")
aux.RegisterItemEvent(4620905, 1, "warskill_learn")
aux.RegisterItemEvent(4621001, 1, "warskill_learn")
aux.RegisterItemEvent(4621002, 1, "warskill_learn")
aux.RegisterItemEvent(4621003, 1, "warskill_learn")
aux.RegisterItemEvent(4621004, 1, "warskill_learn")
aux.RegisterItemEvent(4621005, 1, "warskill_learn")
aux.RegisterItemEvent(4621101, 1, "warskill_learn")
aux.RegisterItemEvent(4621102, 1, "warskill_learn")
aux.RegisterItemEvent(4621103, 1, "warskill_learn")
aux.RegisterItemEvent(4621104, 1, "warskill_learn")
aux.RegisterItemEvent(4621105, 1, "warskill_learn")
aux.RegisterItemEvent(4621201, 1, "warskill_learn")
aux.RegisterItemEvent(4621202, 1, "warskill_learn")
aux.RegisterItemEvent(4621203, 1, "warskill_learn")
aux.RegisterItemEvent(4621204, 1, "warskill_learn")
aux.RegisterItemEvent(4621205, 1, "warskill_learn")
aux.RegisterItemEvent(4621301, 1, "warskill_learn")
aux.RegisterItemEvent(4621302, 1, "warskill_learn")
aux.RegisterItemEvent(4621303, 1, "warskill_learn")
aux.RegisterItemEvent(4621304, 1, "warskill_learn")
aux.RegisterItemEvent(4621305, 1, "warskill_learn")
aux.RegisterItemEvent(4621401, 1, "warskill_learn")
aux.RegisterItemEvent(4621402, 1, "warskill_learn")
aux.RegisterItemEvent(4621403, 1, "warskill_learn")
aux.RegisterItemEvent(4621404, 1, "warskill_learn")
aux.RegisterItemEvent(4621405, 1, "warskill_learn")
aux.RegisterItemEvent(4621501, 1, "warskill_learn")
aux.RegisterItemEvent(4621502, 1, "warskill_learn")
aux.RegisterItemEvent(4621503, 1, "warskill_learn")
aux.RegisterItemEvent(4621504, 1, "warskill_learn")
aux.RegisterItemEvent(4621505, 1, "warskill_learn")
aux.RegisterItemEvent(4621601, 1, "warskill_learn")
aux.RegisterItemEvent(4621602, 1, "warskill_learn")
aux.RegisterItemEvent(4621603, 1, "warskill_learn")
aux.RegisterItemEvent(4621604, 1, "warskill_learn")
aux.RegisterItemEvent(4621605, 1, "warskill_learn")
aux.RegisterItemEvent(4621701, 1, "warskill_learn")
aux.RegisterItemEvent(4621702, 1, "warskill_learn")
aux.RegisterItemEvent(4621703, 1, "warskill_learn")
aux.RegisterItemEvent(4621704, 1, "warskill_learn")
aux.RegisterItemEvent(4621705, 1, "warskill_learn")
aux.RegisterItemEvent(4621801, 1, "warskill_learn")
aux.RegisterItemEvent(4621802, 1, "warskill_learn")
aux.RegisterItemEvent(4621803, 1, "warskill_learn")
aux.RegisterItemEvent(4621804, 1, "warskill_learn")
aux.RegisterItemEvent(4621805, 1, "warskill_learn")

--技能书使用限制
function warskillbook_canuse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, math.floor((TypeID-420700)/100))

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if (TypeID - 420700)%10 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4620801, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4620802, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4620803, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4620804, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4620805, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4620901, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4620902, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4620903, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4620904, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4620905, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621001, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621002, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621003, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621004, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621005, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621101, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621102, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621103, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621104, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621105, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621201, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621202, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621203, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621204, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621205, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621301, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621302, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621303, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621304, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621305, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621401, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621402, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621403, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621404, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621405, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621501, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621502, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621503, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621504, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621505, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621601, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621602, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621603, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621604, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621605, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621701, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621702, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621703, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621704, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621705, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621801, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621802, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621803, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621804, 0, "warskillbook_canuse")
aux.RegisterItemEvent(4621805, 0, "warskillbook_canuse")










