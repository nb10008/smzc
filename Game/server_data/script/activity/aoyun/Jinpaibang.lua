
--与6040104  金牌榜 对话
function n6040104_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curyear = tonumber(os.date("%y"))
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))

	local curtime = curyear*10000+curmon*100+curday
	local jiangpaijifen_time = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_time"])
	local nowtime = curhour*100 + curmin
	if (curtime - jiangpaijifen_time == 1 and nowtime >= 1200) or (curtime - jiangpaijifen_time > 1)then		--当前日期与玩家数据对比,如果隔天12点每日积分清零
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_got"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_time"], curtime)
	end

	local jingyan = {}
	jingyan[50] = 7776
	jingyan[51] = 9235
	jingyan[52] = 10818
	jingyan[53] = 12602
	jingyan[54] = 14538
	jingyan[55] = 17772
	jingyan[56] = 21095
	jingyan[57] = 25884
	jingyan[58] = 31610
	jingyan[59] = 38089
	jingyan[60] = 34069
	jingyan[61] = 30357
	jingyan[62] = 26976
	jingyan[63] = 23950
	jingyan[64] = 26542
	jingyan[65] = 26956
	jingyan[66] = 27371
	jingyan[67] = 27786
	jingyan[68] = 28201
	jingyan[69] = 28615
	jingyan[70] = 29030
	jingyan[71] = 29445
	jingyan[72] = 29859
	jingyan[73] = 30274
	jingyan[74] = 30689
	jingyan[75] = 31104
	jingyan[76] = 31518
	jingyan[77] = 31933
	jingyan[78] = 32348
	jingyan[79] = 32763
	jingyan[80] = 55296
	jingyan[81] = 55987
	jingyan[82] = 56678
	jingyan[83] = 57369
	jingyan[84] = 58060
	jingyan[85] = 58752
	jingyan[86] = 59443
	jingyan[87] = 60134
	jingyan[88] = 60825
	jingyan[89] = 61516
	jingyan[90] = 62208
	jingyan[91] = 62899
	jingyan[92] = 63590
	jingyan[93] = 64281
	jingyan[94] = 64972
	jingyan[95] = 65664
	jingyan[96] = 66355
	jingyan[97] = 67046
	jingyan[98] = 67737
	jingyan[99] = 68428
	jingyan[100] = 69120

	if TalkIndex == -1 then
		local nowtime = curmon*100 + curday
		if nowtime >= 724 and nowtime < 821  then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211100) -- "金牌榜：\n    你可以在这里提交奖牌兑换奖章和积分,查看奖牌榜和领取活动奖励。\n    你当前的每日积分是 <p1>\n    你的整体积分是<p2>"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"]))
			msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_all"]))

			msg.AddMsgEvent(MsgID, 21, 4) -- 选项按钮
			msg.AddMsgEvent(MsgID, 1, 211101) -- “提交奖牌”

			msg.AddMsgEvent(MsgID, 21, 5)   --选项按钮
			msg.AddMsgEvent(MsgID, 1, 211102)   --"查看奖牌榜"

			msg.AddMsgEvent(MsgID, 21, 6)   --选项按钮
			msg.AddMsgEvent(MsgID, 1, 211103)   --"领取奖励"

			msg.AddMsgEvent(MsgID, 21, 7)   --选项按钮
			msg.AddMsgEvent(MsgID, 1, 211104)   --"活动说明"

			msg.AddMsgEvent(MsgID, 21, 20)   --取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)   --"取消"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211100) -- "金牌榜：\n    你可以在这里提交奖牌兑换奖章和积分,查看奖牌榜和领取活动奖励。\n    你当前的每日积分是 <p1>\n    你的整体积分是<p2>"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"]))
			msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_all"]))

			msg.AddMsgEvent(MsgID, 21, 5)   --选项按钮
			msg.AddMsgEvent(MsgID, 1, 211102)   --"查看奖牌榜"

			msg.AddMsgEvent(MsgID, 21, 6)   --选项按钮
			msg.AddMsgEvent(MsgID, 1, 211103)   --"领取奖励"

			msg.AddMsgEvent(MsgID, 21, 7)   --选项按钮
			msg.AddMsgEvent(MsgID, 1, 211104)   --"活动说明"

			msg.AddMsgEvent(MsgID, 21, 20)   --取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)   --"取消"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end

	elseif TalkIndex == 4 then			--提交奖牌
		local lev = role.GetRoleLevel(MapID, InstanceID,RoleID)

		if lev < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211204) -- 等级不足50
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif nowtime >= 2230 or nowtime <= 1200 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211207) -- 金牌榜：\n    现在不能提交金牌
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetBagFreeSize(RoleID) < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211105) -- "金牌榜：\n    背包要有空间才能兑换奖励！"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211122) -- "金牌榜：\n    提交奖牌可以换取积分！\n        提交1枚奖牌可换取30个奥运奖章,10点奖牌积分\n    提交5枚奖牌可换取165个奥运奖章,55点奖牌积分\n    提交10枚奖牌可换取370个奥运奖章,120点奖牌积分"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid

				msg.AddMsgEvent(MsgID, 21, 8)   --选项按钮
				msg.AddMsgEvent(MsgID, 1, 211106)   --"提交奖牌*1"

				msg.AddMsgEvent(MsgID, 21, 9)   --选项按钮
				msg.AddMsgEvent(MsgID, 1, 211107)   --"提交奖牌*5"

				msg.AddMsgEvent(MsgID, 21, 10)   --选项按钮
				msg.AddMsgEvent(MsgID, 1, 211108)   --"提交奖牌*10"

				msg.AddMsgEvent(MsgID, 21, 20)   --取消按钮
				msg.AddMsgEvent(MsgID, 1, 100002)   --"取消"

				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			end
		end
	elseif TalkIndex == 8 then				--提交1枚
		local a = role.GetRoleItemNum(RoleID, 4810037)
		if a < 1 then                              --判断奖牌数量
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211109) -- "金牌榜：\n    你没有足够的奖牌可以提交！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		else
			local level = role.GetRoleLevel(MapID,-1,RoleID)	--提交金牌给予玩家经验奖励
			role.AddRoleExp(MapID, -1, RoleID, jingyan[level])
			role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 30, -1, 8, 420) --给予玩家30个奥运奖章
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810037, 1, 420)  --删除一个奖牌

			HuoDeJiangpaijifen(RoleID,10)				--给予玩家10积分

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211110) -- "金牌榜：\n    想要获得奖励就带更多的奖牌来吧！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end

	elseif TalkIndex == 9 then					--提交5枚
		local a = role.GetRoleItemNum(RoleID, 4810037)
		if a < 5 then                                 --判断奖牌数量是否够5个
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211109) -- "金牌榜：\n    你没有足够的奖牌可以提交！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		else
			local level = role.GetRoleLevel(MapID,-1,RoleID)	--提交金牌给予玩家经验奖励
			role.AddRoleExp(MapID, -1, RoleID, jingyan[level]*5)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 165, -1, 8, 420)     --给予玩家165个奥运奖章
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810037, 5, 420)		--删除5个奖牌

			HuoDeJiangpaijifen(RoleID,55)				--给予玩家55积分

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211110) -- "金牌榜：\n    想要获得奖励就带更多的奖牌来吧！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end


	elseif TalkIndex == 10 then				--提交10枚
		local a = role.GetRoleItemNum(RoleID, 4810037)
		if a < 10 then                                 --判断奖牌数量是否够10个
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211109) -- "金牌榜：\n    你没有足够的奖牌可以提交！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		else
			local level = role.GetRoleLevel(MapID,-1,RoleID)	--提交金牌给予玩家经验奖励
			role.AddRoleExp(MapID, -1, RoleID, jingyan[level]*10)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4810036, 370, -1, 8, 420)     --给予玩家370个奥运奖章
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810037, 10, 420)		--删除10个奖牌

			HuoDeJiangpaijifen(RoleID,120)					--给予玩家120积分

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211110) -- "金牌榜：\n    想要获得奖励就带更多的奖牌来吧！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end



	elseif TalkIndex == 5 then                              --查看奖牌榜
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211121) -- "金牌榜：\n    查看积分排行"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid

		msg.AddMsgEvent(MsgID, 21, 13) -- 选项按钮
		msg.AddMsgEvent(MsgID, 1, 211111) -- “查看每日奖牌积分排行”

		msg.AddMsgEvent(MsgID, 21, 14)   --选项按钮
		msg.AddMsgEvent(MsgID, 1, 211112)   --"查看整体奖牌积分排行"

		msg.AddMsgEvent(MsgID, 21, 20)   --取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)   --"取消"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息

	elseif TalkIndex == 13 then		-- 每日奖牌积分排行
		--[[
		local k
		for i = 11 ,13 do						--判断每日奖牌榜前三名是否都存在
			if act.GetActScriptData(56, 1, i) > 0 then
				k = act.GetActScriptData(56, 1, i)
			else
				k = nil
				break
			end
		end
		if k then
		--]]
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211113) -- "金牌榜：\n    第一名<p1>  <p2>奖牌积分\n    第二名<p3>  <p4>奖牌积分\n    第三名<p5>  <p6>奖牌积分"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 11))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 12))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 13))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 17))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		--[[
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211114) -- "金牌榜：\n    奖牌榜还没统计完全"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end
		--]]


	elseif TalkIndex == 14 then		-- 整体奖牌积分排行
		--[[
		local k2
		for i = 1 ,3 do						--判断整体奖牌榜前三名是否都存在
			if act.GetActScriptData(56, 1, i) > 0 then
				k2 = act.GetActScriptData(56, 1, i)
			else
				k2 = nil
				break
			end
		end
		if k2 then
		--]]
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211113) -- "金牌榜：\n    第一名<p1>  <p2>奖牌积分\n    第二名<p3>  <p4>奖牌积分\n    第三名<p5>  <p6>奖牌积分"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(56, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(56, 1, 7))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		--[[
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211114) -- "金牌榜：\n    奖牌榜还没统计完全"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end
		--]]

	elseif TalkIndex == 6 then           --领取奖励
		local curyear = tonumber(os.date("%y"))
		local curmon = tonumber(os.date("%m"))
		local curday = tonumber(os.date("%d"))


		local curtime = curmon*100+curday
		if curtime < 822 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211115) -- "金牌榜：\n    每日奖励每天只能领取一次,如果你当前的每日奖牌积分排名前三则进行奖励,每天0点将清空每日奖牌积分。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid

			msg.AddMsgEvent(MsgID, 21, 11)   --选项按钮
			msg.AddMsgEvent(MsgID, 1, 211116)   --"领取每日奖励"


			msg.AddMsgEvent(MsgID, 21, 20)   --取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)   --"取消"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211115) -- "金牌榜：\n    每日奖励每天只能领取一次,如果你当前的每日奖牌积分排名前三则进行奖励,每天0点将清空每日奖牌积分。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid

			msg.AddMsgEvent(MsgID, 21, 11)   --选项按钮
			msg.AddMsgEvent(MsgID, 1, 211116)   --"领取每日奖励"

			msg.AddMsgEvent(MsgID, 21, 12)   --选项按钮
			msg.AddMsgEvent(MsgID, 1, 211117)   --"领取最终奖励"


			msg.AddMsgEvent(MsgID, 21, 20)   --取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)   --"取消"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end

	elseif TalkIndex == 11 then           --领取每日奖励
		local g = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_got"])		--每日奖励是否已领取
		local level_today = nil			--level为奖励级别记位
		if g < 1 then									--玩家今天没领取过奖励
			if nowtime >= 1200 and nowtime <=2230 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211208) -- 金牌榜：\n    现在不能领取奖励
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 99)
				msg.AddMsgEvent(MsgID, 1, 100001) -- 取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				for h = 11, 13 do
					if act.GetActScriptData(56, 1, h) == RoleID then		--玩家为前三名时
						level_today = h
						break
					end
				end

				if level_today then
					--根据level_today给予奖励
					if level_today == 11 then
						if role.GetBagFreeSize(RoleID) < 4 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 211105) -- "金牌榜：\n    背包要有空间才能兑换奖励！"
							msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
							msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
							msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
							msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820260, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 3020091, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820035, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4810047, 1, -1, 8, 420)
						end
					end

					if level_today == 12 then
						if role.GetBagFreeSize(RoleID) < 4 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 211105) -- "金牌榜：\n    背包要有空间才能兑换奖励！"
							msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
							msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
							msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
							msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820260, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 3020087, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820035, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4810048, 1, -1, 8, 420)
						end
					end
					if level_today == 13 then
						if role.GetBagFreeSize(RoleID) < 3 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 211105) -- "金牌榜：\n    背包要有空间才能兑换奖励！"
							msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
							msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
							msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
							msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, 3020084, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4820035, 1, -1, 8, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 4810049, 1, -1, 8, 420)
						end
					end
					role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_got"], 1)			--将领奖标记为已领取
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 211118) -- "金牌榜：\n    你的积分太少了,我没法给你奖励！"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				end
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211120) -- "金牌榜：\n    你已经领取过奖励了！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end

	elseif TalkIndex == 12 then
		local level_all	= nil		--level为奖励级别记位
		local g2 = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_gotall"])	--整体奖励是否已领取
		if g2 < 1 then
			for h = 1, 3 do
				if act.GetActScriptData(56, 1, h) == RoleID then			--玩家为前三名时
					level_all = h
					break
				end
			end

			if level_all then
				--根据level_all给予奖励
				if level_all == 1 then
					if role.GetBagFreeSize(RoleID) < 4 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 211105) -- "金牌榜：\n    背包要有空间才能兑换奖励！"
						msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
						msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
						msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
						msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
					else
						role.AddRoleItem(MapID, InstanceID, RoleID, 3020134, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3030079, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4820263, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4222701, 1, -1, 8, 420)
					end
				end

				if level_all == 2 then
					if role.GetBagFreeSize(RoleID) < 3 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 211105) -- "金牌榜：\n    背包要有空间才能兑换奖励！"
						msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
						msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
						msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
						msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
					else
						role.AddRoleItem(MapID, InstanceID, RoleID, 3020131, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3030078, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4820260, 1, -1, 8, 420)
					end
				end
				if level_all == 3 then
					if role.GetBagFreeSize(RoleID) < 3 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 211105) -- "金牌榜：\n    背包要有空间才能兑换奖励！"
						msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
						msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
						msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
						msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
					else
						role.AddRoleItem(MapID, InstanceID, RoleID, 3020126, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3030078, 1, -1, 8, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4820260, 1, -1, 8, 420)
					end
				end
				role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_gotall"], 1)			--将领奖标记为已领取
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 211118) -- "金牌榜：\n    你的积分太少了,我没法给你奖励！"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			end

		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211120) -- "金牌榜：\n    你已经领取过奖励了！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息

		end

	elseif TalkIndex == 7 then           --活动说明
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211119) -- "金牌榜：\n    每日积分榜第一名将获得<color=0xff9933ff>七星石 灵魂链+19 上古典籍 95级圣衣包<color=0xffffffff>\n    每日积分第二名将获得<color=0xff9933ff>七星石 灵魂链+15 上古典籍 85级圣衣包<color=0xffffffff>\n    每日积分第三名将获得 <color=0xff9933ff>灵魂链+12 上古典籍 82级圣衣包<color=0xffffffff>\n    总积分第一名将获得<color=0xff9933ff>附魂石+28 8级红宝石 八星石 黄金麒麟<color=0xffffffff>\n    总积分第二名将获得<color=0xff9933ff>附魂石+25 7级红宝石 七星石 <color=0xffffffff>\n    总积分第三名将获得<color=0xff9933ff>附魂石+20 7级红宝石 七星石<color=0xffffffff>"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
		msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	end

