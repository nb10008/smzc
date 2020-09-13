--王者
function Arena_SkillBook(MapID, InstanceID, TypeID, TargetID)
	local Book = {}
    Book[4610001] = 4100101
	Book[4610002] = 4100201
	Book[4610003] = 4100301
	Book[4610004] = 4100401
	Book[4610005] = 4100402
	Book[4610006] = 4100403
	Book[4610007] = 4100501
	Book[4610008] = 4100502
	Book[4610009] = 4100503
	Book[4610010] = 4100504
	Book[4610011] = 4100505
	Book[4610012] = 4100601
	Book[4610013] = 4100602
	Book[4610014] = 4100603
	Book[4610015] = 4100701
	Book[4610016] = 4100702
	Book[4610017] = 4100703
	Book[4610018] = 4100704
	Book[4610019] = 4100705
	Book[4610020] = 4100801
	Book[4610021] = 4100802
	Book[4610022] = 4100803
	Book[4610023] = 4100901
	Book[4610024] = 4100902
	Book[4610025] = 4100903
	Book[4610026] = 4100904
	Book[4610027] = 4100905
	Book[4610028] = 4101001
	Book[4610029] = 4101002
	Book[4610030] = 4101003
	Book[4610031] = 4101101
	Book[4610032] = 4101102
	Book[4610033] = 4101103
	Book[4610034] = 4101104
	Book[4610035] = 4101105

	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.IsLearnedSkill(TargetID, math.floor(Book[TypeID]/100))
    if level ~= nil then
	    role.RemoveSkill(TargetID, math.floor(Book[TypeID]/100))
	end
	role.AddSkill(TargetID, Book[TypeID])
end


aux.RegisterItemEvent(4610001, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610002, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610003, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610004, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610005, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610006, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610007, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610008, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610009, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610010, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610011, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610012, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610013, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610014, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610015, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610016, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610017, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610018, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610019, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610020, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610021, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610022, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610023, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610024, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610025, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610026, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610027, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610028, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610029, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610030, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610031, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610032, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610033, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610034, 1, "Arena_SkillBook")
aux.RegisterItemEvent(4610035, 1, "Arena_SkillBook")


function ArenaBook_CanUse(MapID, InstanceID, TypeID, TargetID)
    local Book = {}
    Book[4610001] = 4100101
	Book[4610002] = 4100201
	Book[4610003] = 4100301
	Book[4610004] = 4100401
	Book[4610005] = 4100402
	Book[4610006] = 4100403
	Book[4610007] = 4100501
	Book[4610008] = 4100502
	Book[4610009] = 4100503
	Book[4610010] = 4100504
	Book[4610011] = 4100505
	Book[4610012] = 4100601
	Book[4610013] = 4100602
	Book[4610014] = 4100603
	Book[4610015] = 4100701
	Book[4610016] = 4100702
	Book[4610017] = 4100703
	Book[4610018] = 4100704
	Book[4610019] = 4100705
	Book[4610020] = 4100801
	Book[4610021] = 4100802
	Book[4610022] = 4100803
	Book[4610023] = 4100901
	Book[4610024] = 4100902
	Book[4610025] = 4100903
	Book[4610026] = 4100904
	Book[4610027] = 4100905
	Book[4610028] = 4101001
	Book[4610029] = 4101002
	Book[4610030] = 4101003
	Book[4610031] = 4101101
	Book[4610032] = 4101102
	Book[4610033] = 4101103
	Book[4610034] = 4101104
	Book[4610035] = 4101105


	local bRet, bIgnore = 0, false
	--判断玩家是否已学会技能且等级合适

	local LearnedSkill = role.IsLearnedSkill(TargetID, math.floor(Book[TypeID]/100))

	if LearnedSkill == nil then
		LearnedSkill = 0
	end

	if Book[TypeID] - math.floor(Book[TypeID]/100)*100 - 1 ~= LearnedSkill then
		--提示物品不可使用
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4002)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4610001, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610002, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610003, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610004, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610005, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610006, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610007, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610008, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610009, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610010, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610011, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610012, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610013, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610014, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610015, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610016, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610017, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610018, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610019, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610020, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610021, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610022, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610023, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610024, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610025, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610026, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610027, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610028, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610029, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610030, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610031, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610032, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610033, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610034, 0, "ArenaBook_CanUse")
aux.RegisterItemEvent(4610035, 0, "ArenaBook_CanUse")
