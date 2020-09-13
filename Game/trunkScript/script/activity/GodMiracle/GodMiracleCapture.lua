-------------------------------------------------------------------------------
-- Copyright (c) 2004 TENGWU Entertainment All rights reserved.
-- filename: GodMiracleCpatureDefine.h
-- author: chdeng
-- actor:
-- data: 2010-01-25
-- last:
-- brief: 神迹争夺结构定义
-------------------------------------------------------------------------------

--神迹争夺活动ID
ActiveID = 25


-- \breif: 所有地图初始化时,设置神迹关联状态，并刷新指定点怪物
function GodMiracleCpature_OnActiveStart(ActiveID)
	if OpenGodMiracleFlag == 0 then
		return
	end
	
	InitGodMiracleOpenFlag()

	InitAllGodMiracle()
end


ResultbroadcastFlag = 0

CalFlag = 0

-- 是否已经结束过标志
FinishedFlag = 0

-- \breif : 地图适时更新函数
--  param1: 活动ID
--  param2：传入时间
--  ramark: 依据时间适时更新神迹的状态
function GodMiracleCpature_OnActiveTimer(ActiveID, Sec)

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- 内层适时更新操作
	BroadcastGodMsgTick()

	-- 外层适时更新操作
	guild.UpdateGodMiracle()

	---------------------------------------------------------------------------------------------------- 时间到结束战斗
	--检测时迹，结束战斗
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec   = tonumber(os.date("%S"))


	if Hour == 21 and (Min == 0 or Min == 1) and FinishedFlag == 0 then
		CalFlag = 1
	end

	if  FinishedFlag == 1 and Hour == 21 and Min > 1 then
		FinishedFlag = 0
	end

	-- 若时间到30秒,记录一次积分
	for k,v in pairs(ClanGodMiracle_CurInfo) do
		---------------------------------------------------------------------------------------------------- 帮派解散
		-- 休战状态检查帮派是否解散
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
		if godCurState ==  1 then

			local GuildIDDiss = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
			if GuildIDDiss~= nil and GuildIDDiss ~= 0 then

				-- 查看占领者帮派是否解散
				if guild.IsHaveGuild(GuildIDDiss) == false then

					local CaptureGodNum = 0
					-- 查看本帮派占领了多少个神迹
					for k1, v in pairs(ClanGodMiracle_CurInfo) do
						if GetDefendGuildID(ClanGodMiracle_CurInfo[k1].MapID) == GuildIDDiss then
							CaptureGodNum = CaptureGodNum + 1

							-- 针对世界广播,神迹坐标可争夺
							-- ###（神迹名称）保护期已过，可被各大帮派进行争夺
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 13, 1)
							msg.AddMsgEvent(MsgID, 1, 1409)
							msg.AddMsgEvent(MsgID, 5 , ClanGodMiracle_CurInfo[k1].GdMiracleID)
							msg.DispatchWorldMsgEvent(MsgID)

							-- 帮派解散，柱子变成银色
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Attack_BuffID.God)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Attack_BuffID.People)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Attack_BuffID.Monster)

							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Defance_BuffID.God)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Defance_BuffID.People)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Defance_BuffID.Monster)

							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Rest_BuffID.God)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Rest_BuffID.People)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Rest_BuffID.Monster)

							unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1)
							unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2)
							unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3)

							-- 针对C++和Lua做清理工作
							guild.InitGodMiracle(k1-1)
							ClearGodMiracle(k1)

							-- 设置神迹为未占领状态
							guild.SetGodCurState(k1-1, 0)


						end
					end

					-- 帮派解散,移除所有在线玩家的增益BUFF
					-- 得到帮派的所有成员
					--[[local tmpRoleIDTab = guild.GetAllGuildMember(GuildIDDiss)
					if tmpRoleIDTab ~= nil then
						for k, v in pairs(tmpRoleIDTab) do
							if (RoleTable[k] ~= nil) then
								unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, tmpRoleIDTab[k], GetBuffIDbyGodMiracleNum(CaptureGodNum))
							end
						end
					end--]]

