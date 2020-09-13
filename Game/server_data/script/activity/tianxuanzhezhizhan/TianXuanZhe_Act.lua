function Tianxuanzhe_OnClick(MapID, InstanceID, RoleID, TOMsgID, ToMapID, x, y, z)

	local curhour = tonumber(os.date("%H"))--小时
	local curmin = tonumber(os.date("%M"))--分钟
	local day = os.date("%A")--星期
    if TOMsgID == 206701 then
		if (day == "Wednesday" or day == "Sunday") and 20 == curhour and (curmin == 50 or curmin == 51) then
			CreatTianXuanRoleObj(RoleID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 100006) -- 您点击确定的时间过晚，已经超出了时间限制，请自行前往。
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end
	else
        if (day == "Wednesday" or day == "Sunday") and 21 == curhour and (curmin == 0 or curmin == 1) then
			if role.IsRoleInStatus(RoleID, 1024) then
				role.CancelRoleStallState(MapID, InstanceID, RoleID)
			end
			local k = math.random(9)
			local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleID)
			if Target_MapID ~= nil and Target_InstanceID ~= nil then
				role.RoleGotoNewMap(Target_MapID, Target_InstanceID, RoleID, S59_MapID, S59_Point[k].x, S59_Point[k].y, S59_Point[k].z)      --传送
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 100006) -- 您点击确定的时间过晚，已经超出了时间限制，请自行前往。
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end
	end
end



function Tianxuanzhe_OnStart(actID)
	Tianxuanzhe_baoming = 0
    TianXuan_ChooseSide()
	for k,v in pairs(TianXuan_Role) do
	    local Target_MapID, Target_InstanceID = role.GetRoleMapID(k)
		if Target_MapID ~= nil and Target_InstanceID ~= nil then
			msg.SendRoleSwitchMsg(4, Target_MapID, Target_InstanceID, k, 1, 47)
		end
	end
end

function Tianxuanzhe_OnTimerMin(actID)

    local Min = tonumber(os.date("%M"))
	if Min == 57 or Min == 58 or Min == 59 then
	    for k,v in pairs(TianXuan_Battle) do
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 206703)  --天选者之战将在XX分钟后结束
			msg.AddMsgEvent(MsgID, 9, 60-Min)
			msg.DispatchMapMsgEvent(MsgID, S59_MapID,k)
		end
	end
	if Min % 2 == 0 then
	    for k,v in pairs(TianXuan_Battle) do
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 206704)  --新的天选者已经产生
			msg.DispatchMapMsgEvent(MsgID, S59_MapID,k)
		    local BattleObj = TianXuan_Battle[k]
            BattleObj:ChooseTianXuanZhiRen()
			BattleObj.Area:ReSetTianXuanArea()
		end
	end
	for k,v in pairs(TianXuan_Battle) do
		local BattleObj = TianXuan_Battle[k]
		BattleObj:SetPlayerPoint_TianXuan()
		BattleObj:ResetInSidePlayer_TianXuan()
	end

end

function Tianxuanzhe_OnEnd(actID)

    for k,v in pairs(TianXuan_Battle) do
		local BattleObj = TianXuan_Battle[k]
		BattleObj:SetPlayerFinalPoint_TianXuan()
		map.DeleteInstance(S59_MapID, k)
	end

    --战场对象，可能开启多个战场
	TianXuan_Battle = {}
	--阵营对象
	TianXuan_Side = {}
	--玩家对象
	TianXuan_Role = {}
	--刷新道具区域对象
	TianXuan_Area = {}
    TianXuan_Roletotal = 0
end

--注册该活动的相关事件
aux.RegisterActEvent(47, 8, "Tianxuanzhe_OnClick")
--注册该活动的相关事件
aux.RegisterActEvent(47, 3, "Tianxuanzhe_OnEnd")
aux.RegisterActEvent(48, 3, "Tianxuanzhe_OnEnd")
aux.RegisterActEvent(47, 2, "Tianxuanzhe_OnStart")
aux.RegisterActEvent(47, 4, "Tianxuanzhe_OnTimerMin")
aux.RegisterActEvent(48, 2, "Tianxuanzhe_OnStart")
aux.RegisterActEvent(48, 4, "Tianxuanzhe_OnTimerMin")

