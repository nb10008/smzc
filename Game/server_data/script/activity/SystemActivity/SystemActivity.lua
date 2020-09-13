--[[占用活动的scriptdata
0：用于记录究竟生成了几个8级属性的装备
1：用于记录究竟生成了几个9级属性的装备
2：用于记录究竟生成了几个10级属性的装备
3：用于记录资料片前夕已刷怪次数
4：用于记录还有多少天才能刷新出戒指的结果
5：用于记录当前的时间段出没出戒指
6: 用于记录当前的奖池金币数量
7：用于记录当天可以奖励的金币数量
8：用于记录当期是第几期
9：用于记录由于特殊设置JBZDY_SetDay而开出的大奖出现在了哪一天
10：用于记录上一期开出的奖励是多少
]]

--神器四个boss状态
SQ_bossREF = 1
SQ_boss1 = 1
SQ_boss2 = 1
SQ_boss3 = 1
SQ_boss4 = 1
--本次玩家是否灌注神器
isGuanzhu = 0
--神器临世当前玩家完成进度
canStartshenqi = 1		--1=可开启，0=正在进行，-1=当前已完成
--仲夏之夜活动
zhongxiayeshizhe = 0
--金币总动员
jiangchi_JBZDY = 6
benrijiangli_JBZDY = 7
benrijiangli_Index = 8
benrijiangli_JBZDY_SetDay = 9
benrijiangli_JBZDY_LastGold = 10
tbl_JBZDY_Online = {}
tbl_JBZDY_Offline = {}

--上古妖精庇护所
SGYJBHS_InstanceID = 1
qingrenjie_chuansongren = 0
SGYJBHS_Gate = {}
SGYJBHS_Gate[1] = 0
SGYJBHS_Gate[2] = 0
SGYJBHS_Gate[3] = 0
--中秋npc
mid_autumnNPC = 0
chuansongrenIn = 0
--场景内只能同时存在1个女王,间隔10分钟
curNum = 0
--通灵世界
todownstairs = 0
toupstairs = 0
curGodcrenum = 0
curEvilcrenum = 0
Evilbosshour = -1
Evilbossmin = -1

World_Boss_Pos = {}
World_Boss_Pos[3040333] = {}
World_Boss_Pos[3040333][1] = {3424072232,616,12783,1136}
World_Boss_Pos[3040333][2] = {3424072232,1106,14690,1135}
World_Boss_Pos[3040333][3] = {3424072232,1133,2821,466}
World_Boss_Pos[3040333][4] = {3424072232,2008,3707,958}
World_Boss_Pos[3040333][5] = {3424072232,2415,4117,1809}
World_Boss_Pos[3040333][6] = {3424072232,1967,5240,2164}
World_Boss_Pos[3040333][7] = {3424072232,2149,9965,2626}
World_Boss_Pos[3040333][8] = {3424072232,1768,5051,2524}
World_Boss_Pos[3040333][9] = {3424072232,1220,11750,2015}
World_Boss_Pos[3040333][10] = {3424072232,611,14793,1338}
World_Boss_Pos[3040334] = {}
World_Boss_Pos[3040334][1] = {3424073256,375,3529,238}
World_Boss_Pos[3040334][2] = {3424073256,426,3047,428}
World_Boss_Pos[3040334][3] = {3424073256,1016,3899,463}
World_Boss_Pos[3040334][4] = {3424073256,401,4163,906}
World_Boss_Pos[3040334][5] = {3424073256,495,3055,1013}
World_Boss_Pos[3040334][6] = {3424073256,1185,2911,481}
World_Boss_Pos[3040334][7] = {3424073256,1268,3269,561}
World_Boss_Pos[3040334][8] = {3424073256,1238,4822,987}
World_Boss_Pos[3040334][9] = {3424073256,1154,2785,1125}
World_Boss_Pos[3040334][10] = {3424073256,996,3206,258}
World_Boss_Pos[3040335] = {}
World_Boss_Pos[3040335][1] = {3424072488,1742,3068,778}
World_Boss_Pos[3040335][2] = {3424072488,1690,3100,314}
World_Boss_Pos[3040335][3] = {3424072488,1662,4097,1432}
World_Boss_Pos[3040335][4] = {3424072488,1047,4891,2304}
World_Boss_Pos[3040335][5] = {3424072488,1376,7780,2309}
World_Boss_Pos[3040335][6] = {3424072488,1902,3500,2601}
World_Boss_Pos[3040335][7] = {3424072488,2561,3581,2591}
World_Boss_Pos[3040335][8] = {3424072488,2682,5747,1933}
World_Boss_Pos[3040335][9] = {3424072488,2645,8130,1235}
World_Boss_Pos[3040335][10] = {3424072488,2544,5000,502}
World_Boss_Pos[3040336] = {}
World_Boss_Pos[3040336][1] = {3424075048,1048,10743,2693}
World_Boss_Pos[3040336][2] = {3424075048,1039,10060,2216}
World_Boss_Pos[3040336][3] = {3424075048,742,10577,1740}
World_Boss_Pos[3040336][4] = {3424075048,272,12647,1150}
World_Boss_Pos[3040336][5] = {3424075048,868,9998,209}
World_Boss_Pos[3040336][6] = {3424075048,2783,9774,599}
World_Boss_Pos[3040336][7] = {3424075048,2745,16738,1473}
World_Boss_Pos[3040336][8] = {3424075048,2012,10964,2631}
World_Boss_Pos[3040336][9] = {3424075048,2186,7747,1882}
World_Boss_Pos[3040336][10] = {3424075048,1752,12661,504}

World_Boss_1 = 0
World_Boss_2 = 0
World_Boss_3 = 0
World_Boss_4 = 0
--麒麟刷出地点
Qilin = {}
Qilin[3424075048] = {}
Qilin[3424075048][1] = {768,10753,1713}
Qilin[3424075048][2] = {406,12510,458}
Qilin[3424075048][3] = {2780,9289,349}
Qilin[3424075048][4] = {2768,10196,2142}
Qilin[3424072232] = {}
Qilin[3424072232][1] = {1208,11767,2009}
Qilin[3424072232][2] = {2432,3634,1677}
Qilin[3424072232][3] = {2307,6815,903}
Qilin[3424072232][4] = {1202,2808,442}
Qilin[3424072488] = {}
Qilin[3424072488][1] = {1270,3684,622}
Qilin[3424072488][2] = {2163,2999,1468}
Qilin[3424072488][3] = {775,6226,2048}
Qilin[3424072488][4] = {2136,7200,651}
Qilin_map = 0

function SystemActivity_OnTimerMin(actID)

	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	local curhour = tonumber(os.date("%H"))--小时
	local curmin = tonumber(os.date("%M"))--分钟
	local day = os.date("%A")--星期

--------------------------------------------------魔血巨石的相关脚本-----------------------------------------------------
	for i = 1,6 do
		if moxue_NPC[i].monid == 0 and moxue_NPC[i].time <= 0 then
			moxue_NPC[i].monid = map.MapCreateCreature(moxue_NPC[i].mapid, -1, moxue_NPC[i].npcid, moxue_NPC[i].x, moxue_NPC[i].y, moxue_NPC[i].z)
			unit.AddBuff(moxue_NPC[i].mapid, -1, moxue_NPC[i].monid, 6935801, moxue_NPC[i].monid)
		elseif  moxue_NPC[i].monid == 0 and moxue_NPC[i].time > 0 then
			moxue_NPC[i].time = moxue_NPC[i].time-1
		end
	end

	if curyear ~= 2011 or curmon ~= 1 or curday < 11 or curday > 18 or LaBaZhou_Open == 0 then
	    map.MapDeleteCreature(3424073512, -1, LaBaZhou_NPC)
		LaBaZhou_NPC = nil
	end

