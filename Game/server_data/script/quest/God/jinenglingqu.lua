--野猪
function c5610023_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local CurTime = tonumber(os.date("%j"))
	local LastTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])/100)
	if CurTime ~= LastTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], CurTime*100)
	end
	local index = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"]) % 100
	if TalkIndex == -1 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 442052)		--技能领取人：\n    如果你不小心弄丢了三界主宰给你的重要物品，我可以帮你向他再要一个。不过以后你可以小心一点了，我可不能每一次都为了这点小事去打扰三界主宰。
		msg.AddMsgEvent(MsgID, 24, TargetID)
		--灵魂净化1
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40057) and role.GetRoleItemNum(RoleID, 4810004) < 1 and role.IsLearnedSkill(RoleID, 20107) == nil and  index%2 == 0 then
		    msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
			msg.AddMsgEvent(MsgID, 1, 442053)		--领取灵魂净化技能书
		end
		--凝聚2
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40058) and role.GetRoleItemNum(RoleID, 4810007) < 1 and role.IsLearnedSkill(RoleID, 90005) == nil and  index%4 - index%2 == 0 then
		    msg.AddMsgEvent(MsgID, 21, 5)			--确定按钮
			msg.AddMsgEvent(MsgID, 1, 442054)		--领取凝聚技能书
		end
		--神化4
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40059) and role.GetRoleItemNum(RoleID, 4622001) < 1 and role.IsLearnedSkill(RoleID, 23000) == nil and  index%8 - index%4 == 0 then
		    msg.AddMsgEvent(MsgID, 21, 6)			--确定按钮
			msg.AddMsgEvent(MsgID, 1, 442055)		--领取神化技能书
		end
		--旅行8
		if role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40060) and role.GetRoleItemNum(RoleID, 4630301) < 1 and index%16 - index%8 == 0 then
		    msg.AddMsgEvent(MsgID, 21, 7)			--确定按钮
			msg.AddMsgEvent(MsgID, 1, 442056)		--领取自选领域：旅行
		end
        msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
	    if role.GetBagFreeSize(RoleID) < 1 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20, 100003)
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.AddMsgEvent(MsgID, 21, 8)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if TalkIndex == 4 then
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])+1)
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4810004, 1, -1, 8, 420)
			elseif TalkIndex == 5 then
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])+2)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4810007, 1, -1, 8, 420)
			elseif TalkIndex == 6 then
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])+4)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4622001, 1, -1, 8, 420)
			elseif TalkIndex == 7 then
			    role.SetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"], role.GetRoleScriptData(RoleID, 1, RoleDataType["God_SkillBook"])+8)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4630301, 1, -1, 8, 420)
			end
		end
	end
end
-- 注册
aux.RegisterCreatureEvent(5610023, 7, "c5610023_OnTalk")