--					guild.AddBuffToAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), GetBuffIDbyGodMiracleNum(CaptureGodNum))
				end
			end
		end

		---------------------------------------------------------------------------------------------------- 检测战斗是否结束
		-- 非争夺状态不做处理
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
		if godCurState == 2  then

			--BUG
			--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

				------------------------------------------------------------------------------------------------- 30争夺双方积分
				if CalFlag == 0 then
					ModTick(GetCurInfoMapID(k), 1)
					if  Gettick(GetCurInfoMapID(k)) == GodEnum.StdScoresTick then
						-- 守方
						local pillarNum = GuildCapturePillar(GetCurInfoMapID(k), GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))
						if pillarNum > 0 then
							ModScores(GetCurInfoMapID(k), GodEnum.Defend, pillarNum)
							guild.SetGodGuildPoint(k-1, GodEnum.Defend, pillarNum)
						end

						-- 攻方
						local pillarNum1 = GuildCapturePillar(GetCurInfoMapID(k), GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))
						if pillarNum1 > 0 then
							ModScores(GetCurInfoMapID(k), GodEnum.Capture, pillarNum1)
							guild.SetGodGuildPoint(k-1,  GodEnum.Capture, pillarNum1)
						end
						SetTick(ClanGodMiracle_CurInfo[k].MapID, 0)
					end
				end

				if CalFlag == 1  then
					--  无争夺特殊处理
					if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) == 0 then

						-- 记录战斗结果
						Capture_Result[k].WinOrLoseFlag = 0
						Capture_Result[k].DefendGuild   = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
						Capture_Result[k].CaptureGuild  = 0

						-- 设置处于空闲状态
						guild.SetGodCurState(k-1, 1)
						if GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID) == 0 then
							guild.SetGodCurState(k-1, 0)
						end

						-- 双方帮派解除战斗状态
						guild.UnsetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
						guild.UnsetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)

						aux.WriteLog("begin:Cal(Capture==0 ):CalCaptureContributionAndExploit")
						--外层贡献和功勋计算
						guild.CalCaptureContributionAndExploit(k-1, GodEnum.Defend)
						aux.WriteLog("end:Cal(Capture==0 ):CalCaptureContributionAndExploit")

						-- 保住神迹,不添加任何BUFF
						-- 本帮派成功的捍卫了自己的神迹！
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 1430)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

						-- 守方同盟
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 1430)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchAllianceGuildMsgEvent(MsgID, 1, k-1)

						--ResetGodMiracle(k)

					elseif GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

						if  GetScores(GetCurInfoMapID(k), GodEnum.Defend) >= GetScores(GetCurInfoMapID(k), GodEnum.Capture) then -- 守方胜

							-- 记录战斗结果
							Capture_Result[k].WinOrLoseFlag = 1
							Capture_Result[k].DefendGuild   = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
							Capture_Result[k].CaptureGuild  = GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID)

							-- 设置处于空闲状态
							guild.SetGodCurState(k-1, 1)
							aux.WriteLog("defend win: SetGodCurState(k-1, 1)\r\n")

							-- 双方帮派解除战斗状态
							guild.UnsetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							guild.UnsetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							aux.WriteLog("defend win: UnsetGuildStatus\r\n")

							--外层贡献和功勋计算
							aux.WriteLog("begin:Cal(Defend Win):CalCaptureContributionAndExploit\r\n")
							guild.CalCaptureContributionAndExploit(k-1, GodEnum.Defend)
							aux.WriteLog("end:Cal(Defend Win):CalCaptureContributionAndExploit\r\n")

							-- 保住神迹,不添加任何BUFF
							-- 本帮派成功的捍卫了自己的神迹！
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 13, 1)
							msg.AddMsgEvent(MsgID, 1, 1430)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

							-- 守方同盟
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 13, 1)
							msg.AddMsgEvent(MsgID, 1, 1430)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchAllianceGuildMsgEvent(MsgID, 1, k-1)

							--ResetGodMiracle(k)

						elseif GetScores(GetCurInfoMapID(k), GodEnum.Defend) < GetScores(GetCurInfoMapID(k), GodEnum.Capture)  then -- 攻方胜

							-- 记录战斗结果
							Capture_Result[k].WinOrLoseFlag = 2
							Capture_Result[k].DefendGuild   = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
							Capture_Result[k].CaptureGuild  = GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID)

							-- 设置处于空闲状态
							guild.SetGodCurState(k-1, 1)
							aux.WriteLog("capture win: SetGodCurState(k-1, 1)\r\n")

							-- 双方帮派解除战斗状态
							guild.UnsetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							guild.UnsetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							aux.WriteLog("capture win: UnsetGuildStatus\r\n")

							--外层贡献和功勋计算
							aux.WriteLog("begin:Cal(Capture Win):CalCaptureContributionAndExploit\r\n")
							guild.CalCaptureContributionAndExploit(k-1, GodEnum.Capture)
							aux.WriteLog("end:Cal(Capture Win):CalCaptureContributionAndExploit\r\n")

							-- 初始数据
							SetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID, GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))
							SetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID, 0)

							-- 本帮通过艰苦的争夺获得了××神迹的占领权，所有成员获得神佑状态！同时大家可前往神迹购买所需物品
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 102, 100055)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

							-- 攻方同盟
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 102, 100055)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchAllianceGuildMsgEvent(MsgID, 2, k-1)

							--ResetGodMiracle(k)

						end
					end

					-- 不论哪方胜,将柱子设置为绿色
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Attack_BuffID.God)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Attack_BuffID.People)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Attack_BuffID.Monster)

					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Defance_BuffID.God)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Defance_BuffID.People)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Defance_BuffID.Monster)

					-- 取消玩家战场状态
					--	ES_CSAttack		=	8,	// 城战进攻阵营
					--	ES_CSDefence	=	9,	// 城战防御阵营
					--	EGCT_Defend	= 1,	// 防御
					--	EGCT_Capture= 2,	// 争夺
					guild.UnsetStateOfAllRoleInBattleField(1, 9, GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
					guild.UnsetStateOfAllRoleInBattleField(2, 8, GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))

					if GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0  then

						unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God)
						unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People)
						unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster)

						unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Rest_BuffID.God, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1)
						unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Rest_BuffID.People, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2)
						unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Rest_BuffID.Monster, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3)

					end

					aux.WriteLog("Setting Pillar corlor\r\n")

					ResetGodMiracle(k)

					ClearBroaCastGlag()
					ResultbroadcastFlag = 1

				end
			--end
		end
	end

	if ResultbroadcastFlag == 1 then
		aux.WriteLog("Broadcast cal result!\r\n")
		CalFlag = 0
		FinishedFlag	= 1
		BroadcastCaptureResult()
		ResultbroadcastFlag  = 0
		EnterFlag = 0

	end
