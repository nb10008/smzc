--全局表
Shadui_set = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51}
xinxingshadiao = 1
curMSIns = 0
--Qingrenjie_canRef = 1
--坐标
local shadui_normal = {}
--[[
shadui_normal[1] = {x = 395,y = 2818,z = 951}
shadui_normal[2] = {x = 422,y = 2832,z = 925}
shadui_normal[3] = {x = 375,y = 2849,z = 907}
shadui_normal[4] = {x = 419,y = 2846,z = 883}
shadui_normal[5] = {x = 467,y = 2752,z = 873}
shadui_normal[6] = {x = 504,y = 2702,z = 873}
shadui_normal[7] = {x = 383,y = 2855,z = 857}
shadui_normal[8] = {x = 342,y = 2850,z = 885}
shadui_normal[9] = {x = 343,y = 2850,z = 846}
shadui_normal[10] = {x = 382,y = 2850,z = 806}
shadui_normal[11] = {x = 405,y = 2850,z = 762}
shadui_normal[12] = {x = 466,y = 2741,z = 743}
shadui_normal[13] = {x = 512,y = 2700,z = 751}
shadui_normal[14] = {x = 546,y = 2698,z = 733}
shadui_normal[15] = {x = 423,y = 2850,z = 725}
shadui_normal[16] = {x = 330,y = 2844,z = 795}
shadui_normal[17] = {x = 288,y = 2822,z = 811}
shadui_normal[18] = {x = 262,y = 2799,z = 785}
shadui_normal[19] = {x = 255,y = 2782,z = 749}
shadui_normal[20] = {x = 291,y = 2812,z = 738}
shadui_normal[21] = {x = 343,y = 2850,z = 755}
shadui_normal[22] = {x = 377,y = 2850,z = 708}
shadui_normal[23] = {x = 332,y = 2850,z = 689}
shadui_normal[24] = {x = 365,y = 2850,z = 670}
shadui_normal[25] = {x = 407,y = 2850,z = 656}
shadui_normal[26] = {x = 446,y = 2830,z = 681}
shadui_normal[27] = {x = 296,y = 2804,z = 688}
shadui_normal[28] = {x = 254,y = 2754,z = 704}
shadui_normal[29] = {x = 256,y = 2750,z = 649}
shadui_normal[30] = {x = 233,y = 2765,z = 596}
shadui_normal[31] = {x = 296,y = 2787,z = 646}
shadui_normal[32] = {x = 310,y = 2819,z = 597}
shadui_normal[33] = {x = 368,y = 2850,z = 622}
shadui_normal[34] = {x = 439,y = 2765,z = 607}
shadui_normal[35] = {x = 556,y = 2583,z = 564}
shadui_normal[36] = {x = 516,y = 2676,z = 550}
shadui_normal[37] = {x = 470,y = 2714,z = 560}
shadui_normal[38] = {x = 419,y = 2804,z = 550}
shadui_normal[39] = {x = 398,y = 2850,z = 585}
shadui_normal[40] = {x = 375,y = 2848,z = 493}
shadui_normal[41] = {x = 359,y = 2850,z = 557}
--]]
shadui_normal[1] = {x = 460,y = 2728,z = 987}
shadui_normal[2] = {x = 413,y = 2815,z = 947}
shadui_normal[3] = {x = 382,y = 2852,z = 842}
shadui_normal[4] = {x = 329,y = 2847,z = 818}
shadui_normal[5] = {x = 287,y = 2814,z = 771}
shadui_normal[6] = {x = 250,y = 2775,z = 740}
shadui_normal[7] = {x = 245,y = 2713,z = 699}
shadui_normal[8] = {x = 261,y = 2769,z = 678}
shadui_normal[9] = {x = 306,y = 2825,z = 674}
shadui_normal[10] = {x = 276,y = 2755,z = 647}
shadui_normal[11] = {x = 300,y = 2812,z = 608}
shadui_normal[12] = {x = 332,y = 2844,z = 606}
shadui_normal[13] = {x = 345,y = 2850,z = 575}
shadui_normal[14] = {x = 354,y = 2850,z = 545}
shadui_normal[15] = {x = 378,y = 2848,z = 490}
shadui_normal[16] = {x = 354,y = 2848,z = 435}
shadui_normal[17] = {x = 338,y = 2850,z = 380}
shadui_normal[18] = {x = 328,y = 2850,z = 346}
shadui_normal[19] = {x = 361,y = 2851,z = 332}
shadui_normal[20] = {x = 409,y = 2799,z = 311}
shadui_normal[21] = {x = 453,y = 2730,z = 299}
shadui_normal[22] = {x = 503,y = 2569,z = 307}
shadui_normal[23] = {x = 521,y = 2567,z = 358}
shadui_normal[24] = {x = 545,y = 2551,z = 412}
shadui_normal[25] = {x = 541,y = 2556,z = 465}
shadui_normal[26] = {x = 532,y = 2572,z = 496}
shadui_normal[27] = {x = 499,y = 2712,z = 551}
shadui_normal[28] = {x = 540,y = 2599,z = 569}
shadui_normal[29] = {x = 421,y = 2803,z = 544}
shadui_normal[30] = {x = 379,y = 2850,z = 575}
shadui_normal[31] = {x = 419,y = 2849,z = 645}
shadui_normal[32] = {x = 395,y = 2850,z = 717}
shadui_normal[33] = {x = 445,y = 2833,z = 741}
shadui_normal[34] = {x = 567,y = 2676,z = 706}
shadui_normal[35] = {x = 568,y = 2646,z = 746}
shadui_normal[36] = {x = 539,y = 2698,z = 808}
shadui_normal[37] = {x = 489,y = 2723,z = 870}
shadui_normal[38] = {x = 431,y = 2819,z = 879}
shadui_normal[39] = {x = 335,y = 2850,z = 763}
shadui_normal[40] = {x = 359,y = 2850,z = 703}
shadui_normal[41] = {x = 348,y = 2850,z = 862}
shadui_normal[42] = {x = 501,y = 2722,z = 956}
shadui_normal[43] = {x = 544,y = 2700,z = 839}
shadui_normal[44] = {x = 497,y = 2700,z = 738}
shadui_normal[45] = {x = 577,y = 2623,z = 666}
shadui_normal[46] = {x = 574,y = 2577,z = 600}
shadui_normal[47] = {x = 465,y = 2720,z = 567}
shadui_normal[48] = {x = 394,y = 2843,z = 416}
shadui_normal[49] = {x = 400,y = 2827,z = 379}
shadui_normal[50] = {x = 553,y = 2553,z = 315}
shadui_normal[51] = {x = 357,y = 2850,z = 641}
--活动每分钟调用
function act53_OnTimerMin(ActID)
	local curhour = tonumber(os.date("%H"))--小时
	local curmin = tonumber(os.date("%M"))--分钟
	--刷漂流瓶
	if curmin == 0 or curmin == 30 then
		piaoliuping_appear()
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300093)--博博罗曼岛的海滩出现了许多漂流瓶……
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
	if curhour == 19 or curhour == 20 then
		--[[此处重写
		if Qingrenjie_canRef == 1 then
			for k in pairs(shadui_normal) do
				local cid = map.MapCreateColCreature(3390518312, 1, 6011039+math.random(3), shadui_normal[k].x, shadui_normal[k].y, shadui_normal[k].z, 1)
				cre.SetCreatureScriptData(3390518312, 1, cid, 1, 0, k)
				table.remove(Shadui_set, k)
			end
			--根据场景玩家数量刷新计时器
			--删除计时器刷怪
			local temptable = {}
			local total = 0
			temptable = map.GetMapAllRoleID(3390518312, 1)
			if temptable ~= nil then
				total = table.maxn(temptable)
			end
			if total >= 200 then
				map.MapCreateCreature(3390518312, 1, 6011049, 1, 1 , 1)
			elseif total >= 100 then
				map.MapCreateCreature(3390518312, 1, 6011050, 1, 1 , 1)
			else
				map.MapCreateCreature(3390518312, 1, 6011051, 1, 1 , 1)
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300094)--博博罗曼岛的沙雕活动现在开始，让传送使者[x,z]带您体验热带沙滩之趣。
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
			Qingrenjie_canRef = 0
		end
		--]]

		--每分钟刷新
		if Shadui_set ~= nil then
			for k in pairs(Shadui_set) do
				local cid = map.MapCreateColCreature(3390518312, 1, 6011039+math.random(3), shadui_normal[Shadui_set[k]].x, shadui_normal[Shadui_set[k]].y, shadui_normal[Shadui_set[k]].z, 1)
				cre.SetCreatureScriptData(3390518312, 1, cid, 1, 0, Shadui_set[k])
				table.remove(Shadui_set, k)
			end
		end
		if curmin == 0 or curmin == 30 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300094)--博博罗曼岛的沙雕活动现在开始，让传送使者[x,z]带您体验热带沙滩之趣。
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
		end
		if curmin%10 == 0 then
			local shabao_Pos = {}
			shabao_Pos[1] = {x = 404,y = 2794,z = 976}
			shabao_Pos[2] = {x = 381,y = 2849,z = 887}
			shabao_Pos[3] = {x = 530,y = 2700,z = 872}
			shabao_Pos[4] = {x = 266,y = 2819,z = 807}
			shabao_Pos[5] = {x = 376,y = 2850,z = 788}
			shabao_Pos[6] = {x = 530,y = 2700,z = 761}
			shabao_Pos[7] = {x = 282,y = 2791,z = 703}
			shabao_Pos[8] = {x = 431,y = 2850,z = 691}
			shabao_Pos[9] = {x = 348,y = 2850,z = 637}
			shabao_Pos[10] = {x = 261,y = 2761,z = 595}
			shabao_Pos[11] = {x = 427,y = 2814,z = 590}
			shabao_Pos[12] = {x = 376,y = 2849,z = 516}
			shabao_Pos[13] = {x = 542,y = 2566,z = 537}
			local rNum = math.random(13)
			local cid = map.MapCreateColCreature(3390518312, 1, 6011035, shabao_Pos[rNum].x, shabao_Pos[rNum].y, shabao_Pos[rNum].z, 1)
			local cid2 = map.MapCreateColCreature(3390518312, 1, 6011053, 1, 1, 1, 1)--10分钟计数
			cre.SetCreatureScriptData(3390518312, 1, cid2, 1, 0, cid)
			--创建boss场景
			--boss_pos 120,500,120
			map.CreateInstance(3390517544, cid)
			curMSIns = cid
			map.MapCreateColCreature(3390517544, cid, 1095130, 120, 500, 120, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 210023)	--沙丘魔堡的入口突然出现在[,]……
			msg.AddMsgEvent(MsgID, 9, shabao_Pos[rNum].x)
			msg.AddMsgEvent(MsgID, 9, shabao_Pos[rNum].z)
			msg.DispatchMapMsgEvent(MsgID, 3390518312, 1)
		elseif curmin%5 == 0 then
			local shabao_Pos = {}
			shabao_Pos[1] = {x = 404,y = 2794,z = 976}
			shabao_Pos[2] = {x = 381,y = 2849,z = 887}
			shabao_Pos[3] = {x = 530,y = 2700,z = 872}
			shabao_Pos[4] = {x = 266,y = 2819,z = 807}
			shabao_Pos[5] = {x = 376,y = 2850,z = 788}
			shabao_Pos[6] = {x = 530,y = 2700,z = 761}
			shabao_Pos[7] = {x = 282,y = 2791,z = 703}
			shabao_Pos[8] = {x = 431,y = 2850,z = 691}
			shabao_Pos[9] = {x = 348,y = 2850,z = 637}
			shabao_Pos[10] = {x = 261,y = 2761,z = 595}
			shabao_Pos[11] = {x = 427,y = 2814,z = 590}
			shabao_Pos[12] = {x = 376,y = 2849,z = 516}
			shabao_Pos[13] = {x = 542,y = 2566,z = 537}
			local rNum = math.random(13)
			local cid = map.MapCreateColCreature(3390518312, 1, 6011035, shabao_Pos[rNum].x, shabao_Pos[rNum].y, shabao_Pos[rNum].z, 1)
			local cid2 = map.MapCreateColCreature(3390518312, 1, 6011053, 1, 1, 1, 1)--10分钟计数
			cre.SetCreatureScriptData(3390518312, 1, cid2, 1, 0, cid)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 210023)	--沙丘魔堡的入口突然出现在[,]……
			msg.AddMsgEvent(MsgID, 9, shabao_Pos[rNum].x)
			msg.AddMsgEvent(MsgID, 9, shabao_Pos[rNum].z)
			msg.DispatchMapMsgEvent(MsgID, 3390518312, 1)
			--创建宝箱场景
			map.CreateInstance(3390517544, cid)
			curMSIns = cid
			local baoxiang_Pos = {}
			baoxiang_Pos[1] ={cre = 6011045,x = 6000,y = 500,z = 6000}
			baoxiang_Pos[2] ={cre = 6011046,x = 6845,y = 500,z = 5345}
			baoxiang_Pos[3] ={cre = 6011046,x = 6840,y = 500,z = 6579}
			baoxiang_Pos[4] ={cre = 6011046,x = 5650,y = 500,z = 6949}
			baoxiang_Pos[5] ={cre = 6011046,x = 4936,y = 500,z = 5938}
			baoxiang_Pos[6] ={cre = 6011046,x = 5703,y = 500,z = 4969}
			baoxiang_Pos[7] ={cre = 6011047,x = 4130,y = 500,z = 8411}
			baoxiang_Pos[8] ={cre = 6011047,x = 3109,y = 500,z = 7026}
			baoxiang_Pos[9] ={cre = 6011047,x = 3137,y = 500,z = 4961}
			baoxiang_Pos[10] ={cre = 6011047,x = 4134,y = 500,z = 3632}
			baoxiang_Pos[11] ={cre = 6011047,x = 5952,y = 500,z = 3008}
			baoxiang_Pos[12] ={cre = 6011047,x = 7803,y = 500,z = 3626}
			baoxiang_Pos[13] ={cre = 6011047,x = 8819,y = 500,z = 4977}
			baoxiang_Pos[14] ={cre = 6011047,x = 8824,y = 500,z = 7038}
			baoxiang_Pos[15] ={cre = 6011047,x = 7804,y = 500,z = 8421}
			baoxiang_Pos[16] ={cre = 6011047,x = 5976,y = 500,z = 9017}
			for k in pairs(baoxiang_Pos) do
				map.MapCreateColCreature(3390517544, cid, baoxiang_Pos[k].cre, baoxiang_Pos[k].x/50, baoxiang_Pos[k].y, baoxiang_Pos[k].z/50, 1)
			end
		end
	end