-------------------------------------------------------情人节传送NCP刷出删除--------------------------------------------------------

	if (day == "Tuesday" or day == "Thursday" or day == "Saturday" or day == "Sunday") then
		if (curhour >= 18 and curhour <= 24) then
			if qingrenjie_chuansongren == 0 then
				qingrenjie_chuansongren = map.MapCreateCreature(3424073512, -1, 5600026, 634, 23094, 645)-- 情人节传送人做表
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300047)--罗曼村传送人已经出现在斗战天城<634,645>，通过他可以进博博罗曼村
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
			if curmin == 30 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300047)--罗曼村传送人已经出现在斗战天城<634,645>，通过他可以进博博罗曼村
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
		else
			if qingrenjie_chuansongren ~= 0 then
				map.MapDeleteCreature(3424073512, -1, qingrenjie_chuansongren)--时间之外删除情人节传送人
				qingrenjie_chuansongren = 0
			end
		end
	else
		if qingrenjie_chuansongren ~= 0 then
			map.MapDeleteCreature(3424073512, -1, qingrenjie_chuansongren)--时间之外删除情人节传送人
			qingrenjie_chuansongren = 0
		end
	end

--------------------------------------------------上古妖精庇护所相关脚本-------------------------------------------------------
    if curmin == 50 then
	    local mappoint = {}
		mappoint[3424072488] = {x=1094,y=3920,z=1421}
		mappoint[3424072232] = {x=578,y=12973,z=1976}
		mappoint[3424075048] = {x=1618,y=11498,z=2082}
	    SGYJBHS_Gate[1] = map.MapCreateColCreature(3424072488, -1, 5610003, mappoint[3424072488].x, mappoint[3424072488].y, mappoint[3424072488].z)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 440108)    --庇护所之门已经出现在XXX,XXX,它将在XXX分钟后再次消失！
		msg.AddMsgEvent(MsgID, 9, mappoint[3424072488].x)
		msg.AddMsgEvent(MsgID, 9, mappoint[3424072488].z)
		msg.AddMsgEvent(MsgID, 9, 60-curmin)
		msg.DispatchMapMsgEvent(MsgID, 3424072488, -1)
		SGYJBHS_Gate[2] = map.MapCreateColCreature(3424072232, -1, 5610003, mappoint[3424072232].x, mappoint[3424072232].y, mappoint[3424072232].z)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 440108)    --庇护所之门已经出现在XXX,XXX,它将在XXX分钟后再次消失！
		msg.AddMsgEvent(MsgID, 9, mappoint[3424072232].x)
		msg.AddMsgEvent(MsgID, 9, mappoint[3424072232].z)
		msg.AddMsgEvent(MsgID, 9, 60-curmin)
		msg.DispatchMapMsgEvent(MsgID, 3424072232, -1)
		SGYJBHS_Gate[3] = map.MapCreateColCreature(3424075048, -1, 5610003, mappoint[3424075048].x, mappoint[3424075048].y, mappoint[3424075048].z)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 440108)    --庇护所之门已经出现在XXX,XXX,它将在XXX分钟后再次消失！
		msg.AddMsgEvent(MsgID, 9, mappoint[3424075048].x)
		msg.AddMsgEvent(MsgID, 9, mappoint[3424075048].z)
		msg.AddMsgEvent(MsgID, 9, 60-curmin)
		msg.DispatchMapMsgEvent(MsgID, 3424075048, -1)
		SGYJBHS_InstanceID = 100000+SGYJBHS_InstanceID%100000+1
		map.CreateInstance(3373740328, SGYJBHS_InstanceID%100000)
		Tani_table = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26} --2-26
		Trec_table = {27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56} --27-56
		Tyao_table = {57,58,59,60,61,62,63,64,65,66} --57-66
		Tcre1_table = {67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106} --67-106
		Tcre2_table = {137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161} --137-161
		SGYJBHS_From = {}                    --全局变量
	elseif (curmin == 53 or curmin == 56) and SGYJBHS_InstanceID > 10000 then
	    local mappoint = {}
		mappoint[3424072488] = {x=1094,y=3920,z=1421}
		mappoint[3424072232] = {x=578,y=12973,z=1976}
		mappoint[3424075048] = {x=1618,y=11498,z=2082}
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 440108)    --庇护所之门已经出现在XXX,XXX,它将在XXX分钟后再次消失！
		msg.AddMsgEvent(MsgID, 9, mappoint[3424072488].x)
		msg.AddMsgEvent(MsgID, 9, mappoint[3424072488].z)
		msg.AddMsgEvent(MsgID, 9, 60-curmin)
		msg.DispatchMapMsgEvent(MsgID, 3424072488, -1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 440108)    --庇护所之门已经出现在XXX,XXX,它将在XXX分钟后再次消失！
		msg.AddMsgEvent(MsgID, 9, mappoint[3424072232].x)
		msg.AddMsgEvent(MsgID, 9, mappoint[3424072232].z)
		msg.AddMsgEvent(MsgID, 9, 60-curmin)
		msg.DispatchMapMsgEvent(MsgID, 3424072232, -1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 440108)    --庇护所之门已经出现在XXX,XXX,它将在XXX分钟后再次消失！
		msg.AddMsgEvent(MsgID, 9, mappoint[3424075048].x)
		msg.AddMsgEvent(MsgID, 9, mappoint[3424075048].z)
		msg.AddMsgEvent(MsgID, 9, 60-curmin)
		msg.DispatchMapMsgEvent(MsgID, 3424075048, -1)
	elseif curmin == 0 then
	    if SGYJBHS_Gate[1] ~= nil and SGYJBHS_Gate[1] ~= 0 then
			map.MapDeleteCreature(3424072488, -1, SGYJBHS_Gate[1])
			SGYJBHS_Gate[1] = 0
		end
		if SGYJBHS_Gate[2] ~= nil and SGYJBHS_Gate[2] ~= 0 then
			map.MapDeleteCreature(3424072232, -1, SGYJBHS_Gate[2])
			SGYJBHS_Gate[2] = 0
		end
		if SGYJBHS_Gate[3] ~= nil and SGYJBHS_Gate[3] ~= 0 then
			map.MapDeleteCreature(3424075048, -1, SGYJBHS_Gate[3])
			SGYJBHS_Gate[3] = 0
		end

	elseif curmin == 30 and SGYJBHS_InstanceID > 10000 then
	    map.DeleteInstance(3373740328, SGYJBHS_InstanceID%100000)
        SGYJBHS_From = {}                    --全局变量
	end
------------------------------------------------------领域神卷商人----------------------------------------------------------------------

	if curhour % 4 == 0 and curmin == 0 then
		local ran = math.random(100)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 440011)--领域神卷商人已经出现在斗战天城<652,645>，通过他可以获得自选领域
		msg.DispatchMapMsgEvent(MsgID, 3424073512, -1)

		if ran < 50 then
			LYSJSR_ID = map.MapCreateColCreature(3424073512, -1, 5610018, 632, 23094, 771, 1)
		elseif ran < 67 then
			LYSJSR_ID = map.MapCreateColCreature(3424073512, -1, 5610016, 632, 23094, 771, 1)
		elseif ran < 82 then
			LYSJSR_ID = map.MapCreateColCreature(3424073512, -1, 5610017, 632, 23094, 771, 1)
		else
			LYSJSR_ID = map.MapCreateColCreature(3424073512, -1, 5610019, 632, 23094, 771, 1)
		end
	elseif (curhour == 1 or curhour == 5 or curhour == 9 or curhour == 13 or curhour == 17 or curhour == 21) and curmin == 0 then
	    if LYSJSR_ID ~= nil and LYSJSR_ID ~= 0 then
            map.MapDeleteCreature(3424073512, -1, LYSJSR_ID)
			LYSJSR_ID = 0
        end
	end
