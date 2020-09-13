--妖精之母刷新（七花之海606，3881，1303），母亲节大使刷新地点（斗战天城700，23510，765）
--活动开始
yaojingzhimu = 0
muqinjiedashi = 0 
Muqinjie_start = 0
function muqinjie_OnTimerMin(actID)
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	if yaojingzhimu == 0 then
		if curyear == 2011 and curmon == 5 and (curday >= 4 and curday <= 17) then
			yaojingzhimu = map.MapCreateCreature(3424073256, -1, 5600055, 606, 3881, 1303)-- 28163   23510   29803
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300049)    --妖精之母已经出现在七花之海的<606, 1303>处，母亲节就此已经开始了，大家要积极参加啊。
			msg.DispatchWorldMsgEvent(MsgID)
			Muqinjie_start = 1
		end
	else
		if curyear == 2011 and curmon == 5 and curday > 17 then
			map.MapDeleteCreature(3424073256, -1, yaojingzhimu)
			Muqinjie_start = 2
		end
	end	
end
aux.RegisterActEvent(39, 4, "muqinjie_OnTimerMin")
--活动结束
function muqinjie_OnEnd(actID)
	if yaojingzhimu ~= 0 then
		map.MapDeleteCreature(3424073256, -1, yaojingzhimu)
		Muqinjie_start = 2
	end
end
aux.RegisterActEvent(39, 3, "muqinjie_OnEnd")
--魔王大宝藏
function c6010043_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	local a = math.random(1,10)
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	if curyear == 2011 and curmon == 5 and (curday >= 4 and curday <= 17) then
		--if Muqinjie_start == 1 then
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, KillerID)
			if TeamID ~= nil and TeamID ~= 4294967295 then
				local Role = {}
				Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
				for i=1, 6 do
					if a > 0 and a <= 6 then
						role.AddRoleItem(MapID, InstanceID, Role[i], 4800087, 2, 0, 0, 402)
					else
						role.AddRoleItem(MapID, InstanceID, Role[i], 4800088, 2, 0, 0, 402)
					end
				end
			elseif TeamID ~= nil then
				if a > 0 and a <= 6 then
					role.AddRoleItem(MapID, InstanceID, KillerID, 4800087, 2, 0, 0, 402)
				else
					role.AddRoleItem(MapID, InstanceID, KillerID, 4800088, 2, 0, 0, 402)
				end
			end
		end
	
end
--注册
aux.RegisterCreatureEvent(6010043, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010044, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010045, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010031, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010032, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010033, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010034, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010035, 4, "c6010043_OnDie")
aux.RegisterCreatureEvent(6010049, 4, "c6010043_OnDie")

--康乃馨调查
function c6030013_On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)	
	if Muqinjie_start == 1 then
		local FreeSize = role.GetBagFreeSize(KillerID)
		if(FreeSize >= 1) then
			role.AddRoleItem(MapID, InstanceID, KillerID, 4800086, 1, 0, 0, 402)
		elseif (FreeSize < 1) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 206483)
		msg.DispatchRoleMsgEvent(KillerID, MsgID)
	end