end
aux.RegisterActEvent(53, 4, "act53_OnTimerMin")

--10分钟计时器
function c6011053_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	map.DeleteInstance(3390517544, cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0))
end
aux.RegisterCreatureEvent(6011053, 13, "c6011053_On_Disappear")

--沙堆调查刷新4种沙雕
function c6011040_On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	local x,y,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	local randomNum = math.random(100)
	if randomNum > 90 then--桃心
		if xinxingshadiao == 1 then
			local cid = map.MapCreateColCreature(MapID, InstanceID, 6011044, x, y, z, 1)
			cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0))
			map.MapCreateColCreature(MapID, InstanceID, 6011048, x, y+1000, z, 1)
			xinxingshadiao = 0
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 210032)	--心形
			msg.AddMsgEvent(MsgID, 9, x)
			msg.AddMsgEvent(MsgID, 9, z)
			msg.DispatchMapMsgEvent(MsgID, 3390518312, 1)
		else
			local cid = map.MapCreateColCreature(MapID, InstanceID, 6011037, x, y, z, 1)
			cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0))
		end
	elseif randomNum > 80 then--海龟
		local cid = map.MapCreateColCreature(MapID, InstanceID, 6011037, x, y, z, 1)
		cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0))
	elseif randomNum > 30 then--海星
		local cid = map.MapCreateColCreature(MapID, InstanceID, 6011038, x, y, z, 1)
		cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0))
	else--海豚
		local cid = map.MapCreateColCreature(MapID, InstanceID, 6011039, x, y, z, 1)
		cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0))
	end