------------------------------------------------------劳动节--------------------------------------------------------------------------------
	--[[if (curmon == 4 and curday >= 26) or (curmon == 5 and curday <= 10) then
	    if curmin == 0 and (curhour == 10 or curhour == 14 or curhour == 20 or curhour == 23) then
		    landuo_num[3424071976] = 5
			landuo_num[3424071720] = 5
			landuo_num[3424072488] = 5
			xiedai_num[3424071976] = 5
			xiedai_num[3424071720] = 5
			xiedai_num[3424072488] = 5
	        local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300050)    --打败懒惰，击垮懈怠，劳动最光荣！收集奔雷裂谷，北洲天雪，西洲幻林活动怪物掉落的“劳”、“动”、“最”、“光”、“荣”字帖将可以兑换各种奖励！
			msg.DispatchWorldMsgEvent(MsgID)
			for i =1,5 do
			    local ID1 = map.MapCreateColCreature(3424071976, -1, 1100009, laodongjie_pos[3424071976][i*2-1].x, laodongjie_pos[3424071976][i*2-1].y, laodongjie_pos[3424071976][i*2-1].z, 1)
                cre.SetCreatureScriptData(3424071976, -1, ID1, 1, 0, 3424071976)
				cre.SetCreatureScriptData(3424071976, -1, ID1, 1, 1, 2*i-1)
				local ID2 = map.MapCreateColCreature(3424071720, -1, 1100009, laodongjie_pos[3424071720][i*2-1].x, laodongjie_pos[3424071720][i*2-1].y, laodongjie_pos[3424071720][i*2-1].z, 1)
                cre.SetCreatureScriptData(3424071720, -1, ID2, 1, 0, 3424071720)
				cre.SetCreatureScriptData(3424071720, -1, ID2, 1, 1, 2*i-1)
				local ID3 = map.MapCreateColCreature(3424072488, -1, 1100009, laodongjie_pos[3424072488][i*2-1].x, laodongjie_pos[3424072488][i*2-1].y, laodongjie_pos[3424072488][i*2-1].z, 1)
                cre.SetCreatureScriptData(3424072488, -1, ID3, 1, 0, 3424072488)
				cre.SetCreatureScriptData(3424072488, -1, ID3, 1, 1, 2*i-1)
				local ID4 = map.MapCreateColCreature(3424071976, -1, 1100010, laodongjie_pos[3424071976][i*2].x, laodongjie_pos[3424071976][i*2].y, laodongjie_pos[3424071976][i*2].z, 1)
                cre.SetCreatureScriptData(3424071976, -1, ID4, 1, 0, 3424071976)
				cre.SetCreatureScriptData(3424071976, -1, ID4, 1, 1, 2*i)
				local ID5 = map.MapCreateColCreature(3424071720, -1, 1100010, laodongjie_pos[3424071720][i*2].x, laodongjie_pos[3424071720][i*2].y, laodongjie_pos[3424071720][i*2].z, 1)
                cre.SetCreatureScriptData(3424071720, -1, ID5, 1, 0, 3424071720)
				cre.SetCreatureScriptData(3424071720, -1, ID5, 1, 1, 2*i)
				local ID6 = map.MapCreateColCreature(3424072488, -1, 1100010, laodongjie_pos[3424072488][i*2].x, laodongjie_pos[3424072488][i*2].y, laodongjie_pos[3424072488][i*2].z, 1)
                cre.SetCreatureScriptData(3424072488, -1, ID6, 1, 0, 3424072488)
				cre.SetCreatureScriptData(3424072488, -1, ID6, 1, 1, 2*i)
			end
		elseif (curmin == 0 and (curhour == 11 or curhour == 15 or curhour == 21 ))  or (curmin == 59 and curhour == 23) then
		    landuo_num[3424071976] = 70
			landuo_num[3424071720] = 70
			landuo_num[3424072488] = 70
			xiedai_num[3424071976] = 70
			xiedai_num[3424071720] = 70
			xiedai_num[3424072488] = 70
		end
	end]]
-------------------------------------------------------家族召集------------------------------------------------------------------------
    for k,v in pairs(Family_zhaoji) do
	    v[6] = v[6] - 1
		if v[6] <= 0 then
		    Family_zhaoji[k] = nil
		else
		    Family_zhaoji[k][6] = 1
		end
	end
-------------------------------------------------------修炼场------------------------------------------------------------------------
	--LxC: 这几张试炼场地图都不存在……
    local RoleTbl1 = map.GetMapAllRoleID(2748433913, -1)
	local RoleTbl2 = map.GetMapAllRoleID(2748434169, -1)
	local RoleTbl3 = map.GetMapAllRoleID(2748434425, -1)
	if RoleTbl1 ~= nil then
		for k,v in pairs(RoleTbl1) do
			local time1 = role.GetRoleScriptData(v, 1, RoleDataType["XiuLianChang_Time"])
			if time1 == 0 then
				local MapID, InstanceID = role.GetRoleMapID(v)
				role.RoleGotoNewMap(MapID, InstanceID, v, 3424073512, 576, 462, 712)
			else
				if time1 == 30 or time1 == 10 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 605018) 		--您所能修炼的时间已经不足XX分钟，请及时购买门票延长时间，否则您将会在时间用尽后被传送至斗战天城。
					msg.AddMsgEvent(MsgID, 9, time1) 		--npcid
					msg.DispatchRoleMsgEvent(v, MsgID)
				end
				role.SetRoleScriptData(v, 1, RoleDataType["XiuLianChang_Time"],time1-1)
				--[[local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 16,	28)								--
				msg.AddMsgEvent(MsgID, 16,	(time1-1)*60)							--剩余时间
				msg.DispatchRoleMsgEvent(k, MsgID)]]
			end
		end
	end
	if RoleTbl2 ~= nil then
		for k,v in pairs(RoleTbl2) do
			local time1 = role.GetRoleScriptData(v, 1, RoleDataType["XiuLianChang_Time"])
			if time1 == 0 then
				local MapID, InstanceID = role.GetRoleMapID(v)
				role.RoleGotoNewMap(MapID, InstanceID, v, 3424073512, 576, 462, 712)
			else
				if time1 == 30 or time1 == 10 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 605018) 		--您所能修炼的时间已经不足XX分钟，请及时购买门票延长时间，否则您将会在时间用尽后被传送至斗战天城。
					msg.AddMsgEvent(MsgID, 9, time1) 		--npcid
					msg.DispatchRoleMsgEvent(v, MsgID)
				end
				role.SetRoleScriptData(v, 1, RoleDataType["XiuLianChang_Time"],time1-1)
				--[[local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 16,	28)								--
				msg.AddMsgEvent(MsgID, 16,	(time1-1)*60)							--剩余时间
				msg.DispatchRoleMsgEvent(k, MsgID)]]
			end
		end
	end
	if RoleTbl3 ~= nil then
		for k,v in pairs(RoleTbl3) do
			local time1 = role.GetRoleScriptData(v, 1, RoleDataType["XiuLianChang_Time"])
			if time1 == 0 then
				local MapID, InstanceID = role.GetRoleMapID(v)
				role.RoleGotoNewMap(MapID, InstanceID, v, 3424073512, 576, 462, 712)
			else
				if time1 == 30 or time1 == 10 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 605018) 		--您所能修炼的时间已经不足XX分钟，请及时购买门票延长时间，否则您将会在时间用尽后被传送至斗战天城。
					msg.AddMsgEvent(MsgID, 9, time1) 		--npcid
					msg.DispatchRoleMsgEvent(v, MsgID)
				end
				role.SetRoleScriptData(v, 1, RoleDataType["XiuLianChang_Time"],time1-1)
				--[[local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 16,	28)								--
				msg.AddMsgEvent(MsgID, 16,	(time1-1)*60)							--剩余时间
				msg.DispatchRoleMsgEvent(k, MsgID)]]
			end
		end
	end

	if curhour % 2 == 1 and curmin == 0 then
	    for k,v in pairs(XiuLian_Fire) do
		    for k1,v1 in pairs(v) do
				map.MapDeleteCreature(k, -1, v1)
			end
		end
		XiuLian_Fire = {}
	elseif curmin == 0 then
	    map.SetMapScriptData(2748433913, -1,1,0,0)
		map.SetMapScriptData(2748433913, -1,1,1,0)
		map.SetMapScriptData(2748434169, -1,1,0,0)
		map.SetMapScriptData(2748434169, -1,1,1,0)
		map.SetMapScriptData(2748434425, -1,1,0,0)
		map.SetMapScriptData(2748434425, -1,1,1,0)
		XiuLian_Fire = {}
	    for i = 1,20 do
		    map.MapCreateColCreature(2748433913, -1, 1099301, XiuLian_Pos[i].x, XiuLian_Pos[i].y, XiuLian_Pos[i].z, 1)
			map.MapCreateColCreature(2748434169, -1, 1099304, XiuLian_Pos[i].x, XiuLian_Pos[i].y, XiuLian_Pos[i].z, 1)
			map.MapCreateColCreature(2748434425, -1, 1099307, XiuLian_Pos[i].x, XiuLian_Pos[i].y, XiuLian_Pos[i].z, 1)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 605019)      --炼魂殿灵已经出现！击杀炼魂殿灵将可以获得魂水晶兑换高额经验
		msg.DispatchMapMsgEvent(MsgID, 2748433913, -1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 605019)      --炼魂殿灵已经出现！击杀炼魂殿灵将可以获得魂水晶兑换高额经验
		msg.DispatchMapMsgEvent(MsgID, 2748434169, -1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 605019)      --炼魂殿灵已经出现！击杀炼魂殿灵将可以获得魂水晶兑换高额经验
		msg.DispatchMapMsgEvent(MsgID, 2748434425, -1)
	end