end
--注册
aux.RegisterCreatureEvent(6030013, 4, "c6030013_On_Die")
--妖精之母对话
function yaojingzhimu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	
	local JT2_num = role.GetRoleItemNum(RoleID, 3050010)--2等精铁数量
	local JT3_num = role.GetRoleItemNum(RoleID, 3050011)--3等精铁数量
	local HTB_num = role.GetRoleItemNum(RoleID, 4800090)--蓝色魂体本源
	local HTG_num = role.GetRoleItemNum(RoleID, 4800089)--绿色魂体本源
	local num =  role.GetRoleItemNum(RoleID, 4800086)--康乃馨
	local Vocation = role.GetRoleClass(RoleID)--玩家职业
	local a = math.random(1,9)
	local b = math.random(1,9)
	local CurTime = tonumber(os.date("%j"))
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["muqin_time"])
	local jingtie_num = role.GetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"])
	if CurTime ~= UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["muqin_time"], CurTime)
	end
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 30 then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206432)		--“妖精之母：\n    轰隆轰隆…… (妖精之母的魂体因为魂体的消散，传出魂体不断瓦解的声音)”
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 4)			
			msg.AddMsgEvent(MsgID, 1, 206433)		-- 修补魂体裂痕
			msg.AddMsgEvent(MsgID, 21, 5)			
			msg.AddMsgEvent(MsgID, 1, 206434)		-- 献给母亲的节日礼物
			msg.AddMsgEvent(MsgID, 21, 18)
			msg.AddMsgEvent(MsgID, 1, 206471)   --"兑换称号<慈母的祝福>"
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)   --"取消"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206435)		--“妖精之母：\n     经过好几次的激烈战斗，斗神们从每个副本中，只能抢回部份的魂体裂痕，因此我们需要大量的精铁才可以将裂痕修补成结晶，如果你拥有玄铁或是精铁请别忘了来我这边进行修补。\n     动作要快，我怕我的魂体会随着时间的流逝而渐渐的瓦解，若是魂体消散，我恐怕就无法复活了”
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 7)			
			msg.AddMsgEvent(MsgID, 1, 100001)		-- 确定
			msg.AddMsgEvent(MsgID, 21, -1)			
			msg.AddMsgEvent(MsgID, 1, 100002)		-- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 7 then
			if jingtie_num < 50 then
				if role.GetBagFreeSize(RoleID) < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 100003) -- "需要至少&lt;p1&gt;格背包空间才可继续操作。"
					msg.AddMsgEvent(MsgID, 9, 1)
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
					msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206436)		--“妖精之母：\n     请选择要使用何种材料进行修补？(3级精铁可兑换蓝色魂体结晶，2级精铁可兑换绿色魂体结晶。每日对多可以兑换50个。)”
					msg.AddMsgEvent(MsgID, 24, TargetID)	
					msg.AddMsgEvent(MsgID, 21, 8)			
					msg.AddMsgEvent(MsgID, 1, 206437)		-- 3级精铁
					msg.AddMsgEvent(MsgID, 21, 9)			
					msg.AddMsgEvent(MsgID, 1, 206438)		-- 2级精铁
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			elseif jingtie_num >= 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206482)		--“今天已经兑换50个了，请明天再来兑换，你还可以通过进入副本获得魂体结晶。”
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- 确定
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 8 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206439)		--“妖精之母：\n    请选择你想要兑换的蓝色魂体结晶！(每6个3级精铁可兑换1个蓝色魂体结晶)”
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 10)			
			msg.AddMsgEvent(MsgID, 1, 206440)		--  兑换1个蓝色魂体结晶
			msg.AddMsgEvent(MsgID, 21, 11)			
			msg.AddMsgEvent(MsgID, 1, 206441)		--  兑换3个蓝色魂体结晶
			msg.AddMsgEvent(MsgID, 21, 12)			
			msg.AddMsgEvent(MsgID, 1, 206442)		--  兑换5个蓝色魂体结晶
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 10 then
			if jingtie_num < 50 then
				if JT3_num < 6 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206443) --  3级精铁不足，兑换失败！
					msg.AddMsgEvent(MsgID, 21, -1)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050011, 6, 101)--删除3级精铁
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800088, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+1)
				end
			elseif jingtie_num >= 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206482)		--“今天已经兑换50个了，请明天再来兑换，你还可以通过进入副本获得魂体结晶。”
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- 确定
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 11 then
			if jingtie_num <= 47 then
				if JT3_num < 18 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206443) --  3级精铁不足，兑换失败！
					msg.AddMsgEvent(MsgID, 21, -1)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050011, 18, 101)--删除3级精铁
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800088, 3, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+3)
				end
			elseif jingtie_num > 47 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206484)		--你已经兑换了&lt;p1&gt;个，最多只能兑换50个，请选择少量的兑换方法
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 9, jingtie_num)
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- 确定
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 12 then
			if jingtie_num <= 45 then
				if JT3_num < 30 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206443) --  3级精铁不足，兑换失败！
					msg.AddMsgEvent(MsgID, 21, -1)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050011, 30, 101)--删除3级精铁
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800088, 5, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+5)
				end
			elseif jingtie_num > 45 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206484)		--你已经兑换了&lt;p1&gt;个，最多只能兑换50个，请选择少量的兑换方法
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 9, jingtie_num)
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- 确定
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 9 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206444)		--“妖精之母：\n    请选择你想要兑换的绿色魂体结晶！(每6个2级精铁可兑换1个绿色魂体结晶)”
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 13)			
			msg.AddMsgEvent(MsgID, 1, 206445)		--  兑换1个绿色魂体结晶
			msg.AddMsgEvent(MsgID, 21, 14)			
			msg.AddMsgEvent(MsgID, 1, 206446)		--  兑换3个绿色魂体结晶
			msg.AddMsgEvent(MsgID, 21, 15)			
			msg.AddMsgEvent(MsgID, 1, 206447)		--  兑换5个绿色魂体结晶
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 13 then
			if jingtie_num < 50 then
				if JT2_num < 6 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206448) --  2级精铁不足，兑换失败！
					msg.AddMsgEvent(MsgID, 21, -1)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050010, 6, 101)--删除2级精铁
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800087, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+1)
				end
			elseif jingtie_num >= 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206482)		--“今天已经兑换50个了，请明天再来兑换，你还可以通过进入副本获得魂体结晶。”
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- 确定
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 14 then
			if jingtie_num <= 47 then
				if JT2_num < 18 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206448) --  2级精铁不足，兑换失败！
					msg.AddMsgEvent(MsgID, 21, -1)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050010, 18, 101)--删除2级精铁
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800087, 3, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+3)
				end
			elseif jingtie_num > 47 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206484)		--你已经兑换了&lt;p1&gt;个，最多只能兑换50个，请选择少量的兑换方法
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 9, jingtie_num)
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- 确定
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 15 then
			if jingtie_num <= 45 then
				if JT2_num < 30 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206448) --  2级精铁不足，兑换失败！
					msg.AddMsgEvent(MsgID, 21, -1)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3050010, 30, 101)--删除2级精铁
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800087, 5, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["muqinyaojing"], jingtie_num+5)
				end
			elseif jingtie_num > 45 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206484)		--你已经兑换了&lt;p1&gt;个，最多只能兑换50个，请选择少量的兑换方法
				msg.AddMsgEvent(MsgID, 24, TargetID)	
				msg.AddMsgEvent(MsgID, 9, jingtie_num)
				msg.AddMsgEvent(MsgID, 21, -1)			
				msg.AddMsgEvent(MsgID, 1, 100001)		-- 确定
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206449)		--“妖精之母：\n    1个蓝色魂体本源可以兑换3张绑定的保底符，30级以上无限次数兑换\n    1个绿色魂体本源可以兑换1张绑定的保底符，30级以上无限次数兑换”
			msg.AddMsgEvent(MsgID, 24, TargetID)	
			msg.AddMsgEvent(MsgID, 21, 16)			
			msg.AddMsgEvent(MsgID, 1, 206450)		--   兑换蓝色魂体本源
			msg.AddMsgEvent(MsgID, 21, 17)			
			msg.AddMsgEvent(MsgID, 1, 206451)		--   兑换绿色魂体本源
			msg.AddMsgEvent(MsgID, 21, -1)	-- 选项一
			msg.AddMsgEvent(MsgID, 1, 100002)	-- "取消"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 16 then
			if HTB_num < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206452) --  您缺少蓝色魂体本源，兑换失败！
				msg.AddMsgEvent(MsgID, 21, -1)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif role.GetBagFreeSize(RoleID) < 3 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 100003) -- "需要至少&lt;p1&gt;格背包空间才可继续操作。"
				msg.AddMsgEvent(MsgID, 9, 3)
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800090, 1, 101)	
				role.AddRoleItem(MapID, InstanceID, RoleID, 3020057, 3, -1, 8, 420)--保底符
			end
		elseif TalkIndex == 17 then
			if HTG_num < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206453) --  您缺少绿色魂体本源，兑换失败！
				msg.AddMsgEvent(MsgID, 21, -1)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif role.GetBagFreeSize(RoleID) < 3 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 100003) -- "需要至少&lt;p1&gt;格背包空间才可继续操作。"
				msg.AddMsgEvent(MsgID, 9, 3)
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800089, 1, 101)	
				role.AddRoleItem(MapID, InstanceID, RoleID, 3020057, 1, -1, 8, 420)--保底符
			end
		elseif TalkIndex == 18 then
			if num < 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206472) -- "妖精之母：\n    你身上的红色康乃馨可不够哦，集齐5个再来吧。"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			elseif role.GetBagFreeSize(RoleID) < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 100003) -- "需要至少&lt;p1&gt;格背包空间才可继续操作。"
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800086, 5, 101)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800092, 1, -1, 8, 420)
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206476) -- 您等级不足30级，无法参加此活动。
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
		msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	end
end
aux.RegisterCreatureEvent(5600055, 7, "yaojingzhimu_OnTalk")
