--s52,3373741864
--1、巧克力对话次数
--2、与玫瑰对话次数
qingrenjie_chuansongren = 0 -- 记录情人节传送人

--传送博博罗曼村
function luomancun_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206501)		--“博博罗曼村传送人：\n博博罗曼村是男男女女们向往的浪漫之地，那里需要男女配合才能达成目标，你要进入博博罗曼村吗？ \n每周二、周四、周六、周日18：00~24：00开放,不要错过时间。”
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 206502)		--“是的,我要进入博博罗曼村”
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local sex = role.GetRoleSex(RoleID)
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 30 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206503)		--博博罗曼村传送人：\n    只有超过30级的玩家才能进入博博罗曼村。
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if sex == 1 then
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373741864, 105, 3000, 369)--男区 105, 3000, 369

			elseif sex == 0 then
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373741864, 411, 2955, 314)--女区411，2955，314

			end
		end
	end
end
aux.RegisterCreatureEvent(5600026, 7, "luomancun_OnTalk")
--区域判断
function Nannv_OnEnterArea(MapID, InstanceID, RoleID, objID)
	local sex = role.GetRoleSex(RoleID)
	if objID == 500 then--女区

		if sex == 1 then
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206528)--这边不符合您的身份，这是属于女士的。
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 251, 3016, 215)--中间区域 251, 3016, 215
		end
	elseif objID == 501 then--男区

		if sex == 0 then
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206516)--这边不符合您的身份，这是属于男士的。
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 251, 3016, 215)--中间区域
		end
	end
end
aux.RegisterMapEvent( "s52", 7, "Nannv_OnEnterArea")
--出副本
function s52_OnPlayerEnter(MapID, InstanceID,RoleID)
	local Q_wupin = {}
	--情人节物品
	Q_wupin[1] = {id = 4800062}
	Q_wupin[2] = {id = 4800063}
	Q_wupin[3] = {id = 4800064}
	Q_wupin[4] = {id = 4800065}	
	Q_wupin[5] = {id = 4800066}
	Q_wupin[6] = {id = 4800067}	
	Q_wupin[7] = {id = 4800068}
	Q_wupin[8] = {id = 4800069}	
	Q_wupin[9] = {id = 4800070}
	Q_wupin[10] = {id = 4800071}
	for i=1,10 do
		local num = role.GetRoleItemNum(RoleID, Q_wupin[i].id)
		role.RemoveFromRole(MapID, InstanceID, RoleID, Q_wupin[i].id, num, 101)
	end