---------------------------------------世界BOSS-------------------------------------------------------------
	local BOSS_index = math.random(1,4)
	local TypeID = 0
	local index = 0
	if curmin == 0 then
		if curhour == 18 then
			if BOSS_index == 1 then
				TypeID = 3040333
				index = math.random(10)
				World_Boss_1 = map.MapCreateColCreature(World_Boss_Pos[TypeID][index][1], -1, TypeID, World_Boss_Pos[TypeID][index][2], World_Boss_Pos[TypeID][index][3], World_Boss_Pos[TypeID][index][4], 1)
                World_Boss_TypeID = TypeID
				World_Boss_index = index
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101,206485)      --XXX刷新在XXX的xXX，XXX，三界的勇士们啊，战斗吧！
				msg.AddMsgEvent(MsgID, 5, TypeID)
				msg.AddMsgEvent(MsgID, 6, World_Boss_Pos[TypeID][index][1])
				msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[TypeID][index][2])
				msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[TypeID][index][4])
				msg.DispatchWorldMsgEvent(MsgID)
			elseif BOSS_index == 2 then
				TypeID = 3040336
				index = math.random(10)
				World_Boss_2 = map.MapCreateColCreature(World_Boss_Pos[TypeID][index][1], -1, TypeID, World_Boss_Pos[TypeID][index][2], World_Boss_Pos[TypeID][index][3], World_Boss_Pos[TypeID][index][4], 1)
                World_Boss_TypeID = TypeID
				World_Boss_index = index
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101,206485)      --XXX刷新在XXX的xXX，XXX，三界的勇士们啊，战斗吧！
				msg.AddMsgEvent(MsgID, 5, TypeID)
				msg.AddMsgEvent(MsgID, 6, World_Boss_Pos[TypeID][index][1])
				msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[TypeID][index][2])
				msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[TypeID][index][4])
				msg.DispatchWorldMsgEvent(MsgID)
			elseif BOSS_index == 3 then
				TypeID = 3040335
				index = math.random(10)
				World_Boss_3 = map.MapCreateColCreature(World_Boss_Pos[TypeID][index][1], -1, TypeID, World_Boss_Pos[TypeID][index][2], World_Boss_Pos[TypeID][index][3], World_Boss_Pos[TypeID][index][4], 1)
                World_Boss_TypeID = TypeID
				World_Boss_index = index
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101,206485)      --XXX刷新在XXX的xXX，XXX，三界的勇士们啊，战斗吧！
				msg.AddMsgEvent(MsgID, 5, TypeID)
				msg.AddMsgEvent(MsgID, 6, World_Boss_Pos[TypeID][index][1])
				msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[TypeID][index][2])
				msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[TypeID][index][4])
				msg.DispatchWorldMsgEvent(MsgID)
			elseif BOSS_index == 4 then
				TypeID = 3040334
				index = math.random(10)
				World_Boss_4 = map.MapCreateColCreature(World_Boss_Pos[TypeID][index][1], -1, TypeID, World_Boss_Pos[TypeID][index][2], World_Boss_Pos[TypeID][index][3], World_Boss_Pos[TypeID][index][4], 1)
                World_Boss_TypeID = TypeID
				World_Boss_index = index
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101,206485)      --XXX刷新在XXX的xXX，XXX，三界的勇士们啊，战斗吧！
				msg.AddMsgEvent(MsgID, 5, TypeID)
				msg.AddMsgEvent(MsgID, 6, World_Boss_Pos[TypeID][index][1])
				msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[TypeID][index][2])
				msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[TypeID][index][4])
				msg.DispatchWorldMsgEvent(MsgID)
			end
		elseif curhour == 17 then
			if World_Boss_1 ~= 0 then
				map.MapDeleteCreature(World_Boss_Pos[3040333][1][1], -1, World_Boss_1)
				World_Boss_1 = 0
			elseif World_Boss_2 ~= 0 then
				map.MapDeleteCreature(World_Boss_Pos[3040336][1][1], -1, World_Boss_2)
				World_Boss_2 = 0
			elseif World_Boss_3 ~= 0 then
				map.MapDeleteCreature(World_Boss_Pos[3040335][1][1], -1, World_Boss_3)
				World_Boss_3 = 0
			elseif World_Boss_4 ~= 0 then
				map.MapDeleteCreature(World_Boss_Pos[3040334][1][1], -1, World_Boss_4)
				World_Boss_4 = 0
			end
		end
	end
	if World_Boss_1 ~= 0 or World_Boss_2 ~= 0 or World_Boss_3 ~= 0 or World_Boss_4 ~= 0 then
		if math.mod(curmin,10) == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101,206491)      --XXX已经出现XXX的xXX，XXX，消灭它之后可以交付任务，获得丰厚奖励
			msg.AddMsgEvent(MsgID, 5, World_Boss_TypeID)
			msg.AddMsgEvent(MsgID, 6, World_Boss_Pos[World_Boss_TypeID][World_Boss_index][1])
			msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[World_Boss_TypeID][World_Boss_index][2])
			msg.AddMsgEvent(MsgID, 9, World_Boss_Pos[World_Boss_TypeID][World_Boss_index][4])
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end
----------------------------------------悬赏榜BUFF--------------------------------------------------------
    if XuanShangBUFF_Open == 1 and (curhour == 20 or curhour == 21) then
	    if curmin%5 == 0 then
		    act.AddAllRoleBuff(6104401)
		end
		if curmin%30 == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102,300051)      --20：00-22：00之间，每隔5分钟系统将会为所有在线玩家添加一个可叠加的状态，该状态最高可以将悬赏榜任务的经验和金钱奖励提高一倍！
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end
-------------------------------------------中秋活动------------------------------------------------------------
	if MidautumnDay == 1 and curmon == 9 and curday < 20 then
		--台湾部分
		if mid_autumnNPC == 0 then
			mid_autumnNPC = map.MapCreateCreature(3424073512, -1, 5600066, 603, 23094, 683)
			mid_autumnNPC = map.MapCreateCreature(3424073512, -1, 5600067, 597, 23094, 687)
		end
		if curmin%2 == 0 then
			if curLajiao ~= nil then
				for k,v in pairs(curLajiao) do
					local recId = map.MapCreateColCreature(lajiao[v].map, -1, 5600068, lajiao[v].x, lajiao[v].y, lajiao[v].z)
					cre.SetCreatureScriptData(lajiao[v].map, -1, recId, 1, 0, v)
					table.remove(curLajiao, k)
				end
			end
			if curYutu ~= nil then
				for k,v in pairs(curYutu) do
					local recId = map.MapCreateColCreature(yutu[v].map, -1, yutu[v].creid, yutu[v].x, yutu[v].y, yutu[v].z)
					cre.SetCreatureScriptData(yutu[v].map, -1, recId, 1, 0, v)
					table.remove(curYutu, k)
				end
			end
		end
		--大陆部分
		if curhour >= 20 and curhour <= 22 then
			if chuansongrenIn == 0 then
				chuansongrenIn = map.MapCreateColCreature(3424073512, -1, 5600069, 591, 23094, 639)
				map.CreateInstance(3373743400, 1)
				map.MapCreateColCreature(3373743400, 1, 5600070, 1047, 5855, 1033)
				map.MapCreateColCreature(3373743400, 1, 5600071, 698, 948, 682)
				map.MapCreateColCreature(3373743400, 1, 5600072, 684, 1105, 698)
				map.MapCreateColCreature(3373743400, 1, 5600074, 697, 1126, 696)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300052)	--映月塘传送人
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
			if curmin == 20 then
				--场景公告：道具提交时间是当日20-22:30，过时无法提交，详情咨询御兔童子
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300053)
				msg.DispatchMapMsgEvent(MsgID, 3373743400, 1)
			end
			if curhour == 22 and curmin == 30 then
				for i=0,5 do
					act.SetActScriptData(44, 1, i+6, act.GetActScriptData(44, 1, i))
					act.SetActScriptData(44, 1, i, 0)
					act.SaveActScriptData(44)
				end
				for i=9,11 do
					act.SetActScriptData(44, 1, i+3, act.GetActScriptData(44, 1, i))
					act.SaveActScriptData(44)
				end
				--全服广播前三名，xx,xx,xx
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300054)
				msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(44,1,11))
				msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(44,1,10))
				msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(44,1,9))
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
			if curhour == 22 and curmin == 59 then
				--提示场景关闭
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300056)
				msg.DispatchMapMsgEvent(MsgID, 3373743400, 1)
			end
			if curS58pos ~= nil then
				for k,v in pairs(curS58pos) do
					local recId = map.MapCreateColCreature(3373743400, 1, 5600073, S58pos[v].x, S58pos[v].y, S58pos[v].z)
					cre.SetCreatureScriptData(3373743400, 1, recId, 1, 0, v)
					table.remove(curS58pos, k)
				end
			end
			if curmin%10 == 0 then
				curNum = 0
			end
		elseif chuansongrenIn ~= 0 then
			map.MapDeleteCreature(3424073512, -1, chuansongrenIn)
			map.CreateInstance(3373743400, 1)
			map.DeleteInstance(3373743400, 1)
			chuansongrenIn = 0
		end
	end
	----------------------------------------国庆活动--------------------------------------------------------
    if guoqingshizhe_id ~= 0 and guoqingshizhe_id ~= nil  then
	    if( curmon>=10 and curday >=18) then
			map.MapDeleteCreature(3424073512, -1, guoqingshizhe_id)
			guoqingshizhe_id=nil
		end
	end
	if guoqingshangdian_id ~= 0 and guoqingshangdian_id ~= nil  then
	    if( curmon>=10 and curday >=18) then
			map.MapDeleteCreature(3424073512, -1, guoqingshangdian_id)
			guoqingshangdian_id=nil
		end
	end
