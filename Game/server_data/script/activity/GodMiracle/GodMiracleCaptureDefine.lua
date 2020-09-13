-------------------------------------------------------------------------------
-- Copyright (c) 2004 TENGWU Entertainment All rights reserved.
-- filename: GodMiracleCpatureDefine.h
-- author: chdeng
-- actor:
-- data: 2010-01-25
-- last:
-- brief: 神迹争夺结构定义
-------------------------------------------------------------------------------
-- 神迹数量
ClanGodMiracleNum = 5
Minrecord 	 = 0
ActiveID = 25

MapID_test02 = 3618849499
MapID_FX	 = 2216111541
MapID_ZL	 = 2216112053
MapID_WS	 = 2216110261
MapID_S02  = 3424073512

--!\ brief: 地图是否在神迹争夺区域内
function IsInGodAreaMap(MapID)
	if MapID == MapID_FX or MapID == MapID_ZL or MapID == MapID_WS then
		return	1
	else
		return 0
	end
end

-- 用的到枚举
GodEnum =  {Null=0, Npc1=1, Npc2=2, Npc3=3, Defend=1, Capture=2, TRUE=1,FALSE=0, StdScoresTick=150}

-- 所有氏族神迹的结构体
ClanGodMiracle_Proto = {}

ClanGodMiracle_Proto[MapID_FX] 	   = {PosX1=0,PosY1=0,PosZ1=0, PosX2=0,PosY2=0,PosZ2=0, PosX3=0,PosY3=0,PosZ3=0, ClanGodMiracleID=1, NPCID1 = 6010012, NPCID2 = 6010013, NPCID3 = 6010014, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, DefAreaID=1139, AttAreaID=1140}
ClanGodMiracle_Proto[MapID_ZL] 	   = {PosX1=0,PosY1=0,PosZ1=0, PosX2=0,PosY2=0,PosZ2=0, PosX3=0,PosY3=0,PosZ3=0, ClanGodMiracleID=2, NPCID1 = 6010015, NPCID2 = 6010016, NPCID3 = 6010017, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, DefAreaID=0, AttAreaID=0}
ClanGodMiracle_Proto[MapID_WS] 	   = {PosX1=0,PosY1=0,PosZ1=0, PosX2=0,PosY2=0,PosZ2=0, PosX3=0,PosY3=0,PosZ3=0, ClanGodMiracleID=3, NPCID1 = 6010018, NPCID2 = 6010019, NPCID3 = 6010020, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, DefAreaID=0, AttAreaID=0}

ClanGodMiracle_Pos = {}
ClanGodMiracle_Pos[MapID_FX]	   = { x1=383, y1=5100,   z1=163,   x2=827, y2=5743,   z2=832, x3=510, y3=4840,   z3=529}			--- 1天神 2魔王 3人界
ClanGodMiracle_Pos[MapID_ZL]	   = { x1=376, y1=3260,   z1=1087, x2=543, y2=2498,   z2=687, x3=932, y3=2664,   z3=786}			--- 1天神 2魔王 3人界
ClanGodMiracle_Pos[MapID_WS]	   = { x1=231, y1=13351, z1=880,   x2=675, y2=14815, z2=606, x3=753, y3=14063, z3=960}			--- 1天神 2魔王 3人界

RoleEnter_Pos = {}
RoleEnter_Pos[MapID_FX]	= { x1=869,	 y1=103,  z1=140,   x2=158,  y2=119,  z2=776}					--天神冢战场
RoleEnter_Pos[MapID_ZL]	= { x1=805,  y1=59,    z1=1295, x2=364,  y2=67,    z2=320}					--七花之海战场		
RoleEnter_Pos[MapID_WS]	= { x1=450,  y1=282,  z1=541,   x2=302,  y2=272,  z2=1178}					--奔雷裂谷战场

-- 三界阵旗
Flag_BeforeCap_BuffID	= {God = 9001801, People = 9002101, Monster = 9002401}	-- 中立
Flag_Attack_BuffID			= {God = 9001901, People = 9002201, Monster = 9002501}	-- 进攻
Flag_Defance_BuffID		= {God = 9002001, People = 9002301, Monster = 9002601}	-- 防守
Flag_Opening_BuffID		= {God = 9002701, People = 9002901, Monster = 9003101}	-- 封印
Flag_Rest_BuffID				= {God = 9002801, People = 9003001, Monster = 9003201}	-- 休战


