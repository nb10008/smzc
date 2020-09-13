
--金币总动员对话脚本
function c6011030_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --如果是第一次对话
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206756)		--“金币总动员特使：\n         现在是第XX期金币总动员活动，本日的大奖基金共有XXXX金，小奖基金共有xxxx金。\n         每天晚上21点30分将会揭晓最终的开奖结果，将会产生3位幸运儿平分奖金（大奖的开出几率为10%），上期奖金为每人XX金。\n         如果中奖玩家不在线，则奖金会累积到下次开奖；中奖者需要从奖金中扣除5%的手续费。”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 9,	act.GetActScriptData(29, 1, benrijiangli_Index))
		msg.AddMsgEvent(MsgID, 9,	act.GetActScriptData(29, 1, jiangchi_JBZDY)+act.GetActScriptData(29, 1, benrijiangli_JBZDY))
		msg.AddMsgEvent(MsgID, 9,	act.GetActScriptData(29, 1, benrijiangli_JBZDY))
		msg.AddMsgEvent(MsgID, 9,	act.GetActScriptData(29, 1, benrijiangli_JBZDY_LastGold))
		msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206757)		--“参与活动”
		msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if rolelevel < 20 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206758)		--“金币总动员特使：\n         只有20级以上的玩家才能参与金币总动员活动。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local jinbizongdongyuan = role.GetRoleScriptData(RoleID, 1, RoleDataType["Time_JBZDY"])
			local benrijiangli_CurTime = act.GetActScriptData(29, 1, benrijiangli_Index)
			if jinbizongdongyuan == benrijiangli_CurTime then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206759)		--“金币总动员特使：\n         你已经报名参加了活动，晚上21：30请在线等候抽取活动的幸运者。”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local money = math.floor(rolelevel/2)
				if role.GetRoleSilver(MapID, InstanceID, RoleID) < money*10000 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	206760)		--“金币总动员特使：\n         你需要XX金才能参与金币总动员活动。”
					msg.AddMsgEvent(MsgID, 24,	TargetID)
					msg.AddMsgEvent(MsgID, 9, money)
					msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
					msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.DecRoleSilver(MapID, InstanceID, RoleID, money*10000, 452)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Time_JBZDY"],benrijiangli_CurTime)
					for k,v in pairs(tbl_JBZDY_Online) do
						if v == RoleID then
							table.remove(tbl_JBZDY_Online,k)
						end
					end
					for k,v in pairs(tbl_JBZDY_Offline) do
						if v == RoleID then
							table.remove(tbl_JBZDY_Offline,k)
						end
					end
					table.insert(tbl_JBZDY_Online,RoleID)
					local bigmoney = math.floor(money*0.3)
					local smallmoney = money - bigmoney
					act.SetActScriptData(29, 1, jiangchi_JBZDY,act.GetActScriptData(29, 1, jiangchi_JBZDY)+bigmoney)
					act.SetActScriptData(29, 1, benrijiangli_JBZDY,act.GetActScriptData(29, 1, benrijiangli_JBZDY)+smallmoney)
					act.SaveActScriptData(29)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 206753)		--报名成功，大奖奖池已有XXX金，小奖奖池已有XXX金。如果晚上9：30开奖时获奖玩家不在线，则该奖金将会自动累计进大奖奖池。
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(29, 1, jiangchi_JBZDY)+act.GetActScriptData(29, 1, benrijiangli_JBZDY))
					msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(29, 1, benrijiangli_JBZDY))
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
        end
	end
end

-- 注册
aux.RegisterCreatureEvent(6011030, 7, "c6011030_OnTalk")