------------------------------------------------------感恩节--------------------------------------------------------------------------------
	if ((curyear == 2011 and curmon == 12 and curday >= 20) or (curyear == 2012 and curmon == 1 and curday <= 3)) then
		ganenjie(curhour,curmin)
	end
------------------------------------------------------领取水晶心--------------------------------------------------------------------------------
	--珂瑞斯特尔（领取水晶心NPC）
	if (crystal == 1)then
		if curhour == 20 and curmin == 0 and crystalNPC == 0 then
			CreateCrystalNPC()
		end
		if curhour == 22 and curmin == 0 and crystalNPC ~= 0 then
			DeleteCrystalNPC()
		end
	end
------------------------------------------------------祈福活动节--------------------------------------------------------------------------------
	if Ganenxianling_open == 1 then
		if curhour == 20 and curmin == 30 and (longshenxianlingId == nil or longshenxianlingId == 0) then
			ganenjieshuaxin()
		elseif curhour == 21 and curmin == 10 and longshenxianlingId ~= nil then
			local MsgID2 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID2, 102, 206916)--祈福结束的玩家可以去领取礼包了
			msg.DispatchBroadcast(MsgID2,-1,-1,-1)
		elseif curhour == 21 and curmin == 30 and longshenxianlingId ~= nil then
			ganenjieshanchu()
		end
	end
	if Ganenshizhe_open == 1 then
		if curhour >= 20 and curhour <= 22 then
			ganenjshizhe(curhour,curmin)
		end
	end
------------------------------------------------------跨服PK--------------------------------------------------------------------------------
	--[[if curyear == 2012 and ((curmon == 3 and curday >= 6) or curmon > 3) and curhour == 19 and curmin == 0 then
		KuaFuPKchsr = map.MapCreateColCreature(3424073512, -1, 5610106,266,23692,720, 1)
		KuaFuPKtalk = false
	end]]