S02_Trans_Pos = {x=611, y=23095, z=671}

--战场传送员NPC的ID
TransmitorID_FX = 5010110		--天神冢战场
TransmitorID_ZL = 5010238		--七花之海战场
TransmitorID_WS = 5010239		--奔雷战场

GodMiracleTypeID = {}
GodMiracleTypeID[TransmitorID_FX] = {GodMiracleTypeID=0, MapID=MapID_FX}
GodMiracleTypeID[TransmitorID_ZL] = {GodMiracleTypeID=1, MapID=MapID_ZL}
GodMiracleTypeID[TransmitorID_WS] = {GodMiracleTypeID=2, MapID=MapID_WS}

--  \breif: 通过柱子ID得到是那个柱子
function GetGodStackeIndex(MapID, NpcTypeID)
	if ClanGodMiracle_Proto[MapID].NPCID1 == NpcTypeID then
		return GodEnum.Npc1
	elseif ClanGodMiracle_Proto[MapID].NPCID2 == NpcTypeID then
		return GodEnum.Npc2
	elseif ClanGodMiracle_Proto[MapID].NPCID3 == NpcTypeID then
		return GodEnum.Npc3
	end
	return GodEnum.Null
end

-- \breif: 得到区域ID
function GetAreaID(MapID)
	return ClanGodMiracle_Proto[MapID].GodAreaID
end

-- \breif: 得到神迹ID(为上层提供)
function GetGodMiracleIDMinOne(MapID)
	return ClanGodMiracle_Proto[MapID].ClanGodMiracleID -1
end

broadCastGlag = {flag1 = 0, flag2 = 0, flag3=0, flag4=0, flag5=0, flag6=0}
function ClearBroaCastGlag()
	broadCastGlag.flag1=0
	broadCastGlag.flag2=0
	broadCastGlag.flag3=0
	broadCastGlag.flag4=0
	broadCastGlag.flag5=0
	broadCastGlag.flag6=0
end

-- 氏族的适时信息
ClanGodMiracle_CurInfo = {}
ClanGodMiracle_CurInfo[1] = {MapID = MapID_FX,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 5510630, openFlag = 0}
ClanGodMiracle_CurInfo[2] = {MapID = MapID_ZL,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 5510631, openFlag = 0}
ClanGodMiracle_CurInfo[3] = {MapID = MapID_WS,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 5510632, openFlag = 0}

-- 结果保存
Capture_Result = {}
Capture_Result[1] = {MapID = MapID_FX, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 5510630}
Capture_Result[2] = {MapID = MapID_ZL, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 5510631}
Capture_Result[3] = {MapID = MapID_WS, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 5510632}

-- \breif: 清空神迹的争夺结果
function ClearGodMiracleResult()
	for k, v in pairs(Capture_Result) do
		Capture_Result[k].DefendGuild = 0
		Capture_Result[k].CaptureGuild = 0
		Capture_Result[k].WinOrLoseFlag = 0
	end
end

-- \breif: 得到开关标志
function GetOpenFlag(MapID)
	return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].openFlag
end

-- \breif: 清空某神迹
function ClearGodMiracle(GodMiracleID)
	ClanGodMiracle_CurInfo[GodMiracleID].DefendGuild 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureGuild 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].Npc1 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc2 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc3 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].DefendSocre 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureScores 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].tick 			= 0
end

-- \breif: 神迹争夺完毕清空神迹信息
function ResetGodMiracle(GodMiracleID)
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureGuild 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].Npc1 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc2 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc3 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].DefendSocre 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureScores 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].tick 			= 0
end

-- \breif: 得到地图ID
function GetCurInfoMapID(GodMiracleID)			return ClanGodMiracle_CurInfo[GodMiracleID].MapID end

-- \breif:  初始化神迹
function SetInitFlag(MapID, inFlag)			ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].InitFlag = inFlag	end
function GetInitFlag(MapID)					return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].InitFlag	end

-- \breif: 得到或设置防御帮派ID
function GetDefendGuildID(MapID) 				return   ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild 	 end
function SetDefendGuildID(MapID, inGuildID)	ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild = inGuildID end

-- \breif: 得到或设置争夺帮派ID
function GetCaptureGuildID(MapID) 				return   ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild   end
function SetCaptureGuildID(MapID, inGuildID)	ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild = inGuildID end

