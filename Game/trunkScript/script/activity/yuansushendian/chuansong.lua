function shendianchuansong_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	--local Act = act.GetActIsStart(37)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local day = os.date("%A")
	if (day == "Saturday" or day == "Sunday") then
	--if Act == true then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206532)		--“元素神殿守门人：\n  1、副本为40级以上可进入。\n  2、入副本无人数限制，若组队进入需判断队伍成员等级，最高与最低等级的玩家不可超过10级。否则提示：队伍成员等级差异过大，无法进入副本。\n  3、进入副本消耗疲劳值25点，副本时间为45分钟。”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 4)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 206541)		--“进入元素神殿”
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
			local minlv = 0		--最小
			local maxlv = 0		--最大
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if level < 30 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206533)		--元素神殿守门人：\n    只有超过30级的玩家才能进入元素神殿。
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if TeamID ~= nil and TeamID ~= 4294967295 then
				local Role = {}
				Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
					for i=1, 6 do
						if Role[i] and Role[i]~= 4294967295 then
						    local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
						    local rolelevel = role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])
							if minlv == 0 then
								minlv = rolelevel
								maxlv = rolelevel
							end
							if maxlv < rolelevel then
							    maxlv = rolelevel
							elseif minlv > rolelevel then
							    minlv = rolelevel
							end
						end
					end
					if maxlv - minlv > 15 then
					    local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20,	206534)		--元素神殿守门人：\n    您队伍中的玩家等级相差过大（大于15级），无法进入元素神殿。
						msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
						msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
						msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373741608, 91, 20, 217)--4567  791   10728

					end
				elseif TeamID ~= nil then
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373741608, 91, 20, 217)

				end
			end
		end
	else
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206564)		--“元素神殿守门人：\n  元素神殿会在每周六、周日全天开启。”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, -1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100001)		--取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(1085204, 7, "shendianchuansong_OnTalk")
--j进入元素神殿后下马

function s53_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 206547)--进入元素神殿
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

--注册
aux.RegisterMapEvent("s53_1", 2, "s53_OnPlayerEnter")

--元素神殿出口
function shendianchukou_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206540)		--“你要离开元素神殿么？”
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100001)		--“确定”
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then

		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512, 685, 23510, 620)--

	end
end
aux.RegisterCreatureEvent(1085206, 7, "shendianchukou_OnTalk")
--[[1085203	神殿日常任务发布人
1085204	元素神殿传送人
1085205	元素神殿开启人
1085206	]]