--斗技场NPC对话脚本
function x5610081_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curyear = tonumber(os.date("%Y"))
	local curtime = curyear*10000 + curmon*100 + curday
    --获取玩家等级
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel < 50 then
	    rolelevel = 50
	elseif rolelevel > 100 then
	    rolelevel = 100
	end
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206705)		--“斗神统领加德：\n         斗神试炼场会在每周三和周日的8点50分开启，想要成为真正的斗神吗？这是唯一的机会！（只有50级以上玩家可以参与此活动）”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		if Tianxuanzhe_baoming == 1 then
			msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	206706)		--“天选者之战报名”
		end
		if act.GetActIsStart(47) == true or act.GetActIsStart(48) == true then
			msg.AddMsgEvent(MsgID, 21,	9)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	206707)		--“进入斗神试炼场”
		elseif act.GetActIsStart(47) ~= true and act.GetActIsStart(48) ~= true then
            if role.GetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_Point"]) ~= 0 then
				msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	206708)		--“领取奖励”
			end
		end

		msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206709)		--“进入训练场”
		msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206710)		--“兑换战场装备”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    if Tianxuanzhe_baoming ~= nil and Tianxuanzhe_baoming == 1 then
		    if TianXuan_Role[RoleID] ~= nil then
	            local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206711)		--“斗神统领加德：\n         你已经报过名了，无法重复报名。”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if rolelevel < 50 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206712)		--“斗神统领加德：\n         只有50级以上玩家才能报名参与此活动。”
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
					msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					CreatTianXuanRoleObj(RoleID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206713)		--“斗神统领加德：\n         报名成功，活动将在9点整准时开始。”
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
					msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	elseif TalkIndex == 5 then
	    if act.GetActIsStart(47) ~= true and act.GetActIsStart(48) ~= true then
		    local point = role.GetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_Point"])
		    local exp1 = math.floor(TianXuan_Exp[rolelevel] * 2 * point / 60)
			local exp2 = 8 * point
			local item = 0
			if point > 80 then
			    item = 4820242
			elseif point > 40 then
			    item = 4820241
			else
			    item = 4820240
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206714)		--“斗神统领加德：\n         上次活动中，你可以领取的奖励：经验XXXX点，斗技值XXX点，额外奖励为XXX。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	exp1)
			msg.AddMsgEvent(MsgID, 9,	exp2)
			msg.AddMsgEvent(MsgID, 4,	item)
			msg.AddMsgEvent(MsgID, 21,	11)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 6 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206715)		--“斗神统领加德：\n         在训练场中PK不会受到任何的惩罚，即使是同一个家族的人之间也可以互相切磋战斗技巧。确定要进入训练场吗？”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	8)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206709)		    --“进入训练场”
		msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 7 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206716)		--“斗神统领加德：\n         天选者套装只能存在1个小时，且只能在斗神试炼场中使用。\n         兑换天选者套装需要100金以及40个天神代币，请选择想要购买的天选者套装：”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	12)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206717)		    --“神兵套装”
		msg.AddMsgEvent(MsgID, 21,	13)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206718)		    --“玄翎套装”
		msg.AddMsgEvent(MsgID, 21,	14)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206719)		    --“乾坤套装”
		msg.AddMsgEvent(MsgID, 21,	15)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206720)		    --“仙音套装”
		msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 8 then
	    if Xunlianchang_TXZ == nil then
		    Xunlianchang_TXZ = {}
		end
		Xunlianchang_TXZ[RoleID] = 1
	    local k = math.random(9)
	    role.RoleGotoNewMap(MapID, InstanceID, RoleID, S59_MapID, S59_Point[k].x, S59_Point[k].y, S59_Point[k].z)      --传送
	elseif TalkIndex == 9 then
	    if GetTianXuanRoleObj(RoleID) == nil then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206721)		--“斗神统领加德：\n         你没有报名，无法进入斗神试炼场。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local k = math.random(9)
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, S59_MapID, S59_Point[k].x, S59_Point[k].y, S59_Point[k].z)      --传送
        end
	elseif TalkIndex == 11 then
		local num = role.GetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_time"])
		if num ~= curtime then
			local FreeSize = role.GetBagFreeSize(RoleID)
			if FreeSize < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	100003)		--“斗神统领加德：\n         你没有足够的背包空间，请整一下背包再兑换套装礼包。”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 9,	1)	--
				msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if act.GetActIsStart(47) ~= true and act.GetActIsStart(48) ~= true then
					local point = role.GetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_Point"])
					local exp1 = math.floor(TianXuan_Exp[rolelevel] * 2.5 * point / 60)
					local exp2 = 8 * point
					local item = 0
					if point > 80 then
						item = 4820242
					elseif point > 40 then
						item = 4820241
					else
						item = 4820240
					end
					role.AddRoleExp(MapID, InstanceID, RoleID, exp1)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_Point"],0)
					role.ModRoleAttValue(MapID, InstanceID, RoleID, 80, exp2)
					role.AddRoleItem(MapID, InstanceID, RoleID, item, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Tianxuanzhe_time"],curtime)
				end
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206732)		--“斗神统领加德：\n         你今天已经兑换套装礼包，不能再次兑换，请明天再来。
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex >= 12 and TalkIndex <= 15 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if role.GetRoleSilver(MapID, InstanceID, RoleID) < 1000000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206722)		--“斗神统领加德：\n         你没有足够的金币，无法兑换套装礼包。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetRoleItemNum(RoleID, 4820081) < 40 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206723)		--“斗神统领加德：\n         你没有足够的天神代币，无法兑换套装礼包。”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if FreeSize < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206724)		--“斗神统领加德：\n         你没有足够的背包空间，请整一下背包再兑换套装礼包。”
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
					msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4820081, 40, 101)
					role.DecRoleSilver(MapID, InstanceID, RoleID, 1000000, 452)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4820243+TalkIndex-12, 1, -1, 8, 420)
				end
			end
		end
	end