-- \breif: 增加和设置TICK
function Gettick(MapID)						return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick	end
function SetTick(MapID, inTick)				ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick = inTick	end
function ModTick(MapID, inTick)				ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick = ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick + inTick  end

-- \breif: 设置和到底积分
--  param2: 守方或攻方的标志
function GetScores(MapID, DOrCFlag)
	if DOrCFlag ==  GodEnum.Defend then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre
	elseif DOrCFlag ==  GodEnum.Capture then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores
	end

end

function SetScores(MapID, DOrCFlag, inScores)
	if DOrCFlag ==  GodEnum.Defend then
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre = inScores
	elseif DOrCFlag == GodEnum.Capture then
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores = inScores
	end
end

function ModScores(MapID, DOrCFlag, inScores)
	if DOrCFlag ==  GodEnum.Defend then
		ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre =  ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre + inScores
	elseif DOrCFlag ==  GodEnum.Capture then
		ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores =  ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores + inScores
	end
end


-- \breif: 设置和得到占领神迹的帮派ID
function GetCapGodStacke(MapID, NpcType)
	if NpcType == GodEnum.Npc1 then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc1
	elseif NpcType == GodEnum.Npc2 then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc2
	elseif NpcType == GodEnum.Npc3 then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc3
	end
end

function SetCapGodStacke(MapID, NpcType, GuildID)
	local flagType = 0
	local PosX=0
	local PosY=0
	local PosZ=0
	if NpcType == GodEnum.Npc1 then
		 PosX=ClanGodMiracle_Proto[MapID].PosX1
		 PosY=ClanGodMiracle_Proto[MapID].PosY1
		 PosZ=ClanGodMiracle_Proto[MapID].PosZ1
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc1 = GuildID
		 flagType = 0
	elseif NpcType == GodEnum.Npc2 then
		 PosX=ClanGodMiracle_Proto[MapID].PosX3
		 PosY=ClanGodMiracle_Proto[MapID].PosY3
		 PosZ=ClanGodMiracle_Proto[MapID].PosZ3
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc2 = GuildID
 		 flagType = 2
	elseif NpcType == GodEnum.Npc3 then
		 PosX=ClanGodMiracle_Proto[MapID].PosX2
		 PosY=ClanGodMiracle_Proto[MapID].PosY2
		 PosZ=ClanGodMiracle_Proto[MapID].PosZ2
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc3 = GuildID
		 flagType = 1
	else
		 return
	end

	-- 通知客户端三界阵旗归属情况
	guild.SyncFlagAttack(GetGodMiracleIDMinOne(MapID), flagType, GetGuildCapType(NpcType, MapID), MapID, 0, 1, PosX, PosY, PosZ)	-- 天
end

-- \breif: 查看帮派ID是否在争夺帮派的俩神迹中
--  param2:
function IsInGodCaptureGuild(MapID, GuildID)

	if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild == 0 and ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild == 0 then
		return 0
	end

	if GuildID == ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild or GuildID == ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild then
		return 1
	end

	return 0

end

-- \breif: 返回玩家所在的帮派是攻防还是守方
function GetRoleGuildDefendOrCapture(MapID, GuildID)

	--取公会阵营类型，1表示防守，2表示攻击，0表示没有参战权
	local guildAlliance = guild.GetGuildAllianceType(GetGodMiracleIDMinOne(MapID), GuildID)
	if guildAlliance == 1 then
		return GodEnum.Defend
	elseif guildAlliance == 2 then
		return GodEnum.Capture
	else
		return GodEnum.Null
	end

end

-- \breif: 查看某帮派占有几个柱子
function GuildCapturePillar(MapID, GuildID)
	if GuildID == 0 then
		return 0
	end

	local Pillar = 0
		if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc1 == GuildID then
			Pillar = Pillar + 1
		end

		if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc2 == GuildID then
			Pillar = Pillar + 1
		end

		if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc3 == GuildID then
			Pillar = Pillar + 1
		end

	return Pillar
end

-- \breif:  得到帮派占领神迹的个数
--  param1: 帮派ID
function GetGuildCaptureNum(GuildID)
	if GuildID ==  0 then
		return 0
	end

	local CapNum = 0
	for k, v in pairs(ClanGodMiracle_CurInfo) do
		if ClanGodMiracle_CurInfo[k].DefendGuild == GuildID then
			CapNum  = CapNum  + 1
		end
	end
	return CapNum
end