end
--注册
aux.RegisterMapEvent("s52", 3, "s52_OnPlayerEnter")
--离开情人节
function Qchukou_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206548)		--“斗战天城传送人：\n    你要离开博博罗曼村吗？\n    离开这里<I>雕刻道具<I>、</I>除虫剂<I>、</I>天神露<I>、</I>高级巧克力粉<I>、</I>巧克力模具<I>、</I>高级除虫剂<I>、</I>冰晶玫瑰<I>、</I>燃炎玫瑰<I>、</I>零度巧克力<I>、</I>精灵巧克力</I>都将被删除。”
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
aux.RegisterCreatureEvent(5600034, 7, "Qchukou_OnTalk")
--玫瑰种植
function meigui_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local sex = role.GetRoleSex(RoleID)
	local growtime = role.GetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"])--与玫瑰对话次数
	local buff = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,60005)--45s buff ID
	local a = math.random(1, 100)
	local laodong_num = role.GetRoleItemNum(RoleID, 4800076)--劳动NUM
	local day = os.date("%A")
	local curhour = tonumber(os.date("%H"))
	if (day == "Tuesday" or day == "Thursday" or day == "Saturday" or day == "Sunday") then
		if (curhour >= 18 and curhour <= 24) then
			if sex == 1 then
				if buff ~= true then
					if laodong_num > 0 then
						if TalkIndex == -1 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 206504)	-- "您确定要照顾玫瑰花么？"
							msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
							msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
							msg.AddMsgEvent(MsgID, 21, 5)	-- 选项二
							msg.AddMsgEvent(MsgID, 1, 100002)	-- "取消"
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						elseif TalkIndex == 4 then	
							if growtime < 5 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206505)	-- "照顾玫瑰花需要消耗1份【甘露】或1份【天神露】，两次浇水之间需间隔45秒；\n【天神露】的效果是【甘露】的5倍！"
								msg.AddMsgEvent(MsgID, 21, 6)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 206506)	-- "使用【甘露】"
								msg.AddMsgEvent(MsgID, 21, 7)	-- 选项二
								msg.AddMsgEvent(MsgID, 1, 206507)	-- "使用【天神露】"
								msg.AddMsgEvent(MsgID, 24, TargetID)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							elseif growtime >= 5 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206508)	-- "照顾玫瑰花需要消耗1份【除虫剂】或1份【高级除虫剂】，两次浇水之间需间隔45秒；\n【高级除虫剂】的效果是【除虫剂】的5倍！"
								msg.AddMsgEvent(MsgID, 21, 8)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 206509)	-- "使用【除虫剂】"
								msg.AddMsgEvent(MsgID, 21, 9)	-- 选项二
								msg.AddMsgEvent(MsgID, 1, 206510)	-- "使用【高级除虫剂】"
								msg.AddMsgEvent(MsgID, 24, TargetID)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							end
						elseif TalkIndex == 6 then
							local QNum = role.GetRoleItemNum(RoleID, 4800060)--【甘露】数量
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206511) --"您没有【甘露】，无法对玫瑰花进行灌溉"
								msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800060, 1, 101)---------删除【甘露】
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420) --冰晶玫瑰
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300032)--XX获得冰晶玫瑰，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420) --燃炎玫瑰
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300033)--XX获得燃炎玫瑰，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --罗曼之星
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300036)--XX获得罗曼之星，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800052, 1, -1, 8, 420) --真爱玫瑰
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 2000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 4000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 5000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 8000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 15000)
									end
									growtime = growtime + 1
									if growtime == 5 then
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800062, 5, -1, 8, 420)--雕刻器具*3
										elseif a > 95 then
											local b = math.random(1, 10)
											if b <= 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800065, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--注册广播句柄
												msg.AddMsgEvent(MsgID,102,300030)--<p1>获得高级巧克力粉，在此祝贺
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											elseif b > 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800066, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--注册广播句柄
												msg.AddMsgEvent(MsgID,102,300031)--<p1>获得巧克力模具，在此祝贺
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											end
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--记录制作次数
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 7 then
							local QNum = role.GetRoleItemNum(RoleID, 4800064)--【天神露】数量
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206512) --"您没有【天神露】，无法对玫瑰花进行灌溉"
								msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800064, 1, 101)---------删除天神露
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420) --冰晶玫瑰
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300032)--XX获得冰晶玫瑰，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420) --燃炎玫瑰
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300033)--XX获得燃炎玫瑰，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --罗曼之星
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300036)--XX获得罗曼之星，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800052, 1, -1, 8, 420) --真爱玫瑰
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 14000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 28000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 35000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 56000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 70000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 105000)
									end
									growtime = growtime + 5
									if growtime >= 5 then
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800062, 5, -1, 8, 420)--雕刻器具*3
										elseif a > 95 then
											local b = math.random(1, 10)
											if b <= 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800065, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--注册广播句柄
												msg.AddMsgEvent(MsgID,102,300030)--<p1>获得高级巧克力粉，在此祝贺
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											elseif b > 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800066, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--注册广播句柄
												msg.AddMsgEvent(MsgID,102,300031)--<p1>获得巧克力模具，在此祝贺
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											end
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--记录制作次数
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 8 then
							local QNum = role.GetRoleItemNum(RoleID, 4800063)--【除虫剂】数量
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206513) --"您没有【除虫剂】，无法为玫瑰花进行除虫"
								msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800063, 1, 101)---------删除除虫剂
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420) --冰晶玫瑰
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300032)--XX获得冰晶玫瑰，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420) --燃炎玫瑰
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300033)--XX获得燃炎玫瑰，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --罗曼之星
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300036)--XX获得罗曼之星，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800052, 1, -1, 8, 420) --真爱玫瑰
									elseif c > 27 and c <= 37 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420) --罗曼徽章 
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 3000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 6000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 12000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 20000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 25000)
									end
									growtime = growtime + 1
									if growtime >= 10 then
										role.RemoveFromRole(MapID, InstanceID, RoleID, 4800076, 1, 101)--删除劳动证
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											growtime = 0
										elseif a > 95 and a <= 99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--注册广播句柄
											msg.AddMsgEvent(MsgID,102,300032)--XX获得冰晶玫瑰，在此祝贺
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										elseif a >99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--注册广播句柄
											msg.AddMsgEvent(MsgID,102,300033)--XX获得燃炎玫瑰，在此祝贺
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--记录制作次数
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 9 then
							local QNum = role.GetRoleItemNum(RoleID, 4800067)--【高级除虫剂】数量
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206514) --"您没有【高级除虫剂】，无法为玫瑰花进行除虫"
								msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800067, 1, 101)---------删除高级除虫剂
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420) --冰晶玫瑰
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300032)--XX获得冰晶玫瑰，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420) --燃炎玫瑰
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300033)--XX获得燃炎玫瑰，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --罗曼之星
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300036)--XX获得罗曼之星，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800052, 1, -1, 8, 420) --真爱玫瑰
									elseif c > 27 and c <= 37 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420) --罗曼徽章 
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 21000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 42000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 70000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 84000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 140000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 175000)
									end
									growtime = growtime + 5
									if growtime >= 10 then
										role.RemoveFromRole(MapID, InstanceID, RoleID, 4800076, 1, 101)--删除劳动证
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											growtime = 0
										elseif a > 95 and a <= 99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800068, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--注册广播句柄
											msg.AddMsgEvent(MsgID,102,300032)--XX获得冰晶玫瑰，在此祝贺
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										elseif a >99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800075, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800069, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--注册广播句柄
											msg.AddMsgEvent(MsgID,102,300033)--XX获得燃炎玫瑰，在此祝贺
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--记录制作次数
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						end
					else
						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
							msg.AddMsgEvent(QuestID1, 1, 206549)--您身上没有罗曼村劳动证，不能进行操作。
							msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206515) --距下一次的时间还没有到，请到时间再来。
					msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					
				end
			elseif sex == 0 then
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206516)--这边不符合您的身份，这是属于男士的。
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			end
		else
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 206567)--博博罗曼村的所有劳动只能在每周二、四、六、日的18:00——24:00进行，其他时间博博罗曼村是需要休息的。
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206567)--博博罗曼村的所有劳动只能在每周二、四、六、日的18:00——24:00进行，其他时间博博罗曼村是需要休息的。
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
	end