end

-- \breif : 活动每分钟刷新
--  param1: 活动ID
function GodMiracleCpature_OnActiveTimeMin(ActiveID)

	if OpenGodMiracleFlag == 0 then
		return
	end
	--  更新所有神迹,记录活跃时间
	guild.UpdateGodMiracleMin()

	BroadcastGodMsgMin()
end


-- \breif : 玩家被杀
--  param1:
function GodMiracleCpature_OnRoleDie(MapID, InstanceID, BeKilledID, KillerID)

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- 地图限制判断
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- 非争夺状态不做处理
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2  and GetCaptureGuildID(MapID) ~= 0 then
		return
	end

	-- 得到杀人者帮派ID
	local killerGuildID = guild.GetRoleGuildID(KillerID)
	if killerGuildID ~= 4294967295 then
		local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, killerGuildID)
		if DOrCFlag ~= GodEnum.Null then
			guild.SetGuildRoleDie(GetGodMiracleIDMinOne(MapID), KillerID, BeKilledID, DOrCFlag)
		end
	end

end



local EnterFlag = 0

-- \breif : 玩家进入神迹
--  param3: 进入脚本区域的玩家ID
--  param4: 进入的脚本区域ID
function GodMiracleCpature_OnRoleEnter(MapID, InstanceID, OwnerID)--, ObjAreaID

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- 地图限制判断
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- 非争夺状态不做处理
	local WeekDay = os.date("%A")
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2 or GetCaptureGuildID(MapID) == 0 then
		if godCurState == 1 and WeekDay=="Saturday" and Hour == 19 and Min >= 50 then

		else
			return
		end
	end

	-- 玩家进入战场要离开队伍
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, OwnerID)
	if TeamID ~= nil then
		role.LetRoleLeaveTeam(TeamID, OwnerID)
	end

	-- 设置玩家进入神迹区域，上层创建并神迹进入神迹区域标志
	-- 在C++层创建玩家并设置玩家可以记录活跃时间
	local guildID = guild.GetRoleGuildID(OwnerID)
	if guildID ~= 4294967295 then
		--取公会阵营类型，1表示防守，2表示攻击，0表示没有参战权
		local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, guildID)
		if DOrCFlag ~= GodEnum.Null then

			-- 设置角色进入神迹标志
			guild.GuildRoleEnterGodArea(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag)

			-- 设置活跃时间
			guild.SetRoleStartActiveTimes(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag, 1)

			--	ES_CSAttack		=	8,	// 城战进攻阵营
			--	ES_CSDefence	=	9,	// 城战防御阵营
			if DOrCFlag == GodEnum.Defend then
				unit.UnsetState(MapID, 0, OwnerID, 8)
				unit.SetState(MapID, 0, OwnerID, 9)
			elseif DOrCFlag == GodEnum.Capture then
				unit.UnsetState(MapID, 0, OwnerID, 9)
				unit.SetState(MapID, 0, OwnerID, 8)
			end

			if EnterFlag == 0 then

				--  进入区域进行广播
				-- 您的帮派正在参与此神迹争夺战，敌友判断已变为争夺战模式！
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1416)
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)

				-- 关闭死亡保护
				role.UnSetRoleState(OwnerID, 1, 1)

				EnterFlag =  1

			end

			-- 通知客户端三界阵旗归属情况
			SyncFlagAttach(MapID, OwnerID)
		end
	end

