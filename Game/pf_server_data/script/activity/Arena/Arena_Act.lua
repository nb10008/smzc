

function ArenaAct_OnStart(actID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100131)  --凯旋者帕伊已经吹响了斗技场开始的号角。谁才是真正的强者，谁才能成为新的斗技之王，让我们拭目以待！
	msg.DispatchBroadcast(MsgID, -1, -1, -1)

    map.MapCreateColCreature(3424073512, -1, 5512001, 556, 22980, 487, "")
	map.MapCreateColCreature(3424073512, -1, 5512001, 827, 23510, 614, "")
	map.MapCreateColCreature(3424073512, -1, 5512001, 291, 23510, 754, "")

end

function ArenaAct_OnTimerMin(actID)

    local Min = tonumber(os.date("%M"))
	if Min == 5 or Min == 15 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100132)  --斗技场开放将会持续1个小时，参与比赛的玩家将有机会获得橙色饰品及竞技专用技能！
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
	end

end

function ArenaAct_OnEnd(actID)

	role.CloseArenaWaitingRoom()

end

--注册该活动的相关事件
aux.RegisterActEvent(31, 3, "ArenaAct_OnEnd")
aux.RegisterActEvent(32, 3, "ArenaAct_OnEnd")
aux.RegisterActEvent(31, 2, "ArenaAct_OnStart")
aux.RegisterActEvent(31, 4, "ArenaAct_OnTimerMin")
aux.RegisterActEvent(32, 2, "ArenaAct_OnStart")
aux.RegisterActEvent(32, 4, "ArenaAct_OnTimerMin")

--斗技场NPC对话脚本
function x5512001_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206263)		--“凯旋者帕伊：\n         力量与荣耀在向你招手！你还在等什么？快点进入斗技场吧！”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206264)		--“加入随机斗技场”
		msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206265)		--“随机斗技场规则”
		msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206266)		--“斗技场其他事务”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    if act.GetActIsStart(31) ~= true and act.GetActIsStart(32) ~= true then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206280)		--“凯旋者帕伊：\n         斗技场只在每天特定的时间才会开放，去活动界面了解一下它的开启时间吧！”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif rolelevel < 40 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206267)		--“凯旋者帕伊：\n         只有40级以上的玩家才能参与随机斗技场比赛。您的等级不足，还是回去磨练磨练再来吧！”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69360) then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206281)		--“凯旋者帕伊：\n         您必须等待报名参赛的状态消失之后才能再次加入斗技场比赛。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    role.DeletRoleFromArenaTable(RoleID)
		    role.JoinArena(MapID,InstanceID,RoleID)
		end
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206268)		--“凯旋者帕伊：\n         只有40级以上的玩家才能参与随机斗技场比赛。\n          比赛将在参与人数达到6个人以后自动开始，到时候我们会为您随机分配队友和对手。比赛开始时您有1分钟的准备时间，之后将会有10分钟的战斗时间。\n          比赛获胜的条件是：击杀所有敌人或者时间结束时存活人数多于对方。\n          在斗技场中获胜的玩家将会获得荣誉值奖励，从而登上荣誉排行榜。在斗技比赛过程中离开或者错过了竞技场的进入时间则会损失一定得荣誉。（荣誉值的获得与损失是随对手的强弱以及个人荣誉值的多少而改变的。）\n          每天完成10场斗技场比赛将可以向我领取经验及斗技点数的奖励。同时你可以使用斗技点数及斗技场中获得的斗技场金币来购买斗技场专属商品。”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206269)		--“凯旋者帕伊：\n         有什么我能帮助您的吗？”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206270)		    --“领取每日奖励”
		msg.AddMsgEvent(MsgID, 21,	8)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206271)		    --“40斗技值兑换1个斗技场金币”
		msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 7 then
	    local TimesToday = role.GetRoleScriptData(RoleID, 1, RoleDataType["TimesToday_Arena"])
		if TimesToday > 1000 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20,	206272)		--“凯旋者帕伊：\n         您今天已经领取过一次斗技场奖励了，无法再次领取”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TimesToday < 6 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20,	206273)		--“凯旋者帕伊：\n         您今天只完成了X场斗技场比赛，需要完成10场才能领取奖励。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	TimesToday)
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local roleexp = math.floor(activity_expe[rolelevel]*0.8)
			role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
			role.ModRoleAttValue(MapID, InstanceID, RoleID, 80, 250)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["TimesToday_Arena"], TimesToday+1000)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206274)		--“凯旋者帕伊：\n         您获得了斗技场每日奖励：XXX点经验和XX点斗技值。斗技值可以用来在斗技场装备商处购买斗技场饰品。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	roleexp)
			msg.AddMsgEvent(MsgID, 9,	250)
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			--兑换奖励人数
			UpdateSeverData(57, 1)
			--玩家计数
			UpdateRoleData(RoleID, 13, 1)
		end
	elseif TalkIndex == 8 then
	    if role.GetRoleAttValue(MapID, InstanceID, RoleID, 80) < 25 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 20,	206275)		--“凯旋者帕伊：\n         对不起，您没有足够的斗技值，我无法为您对换。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    if role.GetBagFreeSize(RoleID) >= 1 then
			    role.ModRoleAttValue(MapID, InstanceID, RoleID, 80, -25)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4600001, 1, -1, 8, 420)
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206269)		--“凯旋者帕伊：\n         有什么我能帮助您的吗？”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	206270)		    --“领取每日奖励”
				msg.AddMsgEvent(MsgID, 21,	8)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	206271)		    --“40斗技值兑换1个斗技场金币”
				msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local MsgID = msg.BeginMsgEvent()
			    msg.AddMsgEvent(MsgID, 20,	206276)		--“凯旋者帕伊：\n         请确保您身上有足够的空位再来兑换。”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	end
end

aux.RegisterCreatureEvent(5512001, 7, "x5512001_OnTalk")

--斗技场宝箱的对话
function ArenaBox_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

    if TalkIndex == -1 then
		if Arena[InstanceID] ~= nil and Arena[InstanceID].Battle ~= nil then
			if (Arena[InstanceID].Battle == 0 or Arena[InstanceID].Battle == 1) or ArenaPlayer[RoleID].Info[InstanceID].Win == 0 then
            	local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206277)		--“    这是个斗技场宝箱，里面装的东西是给斗技场比赛胜利者的奖赏。”
				msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif Arena[InstanceID].Battle == 2 and ArenaPlayer[RoleID].Info[InstanceID].Win == 1 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206278)		--“    胜利者，快领取你的竞技场奖励吧！”
				msg.AddMsgEvent(MsgID, 24,	TargetID)
				msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 4 and ArenaPlayer[RoleID].Info[InstanceID].Win == 1 then
	    if Arena[InstanceID] ~= nil and ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil and Arena[InstanceID].Battle == 2 then
		    for i=1,6 do
				local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
				if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID and ArenaPlayer[Arena[InstanceID].Player[i]] ~= nil and ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID] ~= nil then
					if ArenaPlayer[RoleID].Info[InstanceID].Side == ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID].Side then
					    ArenaBox_GetItem(MapID, InstanceID, TargetTypeID, Arena[InstanceID].Player[i])
					end
				end
			end
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
	end
end

aux.RegisterCreatureEvent(5512004, 7, "ArenaBox_OnTalk")
aux.RegisterCreatureEvent(5512005, 7, "ArenaBox_OnTalk")