end
aux.RegisterCreatureEvent(6030010, 7, "meigui_OnTalk")
--巧克力制作
function qiaokeli_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local sex = role.GetRoleSex(RoleID)
	local growtime = role.GetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"])--巧克力对话次数
	local buff = role.IsRoleHaveBuff(MapID, InstanceID,RoleID, 60005)--45s buff ID
	local a = math.random(1, 100)
	local laodong_num = role.GetRoleItemNum(RoleID, 4800076)--劳动NUM
	local day = os.date("%A")
	local curhour = tonumber(os.date("%H"))
	if (day == "Tuesday" or day == "Thursday" or day == "Saturday" or day == "Sunday") then
		if (curhour >= 18 and curhour <= 24) then
			if sex == 0 then
				if buff ~= true then
					if laodong_num > 0 then
						if TalkIndex == -1 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 206517)	-- "您确定要制作巧克力么？"
							msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
							msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
							msg.AddMsgEvent(MsgID, 21, 5)	-- 选项二
							msg.AddMsgEvent(MsgID, 1, 100002)	-- "取消"
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						elseif TalkIndex == 4 then
							if  growtime < 5 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206518)	-- "制造巧克力需要消耗1份【巧克力粉】或1份【高级巧克力粉】，两次制作之间需间隔45秒；\n【高级巧克力粉】的效果是【巧克力粉】的5倍！"
								msg.AddMsgEvent(MsgID, 21, 6)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 206519)	-- "使用【巧克力粉】"
								msg.AddMsgEvent(MsgID, 21, 7)	-- 选项二
								msg.AddMsgEvent(MsgID, 1, 206520)	-- "使用【高级巧克力粉】"
								msg.AddMsgEvent(MsgID, 24, TargetID)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							elseif growtime >= 5 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206521)	-- "制造巧克力需要消耗1份【雕刻道具】或1份【巧克力模具】，两次造型之间需间隔45秒；\n【巧克力模具】的效果是【雕刻道具】的5倍！"
								msg.AddMsgEvent(MsgID, 21, 8)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 206522)	-- "使用【雕刻道具】"
								msg.AddMsgEvent(MsgID, 21, 9)	-- 选项二
								msg.AddMsgEvent(MsgID, 1, 206523)	-- "使用【巧克力模具】"
								msg.AddMsgEvent(MsgID, 24, TargetID)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							end
						elseif TalkIndex == 6 then
							local QNum = role.GetRoleItemNum(RoleID, 4800061)--【巧克力粉】数量
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206524) --"您没有【巧克力粉】，无法进行巧克力的制作"
								msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800061, 1, 101)---------删除巧克力粉
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420) --零度巧克力
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300034)--XX获得零度巧克力，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420) --精灵巧克力
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300035)--XX获得精灵巧克力，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --罗曼之星
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300036)--XX获得罗曼之星，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800051, 1, -1, 8, 420) --真爱巧克力
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 2000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 4000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 5000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 8000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 15000)
									end
									growtime = growtime + 1
									if growtime == 5 then
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800063, 5, -1, 8, 420)--除虫剂*3
										elseif a > 95 then--高级除虫剂*1或天神露*1
											local b = math.random(1, 10)
											if b > 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800067, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--注册广播句柄
												msg.AddMsgEvent(MsgID,102,300046)--XX获得高级除虫剂，在此祝贺
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											elseif b <= 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800064, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--注册广播句柄
												msg.AddMsgEvent(MsgID,102,300045)--XX获得天神露，在此祝贺
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											end
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--记录制作次数
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 7 then
							local QNum = role.GetRoleItemNum(RoleID, 4800065)--【高级巧克力粉】数量
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206525) --"您没有【高级巧克力粉】，无法进行巧克力的制作"
								msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800065, 1, 101)---------删除巧克力粉
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420) --零度巧克力
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300034)--XX获得零度巧克力，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420) --精灵巧克力
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300035)--XX获得精灵巧克力，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --罗曼之星
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300036)--XX获得罗曼之星，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800051, 1, -1, 8, 420) --真爱巧克力
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 14000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 28000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 35000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 56000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 70000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 105000)
									end
									growtime = growtime + 5
									if growtime >= 5 then
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800063, 5, -1, 8, 420)--除虫剂*3
										elseif a > 95 then--高级除虫剂*1或天神露*1
											local b = math.random(1, 10)
											if b > 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800067, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--注册广播句柄
												msg.AddMsgEvent(MsgID,102,300046)--XX获得高级除虫剂，在此祝贺
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											elseif b <= 1 then
												role.AddRoleItem(MapID, InstanceID, RoleID, 4800064, 1, -1, 8, 420)
												local MsgID = msg.BeginMsgEvent();	--注册广播句柄
												msg.AddMsgEvent(MsgID,102,300045)--XX获得天神露，在此祝贺
												msg.AddMsgEvent(MsgID, 2, RoleID)
												msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											end
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--记录制作次数
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 8 then
							local QNum = role.GetRoleItemNum(RoleID, 4800062)--【雕刻道具】数量
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206526) --"您没有【雕刻道具】，无法为巧克力造型"
								msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800062, 1, 101)---------删除巧克力粉
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420) --零度巧克力
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300034)--XX获得零度巧克力，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420) --精灵巧克力
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300035)--XX获得精灵巧克力，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --罗曼之星
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300036)--XX获得罗曼之星，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800051, 1, -1, 8, 420) --真爱巧克力
									elseif c > 27 and c <= 37 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420) --罗曼徽章 
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 3000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 6000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 12000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 20000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 25000)
									end
									growtime = growtime + 1
									if growtime >= 10 then
										role.RemoveFromRole(MapID, InstanceID, RoleID, 4800076, 1, 101)--删除劳动证
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											growtime = 0
										elseif a > 95 and a <= 99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--注册广播句柄
											msg.AddMsgEvent(MsgID,102,300034)--XX获得零度巧克力，在此祝贺
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										elseif a >99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--注册广播句柄
											msg.AddMsgEvent(MsgID,102,300035)--XX获得精灵巧克力，在此祝贺
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--记录制作次数
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						elseif TalkIndex == 9 then
							local QNum = role.GetRoleItemNum(RoleID, 4800066)--【巧克力模具】数量
							if QNum <= 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 20, 206527) --"您没有【巧克力模具】，无法为巧克力造型"
								msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
								msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								local FreeSize = role.GetBagFreeSize(RoleID)
								if FreeSize >= 3 then
									unit.AddBuff(MapID, InstanceID, RoleID, 6000501, RoleID)--45sbuff
									role.RemoveFromRole(MapID, InstanceID, RoleID, 4800066, 1, 101)---------删除巧克力粉
									local c = math.random(1, 1000)
									if c <= 10 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420) --零度巧克力
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300034)--XX获得零度巧克力，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 10 and c <= 15 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420) --精灵巧克力
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300035)--XX获得精灵巧克力，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 15 and c <= 17 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800073, 1, -1, 8, 420) --罗曼之星
										local MsgID = msg.BeginMsgEvent();	--注册广播句柄
										msg.AddMsgEvent(MsgID,102,300036)--XX获得罗曼之星，在此祝贺
										msg.AddMsgEvent(MsgID, 2, RoleID)
										msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
									elseif c > 17 and c <= 27 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800051, 1, -1, 8, 420) --真爱巧克力
									elseif c > 27 and c <= 37 then
										role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420) --罗曼徽章 
									end
									local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
									if level >= 30 and level <= 39 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 21000)
									elseif level >= 40 and level <= 49 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 42000)
									elseif level >= 50 and level <= 59 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 70000)
									elseif level >= 60 and level <= 69 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 84000)
									elseif level >= 70 and level <= 79 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 140000)
									elseif level >= 80 then
										role.AddRoleExp(MapID, InstanceID, RoleID, 175000)
									end
									growtime = growtime + 5
									if growtime >= 10 then
										role.RemoveFromRole(MapID, InstanceID, RoleID, 4800076, 1, 101)--删除劳动证
										if a <= 95 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											growtime = 0
										elseif a > 95 and a <= 99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800070, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--注册广播句柄
											msg.AddMsgEvent(MsgID,102,300034)--XX获得零度巧克力，在此祝贺
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										elseif a >99 then
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800074, 1, -1, 8, 420)
											role.AddRoleItem(MapID, InstanceID, RoleID, 4800071, 1, -1, 8, 420)
											local MsgID = msg.BeginMsgEvent();	--注册广播句柄
											msg.AddMsgEvent(MsgID,102,300035)--XX获得精灵巧克力，在此祝贺
											msg.AddMsgEvent(MsgID, 2, RoleID)
											msg.DispatchBroadcast(MsgID,3373741864,-1,-1);
											growtime = 0
										end
									end
									role.SetRoleScriptData(RoleID, 1, RoleDataType["Rose_Times"], growtime)--记录制作次数
								else
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 26, 100003)
									msg.AddMsgEvent(MsgID, 9, 3)
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
								end
							end
						end
					else
						local QuestID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(QuestID1, 13, 0)
						msg.AddMsgEvent(QuestID1, 1, 206549)--您身上没有罗曼村劳动证，不能进行操作。
						msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206515) --距下一次的时间还没有到，请到时间再来。
					msg.AddMsgEvent(MsgID, 21, 5)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100002)	-- "离开"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			elseif sex == 1 then
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206528)--这边不符合您的身份，这是属于女士的。
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			end
		else
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 206567)--博博罗曼村的所有劳动只能在每周二、四、六、日的18:00——24:00进行，其他时间博博罗曼村是需要休息的。
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206567)--博博罗曼村的所有劳动只能在每周二、四、六、日的18:00——24:00进行，其他时间博博罗曼村是需要休息的。
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
	end
end
aux.RegisterCreatureEvent(6030011, 7, "qiaokeli_OnTalk")