end
aux.RegisterCreatureEvent(6011040, 4, "c6011040_On_Die")
aux.RegisterCreatureEvent(6011041, 4, "c6011040_On_Die")
aux.RegisterCreatureEvent(6011042, 4, "c6011040_On_Die")

--海豚
function c6011039_On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	local x,y,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	local randomNum = math.random(100)
	local theInd = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)
	table.insert(Shadui_set, theInd)
	local fuhao = {-1,1}
	if randomNum <= 90 then
		local level = role.GetRoleLevel(MapID, InstanceID, KillerID)
		if level > 85 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095128, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		elseif level > 75 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095126, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		elseif level > 65 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095124, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		elseif level > 55 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095122, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		else
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095120, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		end
	else
		for i=1,5 do
			map.MapCreateColCreature(MapID, InstanceID, 6011052, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
		end
	end
end
aux.RegisterCreatureEvent(6011039, 4, "c6011039_On_Die")
--海星
function c6011038_On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	local x,y,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	local randomNum = math.random(100)
	local theInd = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)
	table.insert(Shadui_set, theInd)
	local fuhao = {-1,1}
	if randomNum <= 80 then
		local level = role.GetRoleLevel(MapID, InstanceID, KillerID)
		if level > 85 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095129, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		elseif level > 75 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095127, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		elseif level > 65 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095125, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		elseif level > 55 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095123, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		else
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095121, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		end
	else
		for i=1,5 do
			map.MapCreateColCreature(MapID, InstanceID, 6011052, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
		end
	end
end
aux.RegisterCreatureEvent(6011038, 4, "c6011038_On_Die")
--海龟
function c6011037_On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	local x,y,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	local randomNum = math.random(100)
	local theInd = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)
	table.insert(Shadui_set, theInd)
	local fuhao = {-1,1}
	if randomNum <= 70 then
		local level = role.GetRoleLevel(MapID, InstanceID, KillerID)
		if level > 85 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095129, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095128, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		elseif level > 75 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095127, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095126, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		elseif level > 65 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095125, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095124, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		elseif level > 55 then
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095123, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095122, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		else
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095121, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
			for i=1,5 do
				map.MapCreateColCreature(MapID, InstanceID, 1095120, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
			end
		end
	else
		for i=1,5 do
			map.MapCreateColCreature(MapID, InstanceID, 6011052, x+math.random(20)*fuhao[math.random(2)], y, z+math.random(20)*fuhao[math.random(2)], 1)
		end
	end
end
aux.RegisterCreatureEvent(6011037, 4, "c6011037_On_Die")

--心形NPC
function c6011044_On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	local uid1 = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 1)
	local uid2 = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 2)
	local uid3 = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 3)
	local uid4 = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 4)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		if uid1 == 0 and uid2 == 0 then
			msg.AddMsgEvent(MsgID, 20, 210010)			--还未有玩家在上面署名，快抓紧时间呀！
			msg.AddMsgEvent(MsgID, 21, 4)				--签名
			msg.AddMsgEvent(MsgID, 1, 210011)
		elseif uid1 ~= 0 and uid2 == 0 then
			msg.AddMsgEvent(MsgID, 20, 210012)			--xxx 先生 已经在上面写下了自己的名字，正等候他的情侣签下名字
			msg.AddMsgEvent(MsgID, 2, uid1)				--玩家A
			if role.IsCouple(RoleID, uid1) == true then
				msg.AddMsgEvent(MsgID, 21, 4)			--签名
				msg.AddMsgEvent(MsgID, 1, 210011)
			elseif uid1 == RoleID then
				msg.AddMsgEvent(MsgID, 21, 5)
				msg.AddMsgEvent(MsgID, 1, 210013)			--领取奖励
			else
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 210026)			--规则介绍
			end
		elseif uid1 == 0 and uid2 ~= 0 then
			msg.AddMsgEvent(MsgID, 20, 210027)			--xxx 女士 已经在上面写下了自己的名字，正等候她的情侣签下名字
			msg.AddMsgEvent(MsgID, 2, uid2)				--玩家B
			if role.IsCouple(RoleID, uid2) == true then
				msg.AddMsgEvent(MsgID, 21, 4)		--签名
				msg.AddMsgEvent(MsgID, 1, 210011)
			elseif uid2 == RoleID then
				msg.AddMsgEvent(MsgID, 21, 5)
				msg.AddMsgEvent(MsgID, 1, 210013)			--领取奖励
			else
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 210026)			--规则介绍
			end
		else
			msg.AddMsgEvent(MsgID, 20, 210028)			--xxx 先生和 xxx 女士 在此留下爱的印记
			msg.AddMsgEvent(MsgID, 2, uid1)				--玩家A
			msg.AddMsgEvent(MsgID, 2, uid2)				--玩家B
			if uid1 == RoleID or uid2 == RoleID then
				msg.AddMsgEvent(MsgID, 21, 5)
				msg.AddMsgEvent(MsgID, 1, 210013)			--领取奖励
			else
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 210026)			--规则介绍
			end
		end
		msg.AddMsgEvent(MsgID, 24, CreatureID)
		msg.AddMsgEvent(MsgID, 21, 7)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--取消
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local usex = role.GetRoleSex(RoleID)
		local isMarry = role.IsRoleMarried(RoleID)
		local owned = role.GetRoleScriptData(RoleID, 1, RoleDataType["qingrenjiezhi"])
		if owned > 0 then
			--已经完成过爱心沙雕，把机会留给其他人吧
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 210015)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif isMarry == true then
			local loverid = role.GetRoleLoverID(MapID, InstanceID, RoleID)
			local curMapid,_ = role.GetRoleMapID(loverid)
			if curMapid == MapID then
				if usex == 1 then
					if uid1 == 0 then
						if uid2 == 0 then
							cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 1, RoleID)
							local MsgID = msg.BeginMsgEvent()		--签名成功
							msg.AddMsgEvent(MsgID, 26, 210017)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						else
							if role.IsCouple(RoleID, uid2) == true then
								cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 1, RoleID)
								local MsgID = msg.BeginMsgEvent()		--签名成功
								msg.AddMsgEvent(MsgID, 26, 210017)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								--已被其他情侣抢占先机
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 26, 210016)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							end
						end
					elseif uid1 == RoleID and uid2 == 0 then
						local MsgID = msg.BeginMsgEvent()		--等待情侣
						msg.AddMsgEvent(MsgID, 26, 210030)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						--已被其他情侣抢占先机
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 210016)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					if uid2 == 0 then
						if uid1 == 0 then
							cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 2, RoleID)
							local MsgID = msg.BeginMsgEvent()		--签名成功
							msg.AddMsgEvent(MsgID, 26, 210017)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						else
							if role.IsCouple(RoleID, uid1) == true then
								cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 2, RoleID)
								local MsgID = msg.BeginMsgEvent()		--签名成功
								msg.AddMsgEvent(MsgID, 26, 210017)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							else
								--已被其他情侣抢占先机
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 26, 210016)
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
							end
						end
					elseif uid2 == RoleID and uid1 == 0 then
						local MsgID = msg.BeginMsgEvent()		--等待情侣
						msg.AddMsgEvent(MsgID, 26, 210030)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						--已被其他情侣抢占先机
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 210016)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 210031)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			--只有结婚的玩家才可以
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 210018)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 5 then
		if uid1 == RoleID then
			if uid2 == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 210030)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif uid3 > 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 210025)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 3, 1)
				role.AddRoleItem(MapID, InstanceID, RoleID, 8110712, 1, 6, 8, 420)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["qingrenjiezhi"],1)
			end
		elseif uid2 == RoleID then
			if uid1 == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 210030)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif uid4 > 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 210025)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 4, 1)
				role.AddRoleItem(MapID, InstanceID, RoleID, 8110713, 1, 6, 8, 420)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["qingrenjiezhi"],1)
			end
		else
			--已被其他情侣抢占先机
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 210016)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 6 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 210014)			--可以获得情人节专属戒指，每人只可以拥有一个
		msg.AddMsgEvent(MsgID, 24, CreatureID)
		msg.AddMsgEvent(MsgID, 21, 7)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterCreatureEvent(6011044, 7, "c6011044_On_Talk")

