--骑术技能书
function skill_qishu(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 40002)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 40002)
	end
    local a = TypeID - 4622100
	role.AddSkill(TargetID, 4000200+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4622101, 1, "skill_qishu")
aux.RegisterItemEvent(4622102, 1, "skill_qishu")
aux.RegisterItemEvent(4622103, 1, "skill_qishu")
aux.RegisterItemEvent(4622104, 1, "skill_qishu")
aux.RegisterItemEvent(4622105, 1, "skill_qishu")
aux.RegisterItemEvent(4622106, 1, "skill_qishu")

function qishu_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 40002)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4622100 - 1 < LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4622101, 0, "qishu_CanUse")
aux.RegisterItemEvent(4622102, 0, "qishu_CanUse")
aux.RegisterItemEvent(4622103, 0, "qishu_CanUse")
aux.RegisterItemEvent(4622104, 0, "qishu_CanUse")
aux.RegisterItemEvent(4622105, 0, "qishu_CanUse")
aux.RegisterItemEvent(4622106, 0, "qishu_CanUse")


--骑术精通技能书
function skill_qishujingtong(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 40005)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 40005)
	end
    local a = TypeID - 4622200
	role.AddSkill(TargetID, 4000500+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4622201, 1, "skill_qishujingtong")
aux.RegisterItemEvent(4622202, 1, "skill_qishujingtong")
aux.RegisterItemEvent(4622203, 1, "skill_qishujingtong")
aux.RegisterItemEvent(4622204, 1, "skill_qishujingtong")
aux.RegisterItemEvent(4622205, 1, "skill_qishujingtong")
aux.RegisterItemEvent(4622206, 1, "skill_qishujingtong")
aux.RegisterItemEvent(4622207, 1, "skill_qishujingtong")
aux.RegisterItemEvent(4622208, 1, "skill_qishujingtong")
aux.RegisterItemEvent(4622209, 1, "skill_qishujingtong")
aux.RegisterItemEvent(4622210, 1, "skill_qishujingtong")

function qishujingtong_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 40005)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4622200 - 1 < LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4622201, 0, "qishujingtong_CanUse")
aux.RegisterItemEvent(4622202, 0, "qishujingtong_CanUse")
aux.RegisterItemEvent(4622203, 0, "qishujingtong_CanUse")
aux.RegisterItemEvent(4622204, 0, "qishujingtong_CanUse")
aux.RegisterItemEvent(4622205, 0, "qishujingtong_CanUse")
aux.RegisterItemEvent(4622206, 0, "qishujingtong_CanUse")
aux.RegisterItemEvent(4622207, 0, "qishujingtong_CanUse")
aux.RegisterItemEvent(4622208, 0, "qishujingtong_CanUse")
aux.RegisterItemEvent(4622209, 0, "qishujingtong_CanUse")
aux.RegisterItemEvent(4622210, 0, "qishujingtong_CanUse")
