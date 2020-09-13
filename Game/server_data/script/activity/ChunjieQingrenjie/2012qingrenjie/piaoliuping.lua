--漂流瓶调查地物
function piaoliuping_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local num = role.GetRoleScriptData(RoleID, 1, RoleDataType["piaoliuping"]) % 100
	local lastDate = (role.GetRoleScriptData(RoleID, 1, RoleDataType["piaoliuping"]) - num)/100
	local curDate = tonumber(os.date("%j"))
	if(curDate ~= lastDate) then
		lastDate = curDate
		num = 1
		unit.ChangeHP(MapID, InstanceID, TargetID, -2)
		role.AddRoleItem(MapID, InstanceID, RoleID, 4800268, 1, -1, 8, 420)
	else
		if role.IsRoleHaveBuff(MapID, InstanceID,RoleID,90034) then
			if num < 20 then
				num = num + 1
				unit.ChangeHP(MapID, InstanceID, TargetID, -2)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800268, 1, -1, 8, 420)
			else
				unit.ChangeHP(MapID, InstanceID, TargetID, 1)
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 210006)--你今天的调查次数已经达到上限
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			end
		else
			if num < 10 then
				num = num + 1
				unit.ChangeHP(MapID, InstanceID, TargetID, -2)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800268, 1, -1, 8, 420)
			else
				unit.ChangeHP(MapID, InstanceID, TargetID, 1)
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 210001)--你今天的调查次数已经达到上限
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			end
		end
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["piaoliuping"],lastDate*100+num)


end
aux.RegisterCreatureEvent(6011033, 6, "piaoliuping_OnInvest")
aux.RegisterCreatureEvent(6011034, 6, "piaoliuping_OnInvest")

--漂流瓶
function I4800268_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	local r = math.random(100)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {4800258,1,-1,18} --情人节卡片
	tbl[2] = {4800259,1,-1,15} --怪异药水
	tbl[3] = {4800267,1,-1,1} --爱神之羽
	tbl[4] = {4820043,1,-1,15} --经验宝瓶
	tbl[5] = {4800269,1,-1,10} --情人节藏宝图
	tbl[6] = {4090020,1,-1,1} --妖精不老泉
	tbl[7] = {4090019,1,-1,2} --大地之灵
	tbl[8] = {4090021,1,-1,2} --生命之光
	tbl[9] = {4800053,1,-1,4} --妖精巧克力
	tbl[10] = {4400031,1,-1,1}--精炼妖精秘药
	tbl[11] = {4820109,1,-1,3}--五等神秘水晶
	tbl[12] = {4820108,5,-1,5}--四等神秘水晶
	tbl[13] = {4820107,10,-1,6}--三等神秘水晶
	tbl[14] = {4820229,1,-1,2} --三倍悬赏符（80级）
	tbl[15] = {8170110,1,5,1}  --情人节物理紫妖精
	tbl[16] = {8170111,1,5,1}  --情人节法系紫妖精
	tbl[17] = {8170112,1,4,2}  --情人节物理橙妖精
	tbl[18] = {8170113,1,4,2}  --情人节法系橙妖精
	tbl[19] = {4800280,1,-1,5}  --黄金贝*1
	tbl[20] = {4800280,2,-1,2}  --黄金贝*3
	tbl[21] = {4800280,3,-1,2}  --黄金贝*5
	local record = role.GetRoleScriptData(TargetID, 1, RoleDataType["kaiqipiaoliuping"])
	local num = record % 1000
	local lastDate = (record - num)/1000
	local curDate = tonumber(os.date("%j"))
	local mark = false
	if curyear == 2012  and curmon == 2 and curday >= 9 and curday <= 27 then
		if(curDate ~= lastDate) then
			lastDate = curDate
			num = 1
			mark =  false
		elseif num % 5 == 4 then
			num =num +1
			mark = true
		else
			num =num +1
			mark = false
		end
		if mark == false then
			for i = 1,#tbl do
				temp = temp + tbl[i][4]
				if temp >= r then
					index = i
					break
				end
			end
		else
			if math.random(100) % 2 == 0 then
				index = 1
			else
				index = 5
			end
		end
		if curday == 22 and index == 1 then
			role.AddRoleSilver(MapID, InstanceID, TargetID, 50000)
		elseif index == 5 then
			role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1]+math.random(10)-1, tbl[index][2], tbl[index][3], 8, 35)
		else
			role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 35)
		end
		role.SetRoleScriptData(TargetID, 1, RoleDataType["kaiqipiaoliuping"],lastDate*1000+num)
	else
		role.AddRoleSilver(MapID, InstanceID, TargetID, 10000)
	end

end