-- \breif:  依据神迹个数得到玩家当前使用的BUFFID
--  param1: 夺取神迹的个数
--function GetBuffIDbyGodMiracleNum(godMiracleNum)
--	if godMiracleNum == 1 then
--		return 9000701
--	elseif godMiracleNum == 2  then
--		return 9000802
--	elseif godMiracleNum == 3 then
--		return 9000903
--	elseif godMiracleNum == 4 then
--		return 9001004
--	elseif godMiracleNum == 5 then
--		return 9001105
--	end
--end

function InitGodMiracleOpenFlag()
	ClanGodMiracle_CurInfo[1].openFlag = TianShenZhong_OpenFlag;
	ClanGodMiracle_CurInfo[2].openFlag = QiHuaZhiHai_OpenFlag;
	ClanGodMiracle_CurInfo[3].openFlag = BenLeiLieGu_OpenFlag;
end

-- \breif: 创建神迹柱
function CreateGodmiracle()
	for k, v in pairs(ClanGodMiracle_CurInfo) do
		if GetOpenFlag(ClanGodMiracle_CurInfo[k].MapID) == 1 then
			local MapID = ClanGodMiracle_CurInfo[k].MapID
			ClanGodMiracle_Proto[MapID].NPCIDObj1 = map.MapCreateCreature(MapID, 0, ClanGodMiracle_Proto[MapID].NPCID1, ClanGodMiracle_Pos[MapID].x1, ClanGodMiracle_Pos[MapID].y1, ClanGodMiracle_Pos[MapID].z1)
			ClanGodMiracle_Proto[MapID].NPCIDObj2 = map.MapCreateCreature(MapID, 0, ClanGodMiracle_Proto[MapID].NPCID2, ClanGodMiracle_Pos[MapID].x2, ClanGodMiracle_Pos[MapID].y2, ClanGodMiracle_Pos[MapID].z2)
			ClanGodMiracle_Proto[MapID].NPCIDObj3 = map.MapCreateCreature(MapID, 0, ClanGodMiracle_Proto[MapID].NPCID3, ClanGodMiracle_Pos[MapID].x3, ClanGodMiracle_Pos[MapID].y3, ClanGodMiracle_Pos[MapID].z3)
			ClanGodMiracle_Proto[MapID].PosX1,ClanGodMiracle_Proto[MapID].PosY1,ClanGodMiracle_Proto[MapID].PosZ1=unit.GetPosition(MapID, 0,ClanGodMiracle_Proto[MapID].NPCIDObj1);
			ClanGodMiracle_Proto[MapID].PosX2,ClanGodMiracle_Proto[MapID].PosY2,ClanGodMiracle_Proto[MapID].PosZ2=unit.GetPosition(MapID, 0,ClanGodMiracle_Proto[MapID].NPCIDObj3);
			ClanGodMiracle_Proto[MapID].PosX3,ClanGodMiracle_Proto[MapID].PosY3,ClanGodMiracle_Proto[MapID].PosZ3=unit.GetPosition(MapID, 0,ClanGodMiracle_Proto[MapID].NPCIDObj2);
		end
	end
end

-- \breif: 得到神迹的占领情况
function InitAllGodMiracle()

	--创建
	CreateGodmiracle()

	SetDefendGuildID(MapID_FX ,act.GetActScriptData(ActiveID, 1, 0))
	SetDefendGuildID(MapID_ZL ,act.GetActScriptData(ActiveID, 1, 2))
	SetDefendGuildID(MapID_WS ,act.GetActScriptData(ActiveID, 1, 4))

	local WeekDay = os.date("%A")

	if WeekDay == "Saturday" then
		
		local Hour = tonumber(os.date("%H"))
		local Min  = tonumber(os.date("%M"))

		if (Hour == 19 and Min > 50)  or Hour == 20 then
			-- 设置进攻公会
			SetCaptureGuildID(MapID_FX ,act.GetActScriptData(ActiveID, 1, 10))
			SetCaptureGuildID(MapID_ZL ,act.GetActScriptData(ActiveID, 1, 12))
			SetCaptureGuildID(MapID_WS ,act.GetActScriptData(ActiveID, 1, 14))
			broadCastGlag.flag2 = 1
		end

		if Hour == 20 then
			-- 设置神迹为争夺状态
			guild.SetGodCurState(0, 2)
			guild.SetGodCurState(1, 2)
			guild.SetGodCurState(2, 2)
		end

	end

	for k, v in pairs(ClanGodMiracle_CurInfo) do

		if GetOpenFlag(ClanGodMiracle_CurInfo[k].MapID) == 1 then
			-- 依据神迹的当前状态,给柱子上个颜色BUFF
			local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
			if godCurState  == 0 then -- 未被争夺
				ChangePPillarColor(ClanGodMiracle_CurInfo[k].MapID, 0)
			elseif godCurState   == 1 then -- 空闲
				ChangePPillarColor(ClanGodMiracle_CurInfo[k].MapID, 1)
			end
		end
	end