------------------------------------------------------妖精秘境--------------------------------------------------------------------------------
	if ((curhour == 9 and curmin == 0) or (curhour == 12 and curmin == 0) or (curhour == 15 and curmin == 0) or (curhour == 18 and curmin == 0) or (curhour == 21 and curmin == 0)) then
		s62_Creature(curhour)

	end
	if((curhour == 10 and curmin == 0) or (curhour == 13 and curmin == 0) or (curhour == 16 and curmin == 0) or (curhour == 19 and curmin == 0) or (curhour == 22 and curmin == 0)) then
		s62_delete(curhour)
	end
	--------------------------------------资料片：通灵世界-----------------------------------------------------
	if ZLP_start == 1 then
		if 20 == curhour and (curmin == 0 or curmin == 15) then
			--函数
			ZLP_main()
		end
	end

	--------------------------------------资料片：通灵世界-----------------------------------------------------

	if day == "Wednesday" or day == "Sunday" then
		if 20 == curhour and curmin == 50 then
		    Tianxuanzhe_baoming = 1
			msg.SendWorldSwitchMsg(3, 50, 100, 1, 47)
		elseif 20 == curhour and (curmin == 52 or curmin == 55) then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300085) --天选者之战活动已经开始报名，50级以上玩家可以在斗神统领加德（斗战天城【381，330】）处报名并购买天选者套装
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
		end
	end

	--------------------------------------资料片：古神陵墓各boss刷新-----------------------------------
	if curmin == 0 then
		if curGodcrenum == 0 then
			ZLP_godlittleboss()
			curGodcrenum = curGodcrenum + 1
			if math.random(2) < 2 then
				ZLP_godlittleboss()
				curGodcrenum = curGodcrenum + 1
			end
		elseif curGodcrenum == 1 then
			if math.random(2) < 2 then
				ZLP_godlittleboss()
				curGodcrenum = curGodcrenum + 1
			end
		end
		if curEvilcrenum == 0 then
			ZLP_evillittleboss()
			curGodcrenum = curGodcrenum + 1
			if math.random(2) < 2 then
				ZLP_evillittleboss()
				curEvilcrenum = curEvilcrenum + 1
			end
		elseif curEvilcrenum == 1 then
			if math.random(2) < 2 then
				ZLP_evillittleboss()
				curEvilcrenum = curEvilcrenum + 1
			end
		end
	end
	if curhour >= 20 and curhour <= 21 then
		if todownstairs == 0 then
			map.CreateInstance(52649718, 1)
			todownstairs = map.MapCreateColCreature(52648438, 1, 5610070, 250, 10400, 257, 1, "")
			toupstairs = map.MapCreateColCreature(52649718, 1, 5610071, 170, 258, 40, 1, "")
			ZLP_godboss()
		end
	else
		if todownstairs ~= 0 or toupstairs ~= 0 then
			map.MapDeleteCreature(52648438, 1, todownstairs)
			map.MapDeleteCreature(52649718, 1, toupstairs)
			map.DeleteInstance(52649718, 1)
			todownstairs = 0
			toupstairs = 0
		end
	end
	if curhour >= 19 and curhour <= 22 then
		if Evilbosshour == -1 and Evilbossmin == -1 then
			Evilbosshour = math.random(curhour,22)
			if curhour ~= Evilbosshour then
				Evilbossmin = math.random(0,59)
			else
				Evilbossmin = math.random(curmin,59)
				if curmin == 59 then
					Evilbossmin = 0
					Evilbosshour = Evilbosshour + 1
				else
					Evilbossmin = Evilbossmin + 1
				end
			end
		else
			if Evilbosshour == curhour and Evilbossmin == curmin then
				ZLP_evilboss()
			end
		end
	else
		if Evilbosshour ~= -1 or Evilbossmin ~= -1 then
			Evilbosshour = -1
			Evilbossmin = -1
		end
	end
	--------------------------------------金币总动员-----------------------------------
	if JBZDY_Open == 1 and curhour == 21 and curmin <= 30 and curmin % 5 == 0 then
		if curmin ~= 30 and curmin % 5 == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300086)		--9：30将会抽出3位幸运者平分金币总动员奖金。如果开出小奖，则每位幸运者将获得xxx金；如果开出大奖，则可以获得XXX金！
			msg.AddMsgEvent(MsgID, 9, math.floor(act.GetActScriptData(29, 1, benrijiangli_JBZDY)/3*0.95))
			msg.AddMsgEvent(MsgID, 9, math.floor((act.GetActScriptData(29, 1, jiangchi_JBZDY)+act.GetActScriptData(29, 1, benrijiangli_JBZDY))/3*0.95))
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 206754)		--9：30将会抽出3位幸运者平分金币总动员奖金。如果开出小奖，则每位幸运者将获得xxx金；如果开出大奖，则可以获得XXX金！
			msg.AddMsgEvent(MsgID, 9, math.floor(act.GetActScriptData(29, 1, benrijiangli_JBZDY)/3*0.95))
			msg.AddMsgEvent(MsgID, 9, math.floor((act.GetActScriptData(29, 1, jiangchi_JBZDY)+act.GetActScriptData(29, 1, benrijiangli_JBZDY))/3*0.95))
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
		elseif curmin ~= 30 and curmin % 10 == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300086)		--9：30将会抽出3位幸运者平分金币总动员奖金。如果开出小奖，则每位幸运者将获得xxx金；如果开出大奖，则可以获得XXX金！
			msg.AddMsgEvent(MsgID, 9, math.floor(act.GetActScriptData(29, 1, benrijiangli_JBZDY)/3*0.95))
			msg.AddMsgEvent(MsgID, 9, math.floor((act.GetActScriptData(29, 1, jiangchi_JBZDY)+act.GetActScriptData(29, 1, benrijiangli_JBZDY))/3*0.95))
		    msg.DispatchBroadcast(MsgID,-1,-1,-1)
		elseif curmin == 30 then
		    local function GetBigOrSmall()
			    local CurTime = tonumber(os.date("%j"))
			    if #tbl_JBZDY_Online < 200 then --在线小于200，不开大奖
				    return "small"
				end
				if JBZDY_Limit ~= -1 then --and CurTime <= JBZDY_Limit then
					if JBZDY_Limit < 40 and CurTime > 320 then
						return "small"
					end
					if CurTime <= JBZDY_Limit then
						return "small"
					end
				end
				if JBZDY_SetDay ~= -1 then
				    if act.GetActScriptData(29, 1, benrijiangli_JBZDY_SetDay) ~= JBZDY_SetDay then
						if CurTime > JBZDY_SetDay then
                            if math.random(100) <= JBZDY_Nomal + (CurTime-JBZDY_SetDay)*JBZDY_SetDay_Step then
							    act.SetActScriptData(29, 1, benrijiangli_JBZDY_SetDay,JBZDY_SetDay)
								act.SaveActScriptData(29)
								return "big"
							end
						end
						if JBZDY_SetDay > 320 and CurTime < 40 then
							if CurTime < JBZDY_SetDay then
							    if math.random(100) <= JBZDY_Nomal + (CurTime+365-JBZDY_SetDay)*JBZDY_SetDay_Step then
									act.SetActScriptData(29, 1, benrijiangli_JBZDY_SetDay,JBZDY_SetDay)
									act.SaveActScriptData(29)
									return "big"
								end
							end
						end
					end
				end
				if math.random(100) <= JBZDY_Nomal then
				    return "big"
				else
				    return "small"
				end
			end
			local Player = {}
			local money = 0
			local char = GetBigOrSmall()
			if char == "small" then
			    money = math.floor(act.GetActScriptData(29, 1, benrijiangli_JBZDY)/3*0.95)
			else
			    money = math.floor((act.GetActScriptData(29, 1, jiangchi_JBZDY)+act.GetActScriptData(29, 1, benrijiangli_JBZDY))/3*0.95)
			end
			local tbl_player = {}
			local total = 0
			for k,v in pairs(tbl_JBZDY_Online) do
			    table.insert(tbl_player,v)
				total = total + 1
			end
			for k,v in pairs(tbl_JBZDY_Offline) do
			    table.insert(tbl_player,v)
				table.insert(tbl_player,v)
				--table.insert(tbl_player,v)
				total = total + 1
			end
			if total >= 3 then
				for i=1,3 do
				    local index = math.random(#tbl_player)
					Player[i] = tbl_player[index]
					if index ~= #tbl_player then
					    if tbl_player[index] == tbl_player[index+1] then
						    table.remove(tbl_player,index)
							table.remove(tbl_player,index+1)
						end
					end
					if index ~= 1 and Player[i] == tbl_player[index] then
					    if tbl_player[index] == tbl_player[index-1] then
						    table.remove(tbl_player,index)
							table.remove(tbl_player,index-1)
						end
					end
					if Player[i] == tbl_player[index] then
					    table.remove(tbl_player,index)
					end
				end
			elseif total >= 2 then
			    for i=1,2 do
				    local index = math.random(#tbl_player)
					Player[i] = tbl_player[index]
					if index ~= #tbl_player then
					    if tbl_player[index] == tbl_player[index+1] then
						    table.remove(tbl_player,index)
							table.remove(tbl_player,index+1)
						end
					end
					if index ~= 1 and Player[i] == tbl_player[index] then
					    if tbl_player[index] == tbl_player[index-1] then
						    table.remove(tbl_player,index)
							table.remove(tbl_player,index-1)
						end
					end
					if Player[i] == tbl_player[index] then
					    table.remove(tbl_player,index)
					end
				end
			elseif total >= 1 then
			    local index = math.random(#tbl_player)
				Player[1] = tbl_player[index]
				if index ~= #tbl_player then
					if tbl_player[index] == tbl_player[index+1] then
						table.remove(tbl_player,index)
						table.remove(tbl_player,index+1)
					end
				end
				if index ~= 1 and Player[1] == tbl_player[index] then
					if tbl_player[index] == tbl_player[index-1] then
						table.remove(tbl_player,index)
						table.remove(tbl_player,index-1)
					end
				end
				if Player[1] == tbl_player[index] then
					table.remove(tbl_player,index)
				end
			end
			act.SetActScriptData(29, 1, benrijiangli_Index,act.GetActScriptData(29, 1, benrijiangli_Index)+1)
			act.SetActScriptData(29, 1, benrijiangli_JBZDY, 0)
			if char == "small" then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300088)		--本次金币总动员未能开出大奖，&lt;p1&gt;、&lt;p2&gt;、&lt;p3&gt;三位玩家每人获得奖金&lt;p4&gt;金。
				msg.AddMsgEvent(MsgID, 2, Player[1])
				msg.AddMsgEvent(MsgID, 2, Player[2])
				msg.AddMsgEvent(MsgID, 2, Player[3])
				msg.AddMsgEvent(MsgID, 9, money)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
				for i = 1,3 do
				    local temp = 0
				    if Player[i] == nil then
					    act.SetActScriptData(29, 1, jiangchi_JBZDY,act.GetActScriptData(29, 1, jiangchi_JBZDY)+money)
                    else
					    for k,v in pairs(tbl_JBZDY_Offline) do
                            if 	v ==  Player[i] then
								act.SetActScriptData(29, 1, jiangchi_JBZDY,act.GetActScriptData(29, 1, jiangchi_JBZDY)+money)
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 102, 300090)		--&lt;p1&gt;未能在线领取金币奖励，&lt;p2&gt;金将会被累积到下次金币总动员的奖励基金中！
								msg.AddMsgEvent(MsgID, 2, Player[i])
								msg.AddMsgEvent(MsgID, 9, money)
								msg.DispatchBroadcast(MsgID,-1,-1,-1)
								temp = 1
								break
							end
						end
						if temp == 0 then
						    local MapID, InstanceID = role.GetRoleMapID(Player[i])
							if MapID ~= nil then
							    role.AddRoleSilver(MapID, InstanceID, Player[i], money*10000, 101)
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 71,	206755)
								msg.AddMsgEvent(MsgID, 9,	money)		--“确定”
								msg.DispatchRoleMsgEvent(Player[i], MsgID)
							end
						end
					end
				end
			else
			    act.SetActScriptData(29, 1, jiangchi_JBZDY, 0)
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300089)		--本次金币总动员未能开出大奖，&lt;p1&gt;、&lt;p2&gt;、&lt;p3&gt;三位玩家每人获得奖金&lt;p4&gt;金。
				msg.AddMsgEvent(MsgID, 2, Player[1])
				msg.AddMsgEvent(MsgID, 2, Player[2])
				msg.AddMsgEvent(MsgID, 2, Player[3])
				msg.AddMsgEvent(MsgID, 9, money)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
				for i = 1,3 do
				    local temp = 0
				    if Player[i] == nil then
					    act.SetActScriptData(29, 1, jiangchi_JBZDY,act.GetActScriptData(29, 1, jiangchi_JBZDY)+money)
                    else
					    for k,v in pairs(tbl_JBZDY_Offline) do
                            if 	v ==  Player[i] then
								act.SetActScriptData(29, 1, jiangchi_JBZDY,act.GetActScriptData(29, 1, jiangchi_JBZDY)+money)
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 102, 300090)		--&lt;p1&gt;未能在线领取金币奖励，&lt;p2&gt;金将会被累积到下次金币总动员的奖励基金中！
								msg.AddMsgEvent(MsgID, 2, Player[i])
								msg.AddMsgEvent(MsgID, 9, money)
								msg.DispatchBroadcast(MsgID,-1,-1,-1)
								temp = 1
								break
							end
						end
						if temp == 0 then
						    local MapID, InstanceID = role.GetRoleMapID(Player[i])
							if MapID ~= nil then
								role.AddRoleSilver(MapID, InstanceID, Player[i], money*10000, 101)
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 71,	206755)
								msg.AddMsgEvent(MsgID, 9,	money)		--“确定”
								msg.DispatchRoleMsgEvent(Player[i], MsgID)
							end
						end
					end
				end
			end
			act.SetActScriptData(29, 1, jiangchi_JBZDY,act.GetActScriptData(29, 1, jiangchi_JBZDY)+math.floor(JBZDY_SystemGold*0.3))
			act.SetActScriptData(29, 1, benrijiangli_JBZDY, math.floor(JBZDY_SystemGold*0.7))
			act.SetActScriptData(29, 1, benrijiangli_JBZDY_LastGold,money)
			tbl_JBZDY_Online = {}
			tbl_JBZDY_Offline = {}
			act.SaveActScriptData(29)
		end
	end
	---------------------------------幸运之神---------------------------------------
	if lucky == 1 then
		if(curhour == 21 and curmin == 0) then
			dianmingIni()
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 209001)
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
		end
		if(curhour == 2 and curmin == 0) then
			GetGiftRoleID = {}
		end
		if (curmon == 1 and curday == 22) or (curmon ==2 and curday == 6) or (curmon == 2 and curday == 14) then
			if(curhour == 21 and curmin ~= 0 and curmin%10 == 0) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 209003)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			elseif(curhour == 22 and curmin%10 == 0) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 209004)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			elseif(curhour == 23 and curmin%10 == 0) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 209005)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
			if(curhour==21 and curmin ~= 0 and curmin%2 == 0) or (curhour >= 22 and curhour<= 23 and curmin%2 == 0) or (curhour== 23 and curmin == 59)then
				dianmingSpe(curhour,curmin)
			end
		else
			if(curhour == 21 and curmin ~= 0 and curmin%10 == 0) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 209002)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
			if((curhour == 21 and curmin ~= 0 and curmin%2 == 0) or (curhour== 22 and curmin ==0)) then
				dianmingNor(curhour,curmin)
			end
		end
	end
	---------------------------------神器BOSS状态---------------------------------------
	if curhour%6 == 0 and curmin == 0 then
		if SQ_boss1 == 1 then
			local cid1 = map.MapCreateColCreature(3424071720, -1, 3040378, 2550, 14346, 3287, 1, "")
			unit.AddBuff(3424071720, -1, cid1, 6105801, cid1)
			SQ_boss1 = 0
		end
		if SQ_boss2 == 1 then
			local cid2 = map.MapCreateColCreature(3424072488, -1, 3040376, 593, 7050, 445, 1, "")
			unit.AddBuff(3424072488, -1, cid2, 6105801, cid2)
			SQ_boss2 = 0
		end
		if SQ_boss3 == 1 then
			local cid3 = map.MapCreateColCreature(3424072232, -1, 3040377, 2256, 4919, 1897, 1, "")
			unit.AddBuff(3424072232, -1, cid3, 6105801, cid3)
			SQ_boss3 = 0
		end
		if SQ_boss4 == 1 then
			local cid4 = map.MapCreateColCreature(3424075048, -1, 3040379, 904, 10460, 2603, 1, "")
			unit.AddBuff(3424075048, -1, cid4, 6105801, cid4)
			SQ_boss4 = 0
		end
	end
	---------------------------------麒麟刷新3041153---------------------------------------
	--[[if curmin == 43 then
		local r = math.random(1,3)
		local Qilin_map1 = {}
		Qilin_map1[1]={3424075048}
		Qilin_map1[2]={3424072232}
		Qilin_map1[3]={3424072488}

		Qilin_map = Qilin_map1[r][1]

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102,300097)      --一道赤红的光芒在XXX场景闪动，这是赤红麒麟即将现身的征兆。它仅仅存在这个世界十分钟，大家快去寻找吧。
		msg.AddMsgEvent(MsgID, 6, Qilin_map)
		msg.DispatchWorldMsgEvent(MsgID)
	end
	if curmin == 45 then
		local r = math.random(1,4)	
		
		map.MapCreateColCreature(Qilin_map, -1, 3041153, Qilin[Qilin_map][r][1], Qilin[Qilin_map][r][2], Qilin[Qilin_map][r][3],1, "")
		
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102,300098)      --赤红麒麟出现在XXX场景的(x,y)，收集足够的赤红之鳞可以在斗战天城兑换各种道具。
		msg.AddMsgEvent(MsgID, 6, Qilin_map)
		msg.AddMsgEvent(MsgID, 9, Qilin[Qilin_map][r][1])
		msg.AddMsgEvent(MsgID, 9, Qilin[Qilin_map][r][3])
		msg.DispatchWorldMsgEvent(MsgID)
	end]]
	---------------------------------圣灵BOSS刷出---------------------------------------
	if curmin == 0 and (curhour == 10 or curhour == 13 or curhour == 17 or curhour == 23) then
		local r = math.random(1,6)
		local t = math.random(1,4)	
		SLBOSSID = {}
		SLBOSSID[1] = {3041154}
		SLBOSSID[2] = {3041155}
		SLBOSSID[3] = {3041156}
		SLBOSSID[4] = {3041157}
		--圣灵刷出地点
		Sl_boss = {}
		Sl_boss[1] = {3424075048,518,11330,2685}
		Sl_boss[2] = {3424075048,2786,9802,590}
		Sl_boss[3] = {3424072232,834,13240,1026}
		Sl_boss[4] = {3424072232,2084,9120,2609}
		Sl_boss[5] = {3424072488,1946,6000,1768}
		Sl_boss[6] = {3424072488,2647,8130,1214}	
		map.MapCreateColCreature(Sl_boss[r][1], -1, SLBOSSID[t][1], Sl_boss[r][2], Sl_boss[r][3], Sl_boss[r][4],1, "")
		
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102,300099)      
		msg.AddMsgEvent(MsgID, 6, Sl_boss[r][1])
		msg.AddMsgEvent(MsgID, 9, Sl_boss[r][2])
		msg.AddMsgEvent(MsgID, 9, Sl_boss[r][4])
		msg.DispatchWorldMsgEvent(MsgID)
	end
	---------------------------------仲夏夜祈福活动---------------------------------------
	if (curyear == 2012 and curmon == 7 and curday >= 17 and curday <= 31 and curhour == 20 and  curmin >= 30) or (curyear == 2012 and curmon == 7 and curday >= 17 and curday <= 31 and curhour == 21 and curmin <= 30)then
		if zhongxiayeshizhe == 0 then 
			zhongxiayeshizhe = map.MapCreateColCreature(3424073512, -1, 6011056, 615,23510,868, 1, "")
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102,300103)      
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end
	if curyear == 2012 and curmon >= 7 and curday >= 17 and curday <= 31 and curhour == 21 and curmin > 30 then 
		if zhongxiayeshizhe ~= 0 then 
			map.MapDeleteCreature(3424073512, -1, zhongxiayeshizhe)
			zhongxiayeshizhe = 0
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102,300104)      
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end
	---------------------------------2012七夕活动---------------------------------------

	if (curyear == 2012 and curmon == 8 and curday >= 21 and curday <= 28 and curhour == 14 and  curmin <= 30) or (curyear == 2012 and curmon == 8 and curday >= 21 and curday <= 28 and curhour == 21 and curmin <= 30)then
		--七曜坐标
		if curmin == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102,300116)      
			msg.DispatchWorldMsgEvent(MsgID)
		end
		local QY_zuobiao = {}
		QY_zuobiao[1] = {258,3008,207}
		QY_zuobiao[2] = {258,3018,197}
		QY_zuobiao[3] = {245,3038,219}
		QY_zuobiao[4] = {263,3258,293}
		QY_zuobiao[5] = {280,3174,306}
		QY_zuobiao[6] = {303,3168,349}
		QY_zuobiao[7] = {263,3081,372}
		QY_zuobiao[8] = {238,3089,388}
		QY_zuobiao[9] = {225,3149,361}
		QY_zuobiao[10] = {227,3166,335}
		if curmin == 0 or (curmin%3) == 0 then
			for i = 1,10 do
				map.MapCreateColCreature(3373741864, -1, 6040200, QY_zuobiao[i][1],QY_zuobiao[i][2],QY_zuobiao[i][3], 1, "")
			end
		end
	end