--沙堡NPC
function c6011035_On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	--传送
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 210020)			--进入密室
		msg.AddMsgEvent(MsgID, 24, CreatureID)
		msg.AddMsgEvent(MsgID, 21, 4)				--
		msg.AddMsgEvent(MsgID, 1, 100001)			--是
		msg.AddMsgEvent(MsgID, 21, 5)				--
		msg.AddMsgEvent(MsgID, 1, 100002)			--否
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local curInd = map.GetMapScriptData(3390517544, CreatureID, 1, 0)
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 60 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 210024)		--60级以上可以进入
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif curInd < 10 then
			local player_Pos = {}
			player_Pos[1] = {x = 2975,y = 500,z = 9971}
			player_Pos[2] = {x = 1314,y = 500,z = 7663}
			player_Pos[3] = {x = 1332,y = 500,z = 4295}
			player_Pos[4] = {x = 2951,y = 500,z = 2088}
			player_Pos[5] = {x = 5962,y = 500,z = 1134}
			player_Pos[6] = {x = 9024,y = 500,z = 2109}
			player_Pos[7] = {x = 10661,y = 503,z = 4263}
			player_Pos[8] = {x = 10690,y = 500,z = 7706}
			player_Pos[9] = {x = 9022,y = 500,z = 9990}
			player_Pos[10] = {x = 5963,y = 500,z = 10991}
			map.SetMapScriptData(3390517544, CreatureID, 1, 0, curInd+1)
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3390517544, player_Pos[curInd+1].x/50, player_Pos[curInd+1].y/50, player_Pos[curInd+1].z/50)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 210021)		--只有前10名玩家可以进入
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(6011035, 7, "c6011035_On_Talk")