end

-- \breif: 改变柱子颜色
--  param2:柱子的状态： 0 未被争夺 1空闲
function ChangePPillarColor(MapID, ColorType)
	if ColorType == 0 then
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God, ClanGodMiracle_Proto[MapID].NPCIDObj1)
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People, ClanGodMiracle_Proto[MapID].NPCIDObj2)
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster, ClanGodMiracle_Proto[MapID].NPCIDObj3)
	elseif ColorType == 1 then
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, Flag_Rest_BuffID.God, ClanGodMiracle_Proto[MapID].NPCIDObj1)
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, Flag_Rest_BuffID.People, ClanGodMiracle_Proto[MapID].NPCIDObj2)
		unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, Flag_Rest_BuffID.Monster, ClanGodMiracle_Proto[MapID].NPCIDObj3)
	end
end

Minrecord = 0

-- \breif: 广播相关
function BroadcastGodMsgMin()

	--检测时间
	local Min  = tonumber(os.date("%M"))

	Minrecord = Minrecord + 1

	-- 每小时提示一次战场状态
	if Minrecord == 59 then
		for k, v in pairs(ClanGodMiracle_CurInfo) do
			-------------------------------------------------------------------------------- world(big)
			-- 当前××神迹仍处于未被占领状态，请各帮派速往争夺！
			if ClanGodMiracle_CurInfo[k].DefendGuild == 0 and ClanGodMiracle_CurInfo[k].openFlag ~= 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100054)
				msg.AddMsgEvent(MsgID, 5  , ClanGodMiracle_CurInfo[k].GdMiracleID)
				msg.DispatchWorldMsgEvent(MsgID)
			end
		end
		Minrecord = 0
	end

end

BMBroad = 0
ClearBroaCastFlg = 0