end

function GetGuildCapType(NpcType, MapID)
	local CapGuildID = GetCapGodStacke(MapID, NpcType)
	local DefendGuildID = ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild;
	local CaptureGuildID = ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild;
	if CapGuildID == DefendGuildID then
		return 1
	elseif CapGuildID == CaptureGuildID then
		return 2
	else
		return 0
	end
end

function SyncFlagAttach(MapID, OwnerID)
	guild.SyncFlagAttack(GetGodMiracleIDMinOne(MapID), 0, GetGuildCapType(1, MapID), OwnerID, 0, 0, ClanGodMiracle_Proto[MapID].PosX1, ClanGodMiracle_Proto[MapID].PosY1, ClanGodMiracle_Proto[MapID].PosZ1)	-- 天
	guild.SyncFlagAttack(GetGodMiracleIDMinOne(MapID), 1, GetGuildCapType(3, MapID), OwnerID, 0, 0, ClanGodMiracle_Proto[MapID].PosX2, ClanGodMiracle_Proto[MapID].PosY2, ClanGodMiracle_Proto[MapID].PosZ2)	-- 人
	guild.SyncFlagAttack(GetGodMiracleIDMinOne(MapID), 2, GetGuildCapType(2, MapID), OwnerID, 0, 0, ClanGodMiracle_Proto[MapID].PosX3, ClanGodMiracle_Proto[MapID].PosY3, ClanGodMiracle_Proto[MapID].PosZ3)	-- 魔
end

-- \breif : 玩家离开神迹区域
--  param3: 离开脚本区域的玩家ID
--  param4: 离开的脚本区域ID
function GodMiracleCpature_OnRoleLeave(MapID, InstanceID, OwnerID)--, ObjAreaID

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- 地图限制判断
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- 非争夺状态不做处理
	local WeekDay = os.date("%A")
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2 or GetCaptureGuildID(MapID) == 0 then
		if godCurState == 1 and WeekDay=="Saturday" and Hour == 19 and Min >= 50 then

		else
			return
		end
	end

	-- 设置玩家离开神迹区域
	local guildID = guild.GetRoleGuildID(OwnerID)
	if guildID ~= 4294967295 then

		local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, guildID)
		if DOrCFlag ~= GodEnum.Null then

			guild.SetRoleStartActiveTimes(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag, 0)

			-- 取消玩家战场状态
			--	ES_CSAttack		=	8,	// 城战进攻阵营
			--	ES_CSDefence	=	9,	// 城战防御阵营
			unit.UnsetState(MapID, 0, OwnerID, 8)
			unit.UnsetState(MapID, 0, OwnerID, 9)

			-- 设置为练级模式
			local ERS_PeaceModel = 8388608
			role.SetRoleState(ERS_PeaceModel);

			--您已离开神迹区域或争夺战已结束，敌友判断已变为通常模式！
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1417)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)--]]

			--通知客户端
			guild.RoleLeaveArea(GetGodMiracleIDMinOne(MapID), OwnerID, GetRoleGuildDefendOrCapture(MapID, guildID))

			EnterFlag = 0
		end
	end