--心形消失
function c6011044_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local theInd = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
	table.insert(Shadui_set, theInd)
	xinxingshadiao = 1
end
aux.RegisterCreatureEvent(6011044, 13, "c6011044_On_Disappear")

--调查
function c6011040_On_Invest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	if role.GetBagFreeSize(RoleID) < 1 then
		--背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 210019)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif role.GetRoleItemNum(RoleID, 4800280) < 1 then
		--缺少魔法水滴
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 210029)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
	else
		local curNum = math.random(100)
		if curNum > 95 then
			local temp = {3070009,3070040,3070034,3070028}
			local suiji = math.random(1,#temp)
			role.AddRoleItem(MapID, InstanceID, RoleID, temp[suiji], 1, -1, 8, 420)
		else
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if level > 90 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 200000)
			elseif level > 85 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 160000)
			elseif level > 80 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 100000)
			elseif level > 75 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 85000)
			elseif level > 70 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 60000)
			elseif level > 65 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 54000)
			elseif level > 60 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 43000)
			elseif level > 55 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 32000)
			elseif level > 50 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 17000)
			elseif level > 45 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 8000)
			elseif level >= 40 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 4000)
			end
		end
		role.AddRoleItem(MapID, InstanceID, RoleID, 4800266, 1, -1, 8, 420)
		role.RemoveFromRole(MapID, InstanceID, RoleID, 4800280, 1, 102)
	end