function I4800268_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800268, 1, "I4800268_GiftBag")
aux.RegisterItemEvent(4800268, 0, "I4800268_CanUseGiftBag")


function piaoliuping_appear()
	local tbl = {}
	tbl[1] = {620,2362,247}
	tbl[2] = {623,2356,255}
	tbl[3] = {621,2367,261}
	tbl[4] = {614,2391,258}
	tbl[5] = {610,2400,250}
	tbl[6] = {630,2379,293}
	tbl[7] = {635,2464,308}
	tbl[8] = {621,2393,307}
	tbl[9] = {642,2380,327}
	tbl[10] = {641,2380,350}
	tbl[11] = {629,2387,346}
	tbl[12] = {630,2387,364}
	tbl[13] = {646,2380,377}
	tbl[14] = {638,2380,395}
	tbl[15] = {619,2412,411}
	tbl[16] = {625,2389,431}
	tbl[17] = {615,2809,453}
	tbl[18] = {604,2469,445}
	tbl[19] = {619,2382,466}
	tbl[20] = {603,2437,480}
	tbl[21] = {612,2393,500}
	tbl[22] = {598,2475,526}
	tbl[23] = {606,2436,548}
	tbl[24] = {635,2380,576}
	tbl[25] = {634,2570,598}
	tbl[26] = {648,2383,620}
	tbl[27] = {637,2438,635}
	tbl[28] = {659,2391,668}
	tbl[29] = {650,2465,688}
	tbl[30] = {672,2382,701}
	tbl[31] = {655,2459,721}
	tbl[32] = {665,2421,740}
	tbl[33] = {684,2380,773}
	tbl[34] = {680,2380,799}
	tbl[35] = {692,2380,817}
	tbl[36] = {708,2367,842}
	tbl[37] = {729,2360,869}
	tbl[38] = {711,2360,891}
	tbl[39] = {677,2380,868}
	tbl[40] = {651,2380,865}
	tbl[41] = {648,2380,884}
	tbl[42] = {639,2380,903}
	tbl[43] = {632,2380,930}
	tbl[44] = {629,2380,944}
	tbl[45] = {617,2380,968}
	tbl[46] = {631,2380,994}
	tbl[47] = {608,2471,884}
	tbl[48] = {617,2395,843}
	tbl[49] = {597,2542,837}
	tbl[50] = {642,2438,801}
	tbl[51] = {654,2452,764}
	tbl[52] = {642,2496,736}
	tbl[53] = {624,2540,712}
	tbl[54] = {622,2511,638}
	tbl[55] = {606,2464,588}
	tbl[56] = {583,2528,554}
	tbl[57] = {596,2483,509}
	tbl[58] = {601,2449,479}
	tbl[59] = {617,2418,433}
	tbl[60] = {620,2408,409}
	tbl[61] = {614,2444,375}
	tbl[62] = {618,2409,333}
	tbl[63] = {607,2451,304}
	tbl[64] = {623,2360,259}
	tbl[65] = {651,2350,236}
	tbl[66] = {648,2348,219}
	tbl[67] = {666,2294,201}
	tbl[68] = {652,2341,207}
	tbl[69] = {602,2757,197}
	tbl[70] = {605,2459,302}
	tbl[71] = {599,2503,340}
	tbl[72] = {615,2439,373}
	tbl[73] = {611,2397,497}
	tbl[74] = {619,2385,549}
	tbl[75] = {624,2402,593}
	tbl[76] = {655,2384,647}
	tbl[77] = {644,2501,710}
	tbl[78] = {641,2491,753}
	tbl[79] = {638,2476,793}
	tbl[80] = {677,2380,833}
	tbl[81] = {600,2442,952}
	tbl[82] = {610,2387,978}
	tbl[83] = {615,2426,1020}
	tbl[84] = {623,2387,1047}
	tbl[85] = {586,2372,1034}
	tbl[86] = {611,2388,1007}
	tbl[87] = {595,2388,977}
	tbl[88] = {607,2434,939}
	tbl[89] = {606,2403,915}
	tbl[90] = {614,2434,887}
	tbl[91] = {648,2410,834}
	tbl[92] = {650,2348,786}
	tbl[93] = {616,2405,540}
	tbl[94] = {596,2457,1066}
	tbl[95] = {618,2407,1039}
	tbl[96] = {621,2385,1061}
	tbl[97] = {592,2467,1068}
	tbl[98] = {577,2482,1080}
	tbl[99] = {623,2398,1030}
	tbl[100] = {593,2492,547}
	for i=1,#tbl do
		map.MapCreateColCreature(3390518312, 1, 6011032+math.random(2), tbl[i][1], tbl[i][2], tbl[i][3], 1, "")
	end
end
