--王者
function shihu_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 10601)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 10601)
	end
    local a = TypeID - 4620000
	role.AddSkill(TargetID, 1060100+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4620001, 1, "shihu_SkillBook")
aux.RegisterItemEvent(4620002, 1, "shihu_SkillBook")
aux.RegisterItemEvent(4620003, 1, "shihu_SkillBook")
aux.RegisterItemEvent(4620004, 1, "shihu_SkillBook")

function shihu_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 10601)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4620000 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4620001, 0, "shihu_CanUse")
aux.RegisterItemEvent(4620002, 0, "shihu_CanUse")
aux.RegisterItemEvent(4620003, 0, "shihu_CanUse")
aux.RegisterItemEvent(4620004, 0, "shihu_CanUse")

--神甲
function shoubo_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 10602)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 10602)
	end
    local a = TypeID - 4620100
	role.AddSkill(TargetID, 1060200+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4620101, 1, "shoubo_SkillBook")
aux.RegisterItemEvent(4620102, 1, "shoubo_SkillBook")
aux.RegisterItemEvent(4620103, 1, "shoubo_SkillBook")
aux.RegisterItemEvent(4620104, 1, "shoubo_SkillBook")

function shoubo_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 10602)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4620100 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4620101, 0, "shoubo_CanUse")
aux.RegisterItemEvent(4620102, 0, "shoubo_CanUse")
aux.RegisterItemEvent(4620103, 0, "shoubo_CanUse")
aux.RegisterItemEvent(4620104, 0, "shoubo_CanUse")

--幻矢
function xingchenbaoluan_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 10603)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 10603)
	end
    local a = TypeID - 4620200
	role.AddSkill(TargetID, 1060300+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4620201, 1, "xingchenbaoluan_SkillBook")
aux.RegisterItemEvent(4620202, 1, "xingchenbaoluan_SkillBook")
aux.RegisterItemEvent(4620203, 1, "xingchenbaoluan_SkillBook")
aux.RegisterItemEvent(4620204, 1, "xingchenbaoluan_SkillBook")

function xingchenbaoluan_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 10603)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4620200 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4620201, 0, "xingchenbaoluan_CanUse")
aux.RegisterItemEvent(4620202, 0, "xingchenbaoluan_CanUse")
aux.RegisterItemEvent(4620203, 0, "xingchenbaoluan_CanUse")
aux.RegisterItemEvent(4620204, 0, "xingchenbaoluan_CanUse")

--占星
function xinghe_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 10604)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 10604)
	end
    local a = TypeID - 4620300
	role.AddSkill(TargetID, 1060400+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4620301, 1, "xinghe_SkillBook")
aux.RegisterItemEvent(4620302, 1, "xinghe_SkillBook")
aux.RegisterItemEvent(4620303, 1, "xinghe_SkillBook")
aux.RegisterItemEvent(4620304, 1, "xinghe_SkillBook")

function xinghe_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 10604)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4620300 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4620301, 0, "xinghe_CanUse")
aux.RegisterItemEvent(4620302, 0, "xinghe_CanUse")
aux.RegisterItemEvent(4620303, 0, "xinghe_CanUse")
aux.RegisterItemEvent(4620304, 0, "xinghe_CanUse")

--雷神
function shenpan_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 10605)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 10605)
	end
    local a = TypeID - 4620400
	role.AddSkill(TargetID, 1060500+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4620401, 1, "shenpan_SkillBook")
aux.RegisterItemEvent(4620402, 1, "shenpan_SkillBook")
aux.RegisterItemEvent(4620403, 1, "shenpan_SkillBook")
aux.RegisterItemEvent(4620404, 1, "shenpan_SkillBook")

function shenpan_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 10605)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4620400 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4620401, 0, "shenpan_CanUse")
aux.RegisterItemEvent(4620402, 0, "shenpan_CanUse")
aux.RegisterItemEvent(4620403, 0, "shenpan_CanUse")
aux.RegisterItemEvent(4620404, 0, "shenpan_CanUse")

--破魔
function zhendi_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 10606)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 10606)
	end
    local a = TypeID - 4620500
	role.AddSkill(TargetID, 1060600+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4620501, 1, "zhendi_SkillBook")
aux.RegisterItemEvent(4620502, 1, "zhendi_SkillBook")
aux.RegisterItemEvent(4620503, 1, "zhendi_SkillBook")
aux.RegisterItemEvent(4620504, 1, "zhendi_SkillBook")

function zhendi_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 10606)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4620500 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4620501, 0, "zhendi_CanUse")
aux.RegisterItemEvent(4620502, 0, "zhendi_CanUse")
aux.RegisterItemEvent(4620503, 0, "zhendi_CanUse")
aux.RegisterItemEvent(4620504, 0, "zhendi_CanUse")

--神侍
function zangli_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 10607)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 10607)
	end
    local a = TypeID - 4620600
	role.AddSkill(TargetID, 1060700+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4620601, 1, "zangli_SkillBook")
aux.RegisterItemEvent(4620602, 1, "zangli_SkillBook")
aux.RegisterItemEvent(4620603, 1, "zangli_SkillBook")
aux.RegisterItemEvent(4620604, 1, "zangli_SkillBook")

function zangli_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 10607)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4620600 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4620601, 0, "zangli_CanUse")
aux.RegisterItemEvent(4620602, 0, "zangli_CanUse")
aux.RegisterItemEvent(4620603, 0, "zangli_CanUse")
aux.RegisterItemEvent(4620604, 0, "zangli_CanUse")

--天仪
function zhaoze_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, 10608)
    if level ~= nil then
	    role.RemoveSkill(TargetID, 10608)
	end
    local a = TypeID - 4620700
	role.AddSkill(TargetID, 1060800+a)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 4001)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

end


aux.RegisterItemEvent(4620701, 1, "zhaoze_SkillBook")
aux.RegisterItemEvent(4620702, 1, "zhaoze_SkillBook")
aux.RegisterItemEvent(4620703, 1, "zhaoze_SkillBook")
aux.RegisterItemEvent(4620704, 1, "zhaoze_SkillBook")

function zhaoze_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, 10608)

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if TypeID - 4620700 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4620701, 0, "zhaoze_CanUse")
aux.RegisterItemEvent(4620702, 0, "zhaoze_CanUse")
aux.RegisterItemEvent(4620703, 0, "zhaoze_CanUse")
aux.RegisterItemEvent(4620704, 0, "zhaoze_CanUse")