end

-- \breif : 检测指定技能是否能被释放
--  param3: SkillID(技能ID)
--  param4: 释放技能者ID
--  param5: 目标ID
--  return: 错误标示信息以脚本信息发给客户端,本函数只要不返回0就为错误,c++层则不做处理
function GodMiracleCpature_CanCast(MapID, InstanceID, SkillID, OwnerID, TargetID)

	--使用轮回技能
	if SkillID == 10508 then
		-- 地图限制判断
		if IsInGodAreaMap(MapID) == 0 then
			if MapID == 2092183860 or MapID == 2092184116 or MapID == 2092184372 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26,	206282)		--斗技场中无法使用复活技能
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)
				return 999
			end
			return 0
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID,26, 1439)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

	else
		-- 地图限制判断
		if IsInGodAreaMap(MapID) == 0 then
			return
		end

		-- 神迹在空闲状态,不允许开启
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
		if godCurState  == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID,26, 1427)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

		-- 获取帮派ID(eroor:接口函数应该在没有帮派的情况下返回0不应该返回无限大数值)
		local GuildID = guild.GetRoleGuildID(OwnerID);
		-- 获取玩家帮派职位(GuildPos==nil 则帮派不存在)
		local GuildPos = -1
		GuildPos = guild.GetRoleGuildPos(GuildID , OwnerID);
		if GuildPos == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID,26, 1419)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

		-- 玩家没有开启行凶状态,不可开启
		-- 您尚未开启行凶状态，无法进行开启神迹操作！ and role.IsRoleInStatus(OwnerID, 1048576) == false,1048576在龙里面是行凶模式2，只能攻击行凶玩家
		if role.IsRoleInStatus(OwnerID, 4) == false then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1403)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

		if godCurState  == 2 and GetDefendGuildID(MapID) ~= 0 then
			-- 玩家不在争夺帮派范围!
			if GetDefendGuildID(MapID) ~= GuildID and GetCaptureGuildID(MapID) ~= GuildID then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1429)
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)

				return 999
			end
		end

		-- 未参战，无人争夺，不用开启
		if godCurState  == 2 and GetDefendGuildID(MapID) ~= 0 and GetCaptureGuildID(MapID) == 0 then
			return 999
		end

		-- 神迹柱已属于本帮派，不用开启
		local TargetTypeID = unit.GetUnitTypeID(MapID, InstanceID, TargetID)
		local NpcIndex = GetGodStackeIndex(MapID, TargetTypeID)
		if NpcIndex == GodEnum.Null then return 999 end
		local guildID1 = GetCapGodStacke(MapID, NpcIndex)
		if guildID1 ~= 0 and guildID1 == GuildID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1425)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

		--等级不够不可开启
		local OwnerLevel = role.GetRoleLevel(MapID, InstanceID, OwnerID)
		if OwnerLevel < 20 then
			return
		end

		if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
			-- 增加开启效果
			unit.AddBuff(MapID, 0, TargetID, Flag_Opening_BuffID.God, TargetID)
		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
			-- 增加开启效果
			unit.AddBuff(MapID, 0, TargetID, Flag_Opening_BuffID.People, TargetID)
		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
			-- 增加开启效果
			unit.AddBuff(MapID, 0, TargetID, Flag_Opening_BuffID.Monster, TargetID)
		end
	end
end

