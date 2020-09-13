--熊猫族长传送
function XMzuzhang_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444438)		--“熊猫族长:\n    1月18日到30日可进入诺亚方舟场景。\n    诺亚方舟内是一个充满挑战的地方，只有到达一定水平方可挑战。只有到达60级以上的人才可以进入。\n    进入方舟需要缴纳一张方舟船票”
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 444442)		--“进入诺亚方舟”
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if curyear == 2012 and ((curmon == 1 and curday >= 17) or (curmon == 2 and curday <= 10)) then
			if level < 60 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444455)		--你身上没有方舟船票，无法进入诺亚方舟
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 100001)		--确定
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif role.GetRoleItemNum(RoleID, 4830025) >= 1 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4830025, 1, 420)--船票
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 1232817742, 1174, 2557, 1070)--1174，2557，1070
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444439)		--你身上没有方舟船票，无法进入诺亚方舟
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 100001)		--确定
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444440)		--不在新年活动时间内，无法进怒亚方舟
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100001)		--确定
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(6040020, 7, "XMzuzhang_OnTalk")
function XMfalao_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 444441)		--“熊猫法老:\n    你确定要离开这里吗？如果你离开了，再要进来是需要再次消耗一张船票的。请三思而后行。”
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100001)		--确定
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512, 617, 23510, 594)--617，23510，594
	end
end
aux.RegisterCreatureEvent(6040021, 7, "XMfalao_OnTalk")