end
aux.RegisterCreatureEvent(6011040, 6, "c6011040_On_Invest")--沙堆
aux.RegisterCreatureEvent(6011041, 6, "c6011040_On_Invest")--沙堆
aux.RegisterCreatureEvent(6011042, 6, "c6011040_On_Invest")--沙堆

--海豚
function c6011039_On_Invest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)--6
	local maxhp = unit.GetUnitAttValue(MapID, InstanceID, CreatureID, 9)
	if maxhp == 4 then
		unit.AddBuff(MapID, InstanceID, CreatureID, 6940301, CreatureID)
	elseif maxhp == 2 then
		unit.AddBuff(MapID, InstanceID, CreatureID, 6940302, CreatureID)
	end
	if role.GetBagFreeSize(RoleID) < 1 then
		--背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 210019)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif role.GetRoleItemNum(RoleID, 4800280) < 1 then
		--缺少魔法水滴
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 210029)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
	else
		local curNum = math.random(100)
		if curNum > 95 then
			local temp = {3070009,3070040,3070034,3070028}
			local suiji = math.random(1,#temp)
			role.AddRoleItem(MapID, InstanceID, RoleID, temp[suiji], 1, -1, 8, 420)
		else
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if level > 90 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 200000)
			elseif level > 85 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 160000)
			elseif level > 80 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 100000)
			elseif level > 75 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 85000)
			elseif level > 70 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 60000)
			elseif level > 65 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 54000)
			elseif level > 60 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 43000)
			elseif level > 55 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 32000)
			elseif level > 50 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 17000)
			elseif level > 45 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 8000)
			elseif level >= 40 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 4000)
			end
		end
		role.AddRoleItem(MapID, InstanceID, RoleID, 4800266, 1, -1, 8, 420)
		role.RemoveFromRole(MapID, InstanceID, RoleID, 4800280, 1, 102)
	end