-- \breif : 技能释放作用效果
--  param3: SkillID(技能ID)
--  param4: 释放技能者ID
--  return: 1:通知上层不要作用此技能效果 0:则相反
function GodMiracleCpature_CastSkill(MapID, InstanceID, SkillID, OwnerID, TargetID, TargetTypeID)

	-- 地图限制判断
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- 得到占领柱那更柱子
	local GodStackeIndex = GetGodStackeIndex(MapID, TargetTypeID)
	if GodStackeIndex == GodEnum.Null then return 1 end

	-- 玩家帮派解散,不可开启
	local guildID = 0
	guildID = guild.GetRoleGuildID(OwnerID)
	local GuildPos
	GuildPos  = guild.GetRoleGuildPos(guildID, OwnerID);
	if GuildPos == nil then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1404)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end


	-- 玩家没有开启行凶状态,不可开启
	if role.IsRoleInStatus(OwnerID, 1) == true then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1406)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end
	-- and role.IsRoleInStatus(OwnerID, 1048576) == false, 1048576在龙里面是行凶模式2，只能攻击行凶玩家
	if role.IsRoleInStatus(OwnerID, 4) == false then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1406)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end

	-- 神迹柱已属于本帮派，不用开启
	local guildID1 = GetCapGodStacke(MapID, TargetTypeID)
	if guildID1 ~= 0 and guildID1  == guildID then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1415)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end

	--等级不够不可开启
	local OwnerLevel = role.GetRoleLevel(MapID, InstanceID, OwnerID)
	if OwnerLevel < 20 then
		return
	end


	-- 状态0和1是用来处理第一次
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState  == 0 or (godCurState == 2 and GetDefendGuildID(MapID) == 0 ) then
		-- 设置帮派获取神迹柱
		SetCapGodStacke(MapID, GodStackeIndex, guildID)

		if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
			-- 取消柱子效果BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.God)

			-- 移除银色效果
			unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.God)

			-- 增加是攻方效果
			unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.God, TargetID)

		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
			-- 取消柱子效果BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.People)

			-- 移除银色效果
			unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.People)

			-- 增加是攻方效果
			unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.People, TargetID)

		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
			-- 取消柱子效果BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.Monster)

			-- 移除银色效果
			unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.Monster)

			-- 增加是攻方效果
			unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.Monster, TargetID)

		end

		-- 神迹区域范围内广播
		-- ×××××××（帮派名称）帮派的×××××××（玩家名称）已开启了×××××（结柱名称）！
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchWorldMsgEvent(MsgID)

		local piller = GuildCapturePillar(MapID, guildID)
		if piller == 2 then
			-- 设置c++神迹占领帮派
			guild.SetCaptureGodGuildID(GetGodMiracleIDMinOne(MapID), guildID)


			if godCurState == 0 then
				-- 设置处于空闲状态
				guild.SetGodCurState(GetGodMiracleIDMinOne(MapID), 1)
			end

			ClearGodMiracle(ClanGodMiracle_Proto[MapID].ClanGodMiracleID)

			--设置lua此帮派占领神迹
			SetDefendGuildID(MapID, guildID)

			-- 帮派内大字体广播
			-- 本帮通过艰苦的争夺获得了××神迹的占领权，所有成员获得神佑状态！同时大家可前往神迹购买所需物品！
			local MsgID2 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID2, 102, 100055)
			msg.AddMsgEvent(MsgID2, 5, ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].GdMiracleID)
			msg.DispatchGuildMsgEvent(MsgID2, guildID)

			-- 第一次占领神迹并进入空闲状态,神迹柱变绿
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster)

			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_Attack_BuffID.God)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_Attack_BuffID.People)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_Attack_BuffID.Monster)

			unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_Rest_BuffID.God, ClanGodMiracle_Proto[MapID].NPCIDObj1)
			unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_Rest_BuffID.People, ClanGodMiracle_Proto[MapID].NPCIDObj2)
			unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_Rest_BuffID.Monster, ClanGodMiracle_Proto[MapID].NPCIDObj3)

			ResetGodMiracle(ClanGodMiracle_Proto[MapID].ClanGodMiracleID)

		end

	elseif godCurState == 2 then

		-- 争夺时间到,不可开启
		local Hour = tonumber(os.date("%H"))
		if Hour == 21 then
			return
		end

		-- 设置帮派获取神迹柱
		SetCapGodStacke(MapID, GodStackeIndex, guildID)

		-- 神迹区域范围内广播
		-- ×××××××（帮派名称）帮派的×××××××（玩家名称）已开启了×××××（结柱名称）！
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchGuildMsgEvent(MsgID, GetCaptureGuildID(MapID))

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(MapID))

		-- 攻方同盟
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchAllianceGuildMsgEvent(MsgID, 2, GetGodMiracleIDMinOne(MapID))

		-- 守方同盟
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100066)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchAllianceGuildMsgEvent(MsgID, 1, GetGodMiracleIDMinOne(MapID))

		if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
			-- 取消柱子效果BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.God)
		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
			-- 取消柱子效果BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.People)
		elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
			-- 取消柱子效果BUFF
			unit.CancelBuff(MapID, 0, TargetID, Flag_Opening_BuffID.Monster)
		end

		-- 依据攻方或守方变化柱子的颜色
		if guildID ==GetDefendGuildID(MapID) then

			if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.God)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.God)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Attack_BuffID.God)
				unit.AddBuff(MapID, 0, TargetID, Flag_Defance_BuffID.God, TargetID)
			elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.People)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.People)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Attack_BuffID.People)
				unit.AddBuff(MapID, 0, TargetID, Flag_Defance_BuffID.People, TargetID)
			elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.Monster)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.Monster)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Attack_BuffID.Monster)
				unit.AddBuff(MapID, 0, TargetID, Flag_Defance_BuffID.Monster, TargetID)
			end

		elseif guildID == GetCaptureGuildID(MapID) then

			if TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj1 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.God)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.God)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Defance_BuffID.God)
				unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.God, TargetID)
			elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj2 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.People)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.People)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Defance_BuffID.People)
				unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.People, TargetID)
			elseif TargetID == ClanGodMiracle_Proto[MapID].NPCIDObj3 then
				unit.CancelBuff(MapID, 0, TargetID, Flag_BeforeCap_BuffID.Monster)
				unit.CancelBuff(MapID, 0, TargetID, Flag_Rest_BuffID.Monster)

				unit.CancelBuff(MapID, 0, TargetID, Flag_Defance_BuffID.Monster)
				unit.AddBuff(MapID, 0, TargetID, Flag_Attack_BuffID.Monster, TargetID)
			end

		end

	end

	return 0