end

aux.RegisterActEvent(29, 4, "SystemActivity_OnTimerMin")

function SystemActivity_OnClick(MapID, InstanceID, RoleID, TOMsgID, ToMapID, x, y, z)
    if JBZDY_Open == 1 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local money = math.floor(level/2)
		if role.GetRoleSilver(MapID, InstanceID, RoleID) < money*10000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 206752)		--你需要XX金才能参与金币总动员活动。凑足金币后你可以在<color=0xffff0000>金币总动员特使【斗战天城371 732】<color=0xfffff7e0>处报名参与。
			msg.AddMsgEvent(MsgID, 9, money)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    role.DecRoleSilver(MapID, InstanceID, RoleID, money*10000, 452)
			local benrijiangli_CurTime = act.GetActScriptData(29, 1, benrijiangli_Index)
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
			msg.AddMsgEvent(MsgID, 71, 206753)		--大奖奖池已有XXX金，小奖奖池已有XXX金。如果晚上9：30开奖时获奖玩家不在线，则该奖金将会自动累计进大奖奖池。
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(29, 1, jiangchi_JBZDY)+act.GetActScriptData(29, 1, benrijiangli_JBZDY))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(29, 1, benrijiangli_JBZDY))
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end

aux.RegisterActEvent(29, 8, "SystemActivity_OnClick")