end

aux.RegisterCreatureEvent(6040104, 7, "n6040104_OnTalk")

function HuoDeJiangpaijifen(RoleID, Num)

	local p = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_all"])
	local i = role.GetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"])
	if p == nil then
	    p = 0
	end

	i = i + Num
	p = p + Num
	role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_today"], i)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["jiangpaijifen_all"], p)

	--进行整体积分排行操作
	local arrID = {}			--临时保存玩家ID
	local arrNum = {}			--临时保存积分数据
	local bNew = 1				--是否为前三
	for l=1,3 do
		arrID[l] = act.GetActScriptData(56, 1, l)
		if arrID[l] == nil then			--前三不足,空数据置0
		    arrID[l] = 0
		end
		if arrID[l] == RoleID  then		--玩家为前三名时,更新其整体积分数据
			arrNum[l] = p
			bNew = 0	--标记玩家为前三
		else		--玩家非前三名时
			arrNum[l] = act.GetActScriptData(56, 1, l+4)	--将前三名奖牌积分数据保存至临时数组
			if arrNum[l] == nil then		--将空数据置0
				 arrNum[l] = 0
			 end
		end
	end

	if bNew == 1 then				--如果玩家非前三名,将其数据保存至第四位
		arrID[4] = RoleID;
		arrNum[4] = p;
	else						--如果玩家为前三名,将第四位置0
		arrID[4] = 0;
		arrNum[4] = 0;
	end

	for m=1, 3 do					--循环将前四名比较,按积分数由大到小排序临时数组
		for n=m+1, 4 do
			if arrNum[n] > arrNum[m] then
				local temp = arrNum[m]
				arrNum[m] = arrNum[n]
				arrNum[n] = temp
				temp = arrID[m]
				arrID[m] = arrID[n]
				arrID[n] = temp
			end
		end
	end

	for j=1,3 do					--将已排好序的临时数据前三名保存至数据库中
		act.SetActScriptData(56, 1, j, arrID[j])
		act.SetActScriptData(56, 1, j+4, arrNum[j])
	end


	--进行每日积分排行操做
	--[[
	local arrID2 = {}
	local arrNum2 = {}
	--]]
	local bNewb = 1
	for x=11, 13 do
		arrID[x] = act.GetActScriptData(56, 1, x)
		if arrID[x] == nil then
		    arrID[x] = 0
		end
		if arrID[x] == RoleID  then
			arrNum[x] = i
			bNewb = 0
		else
			arrNum[x] = act.GetActScriptData(56, 1, x+4)
			if arrNum[x] == nil then
				  arrNum[x] = 0
			 end
		end
	end

	if bNewb == 1 then
		arrID[14] = RoleID;
		arrNum[14] = i;
	else
		arrID[14] = 0;
		arrNum[14] = 0;
	end

	for c=11, 13 do
		for v=c+1, 14 do
			if arrNum[v] > arrNum[c] then
				local temp = arrNum[c]
				arrNum[c] = arrNum[v]
				arrNum[v] = temp
				temp = arrID[c]
				arrID[c] = arrID[v]
				arrID[v] = temp
			end
		end
	end

	for h=11, 13 do
		act.SetActScriptData(56, 1, h, arrID[h])
		act.SetActScriptData(56, 1, h+4, arrNum[h])
	end


	act.SaveActScriptData(56)

end