end

aux.RegisterCreatureEvent(5610081, 7, "x5610081_OnTalk")

--斗技场宝箱的对话
function x5610080_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206725)		    --“试炼场守卫：\n         你想离开试炼场吗？”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
		msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“确定”
		if InstanceID ~= 100 and InstanceID ~= 101 then
			msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	206710)		--“兑换战场装备”
		end
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if InstanceID == 100 or InstanceID == 101 then
			local num1 =role.GetRoleItemNum(RoleID, 4800283)
			local num2 =role.GetRoleItemNum(RoleID, 4800284)
			if num1 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800283, num1, 101)
			end
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800284, num2, 101)
			end
		end
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, S59_ExitMapID, S59_ExitPoint.x, S59_ExitPoint.y, S59_ExitPoint.z)      --传送
	elseif TalkIndex == 7 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206727)		--“试炼场守卫：\n         天选者套装只能存在1个小时，且只能在斗神试炼场中使用。离开斗神试炼场时，穿着在身上的天选者套装将会消失。\n         兑换天选者套装需要100金以及40个天神代币，请选择想要购买的天选者套装：”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	12)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206717)		    --“神兵套装”
		msg.AddMsgEvent(MsgID, 21,	13)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206718)		    --“玄翎套装”
		msg.AddMsgEvent(MsgID, 21,	14)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206719)		    --“乾坤套装”
		msg.AddMsgEvent(MsgID, 21,	15)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206720)		    --“仙音套装”
		msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex >= 12 and TalkIndex <= 15 then
	    local FreeSize = role.GetBagFreeSize(RoleID)
		if role.GetRoleSilver(MapID, InstanceID, RoleID) < 1000000 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206728)		--“试炼场守卫：\n         你没有足够的金币，无法兑换套装礼包。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetRoleItemNum(RoleID, 4820081) < 40 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206729)		--“试炼场守卫：\n         你没有足够的天神代币，无法兑换套装礼包。”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if FreeSize < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206730)		--“试炼场守卫：\n         你没有足够的背包空间，请整一下背包再兑换套装礼包。”
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
					msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
				    role.RemoveFromRole(MapID, InstanceID, RoleID, 4820081, 40, 101)
					role.DecRoleSilver(MapID, InstanceID, RoleID, 1000000, 452)
				    role.AddRoleItem(MapID, InstanceID, RoleID, 4820243+TalkIndex-12, 1, -1, 8, 420)
				end
			end
		end
	end
end

aux.RegisterCreatureEvent(5610080, 7, "x5610080_OnTalk")