end

--- breif: 对具有传送功能的道具做限制
function GodMiracleCapture_ItemCanUse(MapID, InstanceID, TypeID, TargetID)

	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec   = tonumber(os.date("%S"))

	if Hour == 20 and (Min <= 30 and Min >= 0)then
		-- 玩家在神迹争夺范围内
		local guildID = 0
		guildID = guild.GetRoleGuildID(TargetID)
		local GuildPos
		GuildPos  = guild.GetRoleGuildPos(guildID, TargetID);
		if GuildPos ~= nil then
			if GetDefendGuildID(MapID) == guildID or GetCaptureGuildID(MapID) == guildID then
				return -1, 1
			end
		end
	end
	return 0, 0
end

--- breif: 战场中下线，再上线时回到NPC旁边
function GodMiracleCpature_CanEnterWhenOnline(MapID, RoleID)
	-- 取消玩家战场状态
	--	ES_CSAttack		=	8,	// 城战进攻阵营
	--	ES_CSDefence	=	9,	// 城战防御阵营
	role.UnsetStateBefAdd2Map(RoleID, 8)
	role.UnsetStateBefAdd2Map(RoleID, 9)
	return MapID_S02, S02_Trans_Pos.x, S02_Trans_Pos.y, S02_Trans_Pos.z
end

function GodMiracleCpature_OnRoleEnterArea(MapID, InstanceID, OwnerID, ObjAreaID)
	if OpenGodMiracleFlag == 0 then
		return
	end

	if role.IsRoleHaveBuff(MapID, InstanceID, OwnerID, 9001601) then
		return
	end

	-- 地图限制判断
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- 非争夺状态不做处理
	local WeekDay = os.date("%A")
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2 or GetCaptureGuildID(MapID) == 0 then
		if godCurState == 1 and WeekDay=="Saturday" and Hour == 19 and Min >= 50 then

		else
			return
		end
	end

	local guildID = guild.GetRoleGuildID(OwnerID)
	if guildID == 4294967295 then
		return
	end
	local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, guildID)
	if DOrCFlag == GodEnum.Null then
		return
	end

	-- 脚本区域判断
	if ObjAreaID ~= ClanGodMiracle_Proto[MapID].DefAreaID and ObjAreaID ~= ClanGodMiracle_Proto[MapID].AttAreaID then
		return
	end

	local NeedAddBuff = 0
	if DOrCFlag == GodEnum.Defend and ObjAreaID == ClanGodMiracle_Proto[MapID].DefAreaID then
		NeedAddBuff = 1
	elseif DOrCFlag == GodEnum.Capture and ObjAreaID == ClanGodMiracle_Proto[MapID].AttAreaID then
		NeedAddBuff = 1
	end

	if NeedAddBuff == 1 then
		role.AddBuff(MapID, InstanceID, OwnerID, 9001601, OwnerID)
	end
end