end
aux.RegisterCreatureEvent(6011039, 6, "c6011039_On_Invest")

--海星
function c6011038_On_Invest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)--54
	local maxhp = unit.GetUnitAttValue(MapID, InstanceID, CreatureID, 9)
	if maxhp == 6 then
		unit.AddBuff(MapID, InstanceID, CreatureID, 6940301, CreatureID)
	elseif maxhp == 3 then
		unit.AddBuff(MapID, InstanceID, CreatureID, 6940302, CreatureID)
	end
	if role.GetBagFreeSize(RoleID) < 1 then
		--背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 210019)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif role.GetRoleItemNum(RoleID, 4800280) < 1 then
		--缺少魔法水滴
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 210029)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
	else
		local curNum = math.random(100)
		if curNum > 95 then
			local temp = {3070009,3070040,3070034,3070028}
			local suiji = math.random(1,#temp)
			role.AddRoleItem(MapID, InstanceID, RoleID, temp[suiji], 1, -1, 8, 420)
		else
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if level > 90 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 200000)
			elseif level > 85 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 160000)
			elseif level > 80 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 100000)
			elseif level > 75 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 85000)
			elseif level > 70 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 60000)
			elseif level > 65 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 54000)
			elseif level > 60 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 43000)
			elseif level > 55 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 32000)
			elseif level > 50 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 17000)
			elseif level > 45 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 8000)
			elseif level >= 40 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 4000)
			end
		end
		role.AddRoleItem(MapID, InstanceID, RoleID, 4800266, 1, -1, 8, 420)
		role.RemoveFromRole(MapID, InstanceID, RoleID, 4800280, 1, 102)
	end