-- \breif: 广播相关
-- remark: 每tick更新
function BroadcastGodMsgTick()

	local WeekDay = os.date("%A")
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec = tonumber(os.date("%S"))

	-------------------------------------------------------------------------------- world(bif)
	-- 今日的氏族神迹争夺战竞价报名已经开始，请各大帮派帮主在帮派界面中进行报名！
	if WeekDay == "Monday" then
		if BMBroad == 0 and Hour >=8 and Hour <= 20 and Min == 0 then
			local MsgIDBM = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgIDBM, 101, 1401)
			msg.DispatchWorldMsgEvent(MsgIDBM)
			BMBroad = 1
		end

		if Hour >=8 and Hour <= 20 and Min == 1 and BMBroad == 1 then
			BMBroad = 0
		end

		if Hour == 8 and Min == 0 and ClearBroaCastFlg == 0 then
			ClearBroaCastGlag()
			ClearBroaCastFlg = 1
		end
		if Hour == 8 and Min == 1 and ClearBroaCastFlg == 1 then
			ClearBroaCastFlg = 0
		end
	end

	for k, v in pairs(ClanGodMiracle_CurInfo) do

		-------------------------------------------------------------------------------- guild
		-- 本帮已确定参加今日20：00的××神迹争夺战，请大家做好准备，迎接战斗！
		if WeekDay == "Saturday" and Hour == 18 and Min == 0 then
			-- ***************拿到申请帮派ID***********************
			SetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID, guild.GetApplyGuildID(k-1))

				if broadCastGlag.flag1 == 0 then

					--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

						-- 守方
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100057)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

						-- 攻方
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100057)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchGuildMsgEvent(MsgID, GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))

						-- 守方同盟
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100057)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchAllianceGuildMsgEvent(MsgID, 1, k-1)

						-- 攻方同盟
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100057)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchAllianceGuildMsgEvent(MsgID, 2, k-1)
						
						if ClanGodMiracle_CurInfo[k].MapID == MapID_WS then
							broadCastGlag.flag1 = 1
						end
				--	end
			end
		end

		-------------------------------------------------------------------------------- guild
		-- ××神迹争夺战将于10分钟后开始，请大家前往神迹区域准备战斗！
		if WeekDay == "Saturday" and Hour == 19 and Min == 50 then

			if broadCastGlag.flag2 == 0 then
				--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

					-- 传出所有玩家
					map.MoveRole2AnotherMap(ClanGodMiracle_CurInfo[k].MapID, 0, MapID_S02, S02_Trans_Pos.x, S02_Trans_Pos.y, S02_Trans_Pos.z)

					-- 提前开始设置进攻公会
					SetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID, guild.GetApplyGuildID(k-1))

					-- 守方
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100058)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

					-- 攻方
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100058)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchGuildMsgEvent(MsgID, GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))

					-- 守方同盟
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100058)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchAllianceGuildMsgEvent(MsgID, 1, k-1)

					-- 攻方同盟
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100058)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchAllianceGuildMsgEvent(MsgID, 2, k-1)

					if ClanGodMiracle_CurInfo[k].MapID == MapID_WS then
						broadCastGlag.flag2 = 1
					end
				--end
			end
		end

		-- 设置帮派进入战斗状态，并神迹神迹进入争夺状态
		if WeekDay == "Saturday" and  Hour == 20 and Min == 0 then
			if broadCastGlag.flag5 == 0 then
				--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

					-- 从上层拿数据
					guild.SetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
					guild.SetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)

					-- 设置神迹为争夺状态
					guild.SetGodCurState(k-1, 2)


					--将柱子颜色变化成银色
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_Rest_BuffID.God)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_Rest_BuffID.People)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_Rest_BuffID.Monster)

					unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, Flag_BeforeCap_BuffID.God, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1)
					unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, Flag_BeforeCap_BuffID.People, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2)
					unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, Flag_BeforeCap_BuffID.Monster, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3)

					CalFlag = 0

					if ClanGodMiracle_CurInfo[k].MapID == MapID_WS then
						broadCastGlag.flag5 = 1
					end
				--end
			end
		end

		-- &lt;p1&gt;赢得&lt;p2&gt;的占领权！
		if broadCastGlag.flag6 ==0 and WeekDay == "Saturday" and  Hour == 21 and Min == 1 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100061)
			msg.AddMsgEvent(MsgID, 19, ClanGodMiracle_CurInfo[k].DefendGuild)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.DispatchMapMsgEvent(MsgID, ClanGodMiracle_CurInfo[k].MapID, 0)

			if ClanGodMiracle_CurInfo[k].MapID == MapID_WS then
				broadCastGlag.flag6 = 1
			end
		end
	end

	-------------------------------------------------------------------------------- world(big)
	if broadCastGlag.flag3 == 0 then
		-- 今日的神迹争夺战已经开始！
		if WeekDay == "Saturday" and  Hour == 20 and Min == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100059)
			msg.DispatchWorldMsgEvent(MsgID)
			broadCastGlag.flag3 = 1;
		end
	end


	-------------------------------------------------------------------------------- world
	-- 今日神迹争夺战尘埃落定，获胜帮派成员获得了增加经验，金钱和道具掉率的神佑状态，欢庆胜利！
	if broadCastGlag.flag4 == 0 then
		if Hour == 21 and Min == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100060)
			msg.DispatchWorldMsgEvent(MsgID)

			broadCastGlag.flag4 = 1
		end
	end

end


-- 广播战斗结果
function BroadcastCaptureResult()


	for k, v in pairs(Capture_Result) do

		-- 无进行
		if Capture_Result[k].WinOrLoseFlag  == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 1434)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.DispatchWorldMsgEvent(MsgID)
		--守方胜
		elseif Capture_Result[k].WinOrLoseFlag  == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 1432)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].DefendGuild)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].CaptureGuild)
			msg.DispatchWorldMsgEvent(MsgID)

		--攻赢
		elseif Capture_Result[k].WinOrLoseFlag  == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 1433)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].DefendGuild)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].CaptureGuild)
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end


	--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 1436)
	msg.DispatchWorldMsgEvent(MsgID)--]]

	--guild.SendAllCoptureGodGuildMember()

	ClearGodMiracleResult();
end

-- \breif:  获取神迹的占领帮派ID
--  param1: 神迹的ID
--  remark: 成功：帮派ID 失败:0
function GetGuildByGodMiracle(MapID)
	return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild
end