function GodMiracleCpature_OnRoleLeaveArea(MapID, InstanceID, OwnerID, ObjAreaID)
	if role.IsRoleHaveBuff(MapID, InstanceID, OwnerID, 9001601) then
		role.CancelBuff(MapID, InstanceID, OwnerID, 9001601)
	end
end

--活动控制台开关
function GodMiracleCpature_OnActOpenClose(ActiveID, IsOpen)
	OpenGodMiracleFlag = IsOpen

	TianShenZhong_OpenFlag = IsOpen
	QiHuaZhiHai_OpenFlag		= IsOpen
	BenLeiLieGu_OpenFlag		= IsOpen

	if IsOpen == 0 then
		-- 设置神迹为关闭状态
		guild.SetGodCurState(0, 3)
		guild.SetGodCurState(1, 3)
		guild.SetGodCurState(2, 3)
	else
		-- 设置神迹为未占领状态
		guild.SetGodCurState(0, 0)
		guild.SetGodCurState(1, 0)
		guild.SetGodCurState(2, 0)
	end
end


------------------------------------------------------------------------------
-- register

-- 向活动注册初始化和适时更新操作
aux.RegisterActEvent(ActiveID, 1, "GodMiracleCpature_OnActiveTimer")
aux.RegisterActEvent(ActiveID, 2, "GodMiracleCpature_OnActiveStart")
aux.RegisterActEvent(ActiveID, 4, "GodMiracleCpature_OnActiveTimeMin")
aux.RegisterActEvent(ActiveID, ESAE_OnActOpenClose, "GodMiracleCpature_OnActOpenClose")

-- map
aux.RegisterMapEvent("j01", 5, "GodMiracleCpature_OnRoleDie")
aux.RegisterMapEvent("j03", 5, "GodMiracleCpature_OnRoleDie")
aux.RegisterMapEvent("j04", 5, "GodMiracleCpature_OnRoleDie")

aux.RegisterMapEvent("j01", 2, "GodMiracleCpature_OnRoleEnter")
aux.RegisterMapEvent("j03", 2, "GodMiracleCpature_OnRoleEnter")
aux.RegisterMapEvent("j04", 2, "GodMiracleCpature_OnRoleEnter")

aux.RegisterMapEvent("j01", 3, "GodMiracleCpature_OnRoleLeave")
aux.RegisterMapEvent("j03", 3, "GodMiracleCpature_OnRoleLeave")
aux.RegisterMapEvent("j04", 3, "GodMiracleCpature_OnRoleLeave")

aux.RegisterMapEvent("j01", 7, "GodMiracleCpature_OnRoleEnterArea")
aux.RegisterMapEvent("j03", 7, "GodMiracleCpature_OnRoleEnterArea")
aux.RegisterMapEvent("j04", 7, "GodMiracleCpature_OnRoleEnterArea")

aux.RegisterMapEvent("j01", 23, "GodMiracleCpature_OnRoleLeaveArea")
aux.RegisterMapEvent("j03", 23, "GodMiracleCpature_OnRoleLeaveArea")
aux.RegisterMapEvent("j04", 23, "GodMiracleCpature_OnRoleLeaveArea")


-- 战场中下线，再上线时回到NPC旁边
aux.RegisterMapEvent("j01", 12, "GodMiracleCpature_CanEnterWhenOnline")
aux.RegisterMapEvent("j03", 12, "GodMiracleCpature_CanEnterWhenOnline")
aux.RegisterMapEvent("j04", 12, "GodMiracleCpature_CanEnterWhenOnline")

-- skill
-- 调查技能注册检测和使用事件务
aux.RegisterSkillEvent(9000201, 0, "GodMiracleCpature_CanCast")
aux.RegisterSkillEvent(9000201, 2, "GodMiracleCpature_CastSkill")
-- 轮回技能使用
aux.RegisterSkillEvent(1050801, 0, "GodMiracleCpature_CanCast")

-- 使用丈地云幡|丈地神符|丈地神符|万里追踪
aux.RegisterItemEvent(1310001, 0, "GodMiracleCapture_ItemCanUse")
aux.RegisterItemEvent(1360003, 0, "GodMiracleCapture_ItemCanUse")
aux.RegisterItemEvent(1360102, 0, "GodMiracleCapture_ItemCanUse")
aux.RegisterItemEvent(1360110, 0, "GodMiracleCapture_ItemCanUse")