end
aux.RegisterCreatureEvent(6011038, 6, "c6011038_On_Invest")

--海龟
function c6011037_On_Invest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)--72
	local maxhp = unit.GetUnitAttValue(MapID, InstanceID, CreatureID, 9)
	if maxhp == 8 then
		unit.AddBuff(MapID, InstanceID, CreatureID, 6940301, CreatureID)
	elseif maxhp == 4 then
		unit.AddBuff(MapID, InstanceID, CreatureID, 6940302, CreatureID)
	end
	if role.GetBagFreeSize(RoleID) < 1 then
		--背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 210019)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif role.GetRoleItemNum(RoleID, 4800280) < 1 then
		--缺少魔法水滴
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 210029)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
	else
		local curNum = math.random(100)
		if curNum > 95 then
			local temp = {3070009,3070040,3070034,3070028}
			local suiji = math.random(1,#temp)
			role.AddRoleItem(MapID, InstanceID, RoleID, temp[suiji], 1, -1, 8, 420)
		else
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if level > 90 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 200000)
			elseif level > 85 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 160000)
			elseif level > 80 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 100000)
			elseif level > 75 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 85000)
			elseif level > 70 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 60000)
			elseif level > 65 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 54000)
			elseif level > 60 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 43000)
			elseif level > 55 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 32000)
			elseif level > 50 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 17000)
			elseif level > 45 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 8000)
			elseif level >= 40 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 4000)
			end
		end
		role.AddRoleItem(MapID, InstanceID, RoleID, 4800266, 1, -1, 8, 420)
		role.RemoveFromRole(MapID, InstanceID, RoleID, 4800280, 1, 102)
	end
end
aux.RegisterCreatureEvent(6011037, 6, "c6011037_On_Invest")

--王者宝箱
function c6011045_On_Invest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	if role.GetRoleItemNum(RoleID, 4800267) > 0 then
		if role.GetBagFreeSize(RoleID) < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 210019)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			unit.ChangeHP(MapID, InstanceID, CreatureID, -50)
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4800267, 1, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4800279, 1, -1, 8, 420)
			unit.AddBuff(MapID, InstanceID, RoleID, 6940401, RoleID)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 210022)		--缺少爱神之羽
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
	end
end
aux.RegisterCreatureEvent(6011045, 6, "c6011045_On_Invest")

function c6011046_On_Invest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 6940401, RoleID)
end
aux.RegisterCreatureEvent(6011046, 6, "c6011046_On_Invest")

function c6011047_On_Invest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 6940401, RoleID)
end
aux.RegisterCreatureEvent(6011047, 6, "c6011047_On_Invest")

--s64传出对话
function c6011054_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,210033) --是否要传送到博博罗曼岛
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21,4)			--
		msg.AddMsgEvent(MsgID, 1, 100001)			--“确定”
		msg.AddMsgEvent(MsgID, 21,5)			--
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3390518312,213,2543/50,726)
	end
end
aux.RegisterCreatureEvent(6011054, 7, "c6011054_On_Talk")


















