--[[

RoleDataType["canAddFaith"]

mapScript
1 计时器
2 记怪物1+2的数量
3 记录是否有玩家进来过

creScript
1 index
--]]

--1=boss,2-26=小动物，27-56=资源点，57-66=妖精，67-126=怪物1，127-161=怪物2
SGYJBHS_From = {}

P_table = {}
P_table[1] = {creType = 6040004,x = 190, y = 5688,z = 643}
P_table[2] = {creType = 5610004,x = 159, y = 3333,z = 533}
P_table[3] = {creType = 5610004,x = 277, y = 3175,z = 484}
P_table[4] = {creType = 5610004,x = 242, y = 3136,z = 420}
P_table[5] = {creType = 5610004,x = 153, y = 2719,z = 371}
P_table[6] = {creType = 5610004,x = 176, y = 2233,z = 258}
P_table[7] = {creType = 5610004,x = 222, y = 2374,z = 98}
P_table[8] = {creType = 5610004,x = 326, y = 2305,z = 164}
P_table[9] = {creType = 5610004,x = 168, y = 2352,z = 161}
P_table[10] = {creType = 5610004,x = 314, y = 2257,z = 203}
P_table[11] = {creType = 5610004,x = 338, y = 2668,z = 261}
P_table[12] = {creType = 5610004,x = 461, y = 3204,z = 260}
P_table[13] = {creType = 5610004,x = 535, y = 3220,z = 263}
P_table[14] = {creType = 5610004,x = 377, y = 4251,z = 354}
P_table[15] = {creType = 5610004,x = 504, y = 4023,z = 404}
P_table[16] = {creType = 5610004,x = 626, y = 4878,z = 365}
P_table[17] = {creType = 5610005,x = 569, y = 5451,z = 458}
P_table[18] = {creType = 5610005,x = 627, y = 5655,z = 515}
P_table[19] = {creType = 5610005,x = 728, y = 3316,z = 298}
P_table[20] = {creType = 5610005,x = 822, y = 3360,z = 317}
P_table[21] = {creType = 5610005,x = 756, y = 3208,z = 329}
P_table[22] = {creType = 5610005,x = 775, y = 3229,z = 391}
P_table[23] = {creType = 5610005,x = 828, y = 3040,z = 434}
P_table[24] = {creType = 5610005,x = 782, y = 3189,z = 643}
P_table[25] = {creType = 5610005,x = 681, y = 5098,z = 592}
P_table[26] = {creType = 5610005,x = 708, y = 5476,z = 501}
P_table[27] = {creType = 6040001,x = 419, y = 6346,z = 743}
P_table[28] = {creType = 6040001,x = 553, y = 5401,z = 709}
P_table[29] = {creType = 6040001,x = 377, y = 6380,z = 677}
P_table[30] = {creType = 6040001,x = 339, y = 6212,z = 642}
P_table[31] = {creType = 6040001,x = 341, y = 6106,z = 717}
P_table[32] = {creType = 6040001,x = 313, y = 5889,z = 635}
P_table[33] = {creType = 6040001,x = 312, y = 5860,z = 690}
P_table[34] = {creType = 6040001,x = 288, y = 5829,z = 650}
P_table[35] = {creType = 6040001,x = 253, y = 4835,z = 715}
P_table[36] = {creType = 6040001,x = 179, y = 3344,z = 488}
P_table[37] = {creType = 6040001,x = 250, y = 4738,z = 781}
P_table[38] = {creType = 6040001,x = 243, y = 4553,z = 814}
P_table[39] = {creType = 6040001,x = 184, y = 4268,z = 795}
P_table[40] = {creType = 6040001,x = 146, y = 4203,z = 820}
P_table[41] = {creType = 6040001,x = 160, y = 4645,z = 717}
P_table[42] = {creType = 6040001,x = 129, y = 4679,z = 704}
P_table[43] = {creType = 6040001,x = 140, y = 4265,z = 783}
P_table[44] = {creType = 6040001,x = 198, y = 4273,z = 763}
P_table[45] = {creType = 6040001,x = 220, y = 3279,z = 476}
P_table[46] = {creType = 6040001,x = 193, y = 3181,z = 443}
P_table[47] = {creType = 6040001,x = 193, y = 3357,z = 510}
P_table[48] = {creType = 6040001,x = 177, y = 2409,z = 315}
P_table[49] = {creType = 6040001,x = 284, y = 2497,z = 210}
P_table[50] = {creType = 6040001,x = 257, y = 2444,z = 178}
P_table[51] = {creType = 6040001,x = 202, y = 2277,z = 196}
P_table[52] = {creType = 6040001,x = 389, y = 4134,z = 330}
P_table[53] = {creType = 6040001,x = 351, y = 4596,z = 385}
P_table[54] = {creType = 6040001,x = 352, y = 5178,z = 459}
P_table[55] = {creType = 6040001,x = 510, y = 3607,z = 293}
P_table[56] = {creType = 6040001,x = 423, y = 4330,z = 405}
P_table[57] = {creType = 6040005,x = 593, y = 5827,z = 520}
P_table[58] = {creType = 6040005,x = 545, y = 5996,z = 510}
P_table[59] = {creType = 6040005,x = 562, y = 6257,z = 581}
P_table[60] = {creType = 6040005,x = 511, y = 6185,z = 541}
P_table[61] = {creType = 6040005,x = 835, y = 2933,z = 550}
P_table[62] = {creType = 6040005,x = 777, y = 2971,z = 600}
P_table[63] = {creType = 6040005,x = 788, y = 2881,z = 545}
P_table[64] = {creType = 6040005,x = 828, y = 2887,z = 584}
P_table[65] = {creType = 6040005,x = 272, y = 5492,z = 671}
P_table[66] = {creType = 6040005,x = 291, y = 5330,z = 767}
P_table[67] = {creType = 6040003,x = 208, y = 2886,z = 382}
P_table[68] = {creType = 6040003,x = 195, y = 2992,z = 417}
P_table[69] = {creType = 6040003,x = 221, y = 3120,z = 439}
P_table[70] = {creType = 6040003,x = 200, y = 3204,z = 460}
P_table[71] = {creType = 6040003,x = 266, y = 3185,z = 470}
P_table[72] = {creType = 6040003,x = 226, y = 3371,z = 491}
P_table[73] = {creType = 6040003,x = 162, y = 3390,z = 513}
P_table[74] = {creType = 6040003,x = 195, y = 3305,z = 486}
P_table[75] = {creType = 6040003,x = 244, y = 3248,z = 515}
P_table[76] = {creType = 6040003,x = 172, y = 3348,z = 454}
P_table[77] = {creType = 6040003,x = 189, y = 2349,z = 129}
P_table[78] = {creType = 6040003,x = 221, y = 2321,z = 144}
P_table[79] = {creType = 6040003,x = 181, y = 2317,z = 108}
P_table[80] = {creType = 6040003,x = 192, y = 2305,z = 158}
P_table[81] = {creType = 6040003,x = 171, y = 2343,z = 201}
P_table[82] = {creType = 6040003,x = 238, y = 2450,z = 169}
P_table[83] = {creType = 6040003,x = 260, y = 2519,z = 216}
P_table[84] = {creType = 6040003,x = 277, y = 2430,z = 193}
P_table[85] = {creType = 6040003,x = 263, y = 2453,z = 148}
P_table[86] = {creType = 6040003,x = 196, y = 2368,z = 233}
P_table[87] = {creType = 6040003,x = 518, y = 3811,z = 349}
P_table[88] = {creType = 6040003,x = 493, y = 4036,z = 377}
P_table[89] = {creType = 6040003,x = 464, y = 4078,z = 356}
P_table[90] = {creType = 6040003,x = 511, y = 3670,z = 311}
P_table[91] = {creType = 6040003,x = 431, y = 3838,z = 320}
P_table[92] = {creType = 6040003,x = 487, y = 3399,z = 277}
P_table[93] = {creType = 6040003,x = 437, y = 4270,z = 415}
P_table[94] = {creType = 6040003,x = 385, y = 4011,z = 317}
P_table[95] = {creType = 6040003,x = 355, y = 4471,z = 372}
P_table[96] = {creType = 6040003,x = 308, y = 4599,z = 373}
P_table[97] = {creType = 6040003,x = 747, y = 3560,z = 255}
P_table[98] = {creType = 6040003,x = 784, y = 3186,z = 335}
P_table[99] = {creType = 6040003,x = 804, y = 3610,z = 270}
P_table[100] = {creType = 6040003,x = 821, y = 3587,z = 264}
P_table[101] = {creType = 6040003,x = 795, y = 3476,z = 296}
P_table[102] = {creType = 6040003,x = 807, y = 3198,z = 367}
P_table[103] = {creType = 6040003,x = 806, y = 3057,z = 449}
P_table[104] = {creType = 6040003,x = 552, y = 6160,z = 570}
P_table[105] = {creType = 6040003,x = 522, y = 5937,z = 516}
P_table[106] = {creType = 6040003,x = 471, y = 6506,z = 547}
--boss旁边1
P_table[107] = {creType = 6040003,x = 355, y = 6372,z = 687}
P_table[108] = {creType = 6040003,x = 329, y = 5976,z = 711}
P_table[109] = {creType = 6040003,x = 292, y = 5416,z = 728}
P_table[110] = {creType = 6040003,x = 335, y = 6248,z = 658}
P_table[111] = {creType = 6040003,x = 318, y = 5989,z = 683}
P_table[112] = {creType = 6040003,x = 294, y = 5739,z = 671}
P_table[113] = {creType = 6040003,x = 230, y = 4680,z = 716}
P_table[114] = {creType = 6040003,x = 259, y = 4915,z = 746}
P_table[115] = {creType = 6040003,x = 189, y = 4734,z = 715}
P_table[116] = {creType = 6040003,x = 235, y = 4607,z = 769}
P_table[117] = {creType = 6040003,x = 232, y = 4501,z = 801}
P_table[118] = {creType = 6040003,x = 152, y = 4408,z = 743}
P_table[119] = {creType = 6040003,x = 144, y = 4668,z = 705}
P_table[120] = {creType = 6040003,x = 206, y = 4306,z = 807}
P_table[121] = {creType = 6040003,x = 129, y = 4678,z = 722}
P_table[122] = {creType = 6040003,x = 250, y = 4962,z = 695}
P_table[123] = {creType = 6040003,x = 251, y = 5687,z = 647}
P_table[124] = {creType = 6040003,x = 222, y = 4505,z = 748}
P_table[125] = {creType = 6040003,x = 297, y = 5849,z = 644}
P_table[126] = {creType = 6040003,x = 265, y = 5742,z = 646}
--boss旁边2
P_table[127] = {creType = 6040006,x = 241, y = 4706,z = 733}
P_table[128] = {creType = 6040006,x = 281, y = 5434,z = 690}
P_table[129] = {creType = 6040006,x = 217, y = 4413,z = 778}
P_table[130] = {creType = 6040006,x = 174, y = 4734,z = 715}
P_table[131] = {creType = 6040006,x = 183, y = 5688,z = 658}
P_table[132] = {creType = 6040006,x = 203, y = 5688,z = 644}
P_table[133] = {creType = 6040006,x = 179, y = 5688,z = 649}
P_table[134] = {creType = 6040006,x = 195, y = 5688,z = 654}
P_table[135] = {creType = 6040006,x = 240, y = 5691,z = 622}
P_table[136] = {creType = 6040006,x = 335, y = 6241,z = 677}
--
P_table[137] = {creType = 6040006,x = 491, y = 7031,z = 603}
P_table[138] = {creType = 6040006,x = 464, y = 6953,z = 586}
P_table[139] = {creType = 6040006,x = 560, y = 6468,z = 606}
P_table[140] = {creType = 6040006,x = 492, y = 6154,z = 526}
P_table[141] = {creType = 6040006,x = 599, y = 5998,z = 555}
P_table[142] = {creType = 6040006,x = 568, y = 5843,z = 504}
P_table[143] = {creType = 6040006,x = 861, y = 3162,z = 478}
P_table[144] = {creType = 6040006,x = 769, y = 3036,z = 611}
P_table[145] = {creType = 6040006,x = 755, y = 3311,z = 664}
P_table[146] = {creType = 6040006,x = 784, y = 2881,z = 543}
P_table[147] = {creType = 6040006,x = 851, y = 2907,z = 585}
P_table[148] = {creType = 6040006,x = 825, y = 2990,z = 616}
P_table[149] = {creType = 6040006,x = 423, y = 3407,z = 291}
P_table[150] = {creType = 6040006,x = 406, y = 3892,z = 321}
P_table[151] = {creType = 6040006,x = 460, y = 3888,z = 335}
P_table[152] = {creType = 6040006,x = 515, y = 3214,z = 264}
P_table[153] = {creType = 6040006,x = 177, y = 2255,z = 142}
P_table[154] = {creType = 6040006,x = 244, y = 2313,z = 187}
P_table[155] = {creType = 6040006,x = 296, y = 4234,z = 345}
P_table[156] = {creType = 6040006,x = 326, y = 4239,z = 319}
P_table[157] = {creType = 6040006,x = 327, y = 4234,z = 346}
P_table[158] = {creType = 6040006,x = 298, y = 4273,z = 318}
P_table[159] = {creType = 6040006,x = 390, y = 4854,z = 451}
P_table[160] = {creType = 6040006,x = 424, y = 4641,z = 445}
P_table[161] = {creType = 6040006,x = 390, y = 4603,z = 426}

function Ref_random(MapID, InstanceID, tablename, num)
	for i=1,num do
		local n = math.random(table.maxn(tablename))
		local recId = map.MapCreateColCreature(MapID, InstanceID, P_table[tablename[n]].creType, P_table[tablename[n]].x, P_table[tablename[n]].y, P_table[tablename[n]].z, "")
		cre.SetCreatureScriptData(MapID, InstanceID, recId, 1, 1, tablename[n])
		if P_table[tablename[n]].creType == 5610004 or P_table[tablename[n]].creType == 5610005 then
		    unit.AddBuff(MapID, InstanceID, recId, 6100201, recId)
		end
		table.remove(tablename, n)
	end
end

--获得信仰
function getFaith(MapID, InstanceID, addFaith, RoleID)
	local CurTime = tonumber(os.date("%j"))
	local LastTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["canAddFaith"])
	if CurTime ~= LastTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["canAddFaith"], CurTime)
		local n = role.GetRoleAttValue(MapID, InstanceID, RoleID, 124)
		role.ModRoleAttValue(MapID, InstanceID, RoleID, 124, -n)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FaithToday"], 0)
	end
	if role.GetRoleAttValue(MapID, InstanceID, RoleID, 124) < 2000 then
		role.ModRoleAttValue(MapID, InstanceID, RoleID, 124, addFaith)
		if role.GetRoleAttValue(MapID, InstanceID, RoleID, 124) > 2000 then
			local n = role.GetRoleAttValue(MapID, InstanceID, RoleID, 124) - 2000
			role.ModRoleAttValue(MapID, InstanceID, RoleID, 107, addFaith - n)
			role.ModRoleAttValue(MapID, InstanceID, RoleID, 124, -n)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 440008)   --您今日获得的信仰已经达到2000点上限
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    role.ModRoleAttValue(MapID, InstanceID, RoleID, 107, addFaith)
		end
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FaithToday"], role.GetRoleAttValue(MapID, InstanceID, RoleID, 124))
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440008)   --您今日获得的信仰已经达到2000点上限
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

function CheckFaith(MapID, InstanceID, Faith, RoleID)
	local CurTime = tonumber(os.date("%j"))
	local LastTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["canAddFaith"])
	if CurTime ~= LastTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["canAddFaith"], CurTime)
		local n = role.GetRoleAttValue(MapID, InstanceID, RoleID, 124)
		role.ModRoleAttValue(MapID, InstanceID, RoleID, 124, -n)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FaithToday"], 0)
	end
	if role.GetRoleAttValue(MapID, InstanceID, RoleID, 124)+Faith > 2000 then
		local n = role.GetRoleAttValue(MapID, InstanceID, RoleID, 124)+Faith - 2000
		role.ModRoleAttValue(MapID, InstanceID, RoleID, 107, -n)
		role.ModRoleAttValue(MapID, InstanceID, RoleID, 124, Faith-n)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440008)   --您今日获得的信仰已经达到2000点上限
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
	    role.ModRoleAttValue(MapID, InstanceID, RoleID, 124, Faith)
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["FaithToday"], role.GetRoleAttValue(MapID, InstanceID, RoleID, 124))
end

--s54初始化
function s54_OnInit(MapID, InstanceID)
	--1分钟计时器
	map.MapCreateColCreature(MapID, InstanceID, 6040013, 1, 1, 1, "")
	--小动物
	Ref_random(MapID, InstanceID, Tani_table, 10)
	--资源点
	Ref_random(MapID, InstanceID, Trec_table, 10)
	--妖精雕像
	Ref_random(MapID, InstanceID, Tyao_table, 3)
	--怪物1
	Ref_random(MapID, InstanceID, Tcre1_table, 20)
	--怪物2
	Ref_random(MapID, InstanceID, Tcre2_table, 10)
end
--aux.RegisterMapEvent("s54", 0, "s54_OnInit")


--一分钟计时
function cTimer_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	map.SetMapScriptData(MapID, InstanceID, 1, 1, map.GetMapScriptData(MapID, InstanceID, 1, 1) + 1)
	--小动物<10
	if table.maxn(Tani_table) > 15 then
		Ref_random(MapID, InstanceID, Tani_table, 10)
	end
	--资源点<20
	if table.maxn(Trec_table) > 10 then
		Ref_random(MapID, InstanceID, Trec_table, 10)
	end
	--怪物1<20
	if table.maxn(Tcre1_table) > 20 then
		Ref_random(MapID, InstanceID, Tcre1_table, 15)
	end
	--怪物2<10
	if table.maxn(Tcre2_table) > 15 then
		Ref_random(MapID, InstanceID, Tcre2_table, 10)
	end
	--妖精+雕像<3
	if map.GetMapScriptData(MapID, InstanceID, 1, 1) % 3 then
		if table.maxn(Tyao_table) > 7 then
			Ref_random(MapID, InstanceID, Tyao_table, 3)
		end
	end
	local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
	if table.maxn(RoleTbl) < 50 then
		map.MapCreateColCreature(MapID, InstanceID, 6040013, 1, 1, 1, "")
	elseif table.maxn(RoleTbl) < 75 then
		map.MapCreateColCreature(MapID, InstanceID, 6040014, 1, 1, 1, "")
	elseif table.maxn(RoleTbl) < 100 then
		map.MapCreateColCreature(MapID, InstanceID, 6040015, 1, 1, 1, "")
	else
		map.MapCreateColCreature(MapID, InstanceID, 6040016, 1, 1, 1, "")
	end
end
aux.RegisterCreatureEvent(6040013, 13, "cTimer_OnDisappear")
aux.RegisterCreatureEvent(6040014, 13, "cTimer_OnDisappear")
aux.RegisterCreatureEvent(6040015, 13, "cTimer_OnDisappear")
aux.RegisterCreatureEvent(6040016, 13, "cTimer_OnDisappear")



--小动物
function cxiaodongwu_On_Die(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tani_table, index)
	end
end
aux.RegisterCreatureEvent(5610004, 4, "cxiaodongwu_On_Die")
aux.RegisterCreatureEvent(5610005, 4, "cxiaodongwu_On_Die")

function cxiaodongwu_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tani_table, index)
	end
end
aux.RegisterCreatureEvent(5610004, 13, "cxiaodongwu_On_Disappear")
aux.RegisterCreatureEvent(5610005, 13, "cxiaodongwu_On_Disappear")

--资源
function c6040001_On_Invest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Trec_table, index)
	end
	getFaith(MapID, InstanceID, 40, RoleID)
end
aux.RegisterCreatureEvent(6040001, 6, "c6040001_On_Invest")

function c6040001_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Trec_table, index)
	end
end
aux.RegisterCreatureEvent(6040001, 13, "c6040001_On_Disappear")

--妖精雕像
function c6040005_On_Invest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local number = math.random(100)
	if number < 80 then
		local yaoTab = {5610011,5610012,5610013}
		local yaojing = map.MapCreateColCreature(MapID, InstanceID, yaoTab[math.random(3)], x, y, z, "")
		cre.SetCreatureScriptData(MapID, InstanceID, yaojing, 1, 1, index)
	elseif number <= 100 then
	    if index ~= 0 then
			table.insert(Tyao_table, index)
		end
		map.MapCreateCreature(MapID, InstanceID, 6040011, x, y, z)
	end
end
aux.RegisterCreatureEvent(6040005, 6, "c6040005_On_Invest")

function c6040005_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tyao_table, index)
	end
end
aux.RegisterCreatureEvent(6040005, 13, "c6040005_On_Disappear")

--爱心印章兑换师---代币
function c5610011_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 440002)		--    您确定要用10个爱心印章和我兑换1个动物代币吗？动物代币可以在幽怨妖精之魂处换取随机宠物箱。\n    每个人每天只能兑换一个动物代币。
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid 这个只要是服务器控制的对话就必须要写
		msg.AddMsgEvent(MsgID, 21, 4)			--此处的4在玩家选择“取消”之后会作为TalkIndex的值返回
		msg.AddMsgEvent(MsgID, 1, 100001)			--“确定”
		msg.AddMsgEvent(MsgID, 21, -1)			--此处的-1在玩家选择“取消”之后会作为TalkIndex的值返回
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then

		if role.GetRoleScriptData(RoleID, 1, RoleDataType["canGetCoin"]) == tonumber(os.date("%j")) then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 440109)  		--您今天已经兑换过动物代币了，每个人每天只能兑换一次动物代币
			msg.AddMsgEvent(MsgID, 21, -1)			--此处的-1在玩家选择“取消”之后会作为TalkIndex的值返回
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetRoleItemNum(RoleID, 4810019) < 10 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 440003)  		--您背包中没有足够的爱心印章
			msg.AddMsgEvent(MsgID, 21, -1)			--此处的-1在玩家选择“取消”之后会作为TalkIndex的值返回
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810019, 10, 401)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4810020, 1, -1, 1, 104)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["canGetCoin"], tonumber(os.date("%j")))
		end
	end
end
aux.RegisterCreatureEvent(5610011, 7, "c5610011_On_Talk")
--神迹碎片兑换师---信仰
function c5610012_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 440004)		--您确定要用10个神迹碎片和我兑换信仰吗？
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid 这个只要是服务器控制的对话就必须要写
		msg.AddMsgEvent(MsgID, 21, 4)			--此处的4在玩家选择“取消”之后会作为TalkIndex的值返回
		msg.AddMsgEvent(MsgID, 1, 100001)			--“确定”
		msg.AddMsgEvent(MsgID, 21, -1)			--此处的-1在玩家选择“取消”之后会作为TalkIndex的值返回
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810003) < 10 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 440005)  		--您背包中没有足够的神迹碎片
			msg.AddMsgEvent(MsgID, 21, -1)			--此处的-1在玩家选择“取消”之后会作为TalkIndex的值返回
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810003, 10, 401)
			getFaith(MapID, InstanceID, 100, RoleID)
		end
	end
end
aux.RegisterCreatureEvent(5610012, 7, "c5610012_On_Talk")
--战魂碎片兑换师---信仰
function c5610013_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 440006)		--您确定要用30个战魂碎片和我兑换信仰吗？
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid 这个只要是服务器控制的对话就必须要写
		msg.AddMsgEvent(MsgID, 21, 4)			--此处的4在玩家选择“取消”之后会作为TalkIndex的值返回
		msg.AddMsgEvent(MsgID, 1, 100001)			--“确定”
		msg.AddMsgEvent(MsgID, 21, -1)			--此处的-1在玩家选择“取消”之后会作为TalkIndex的值返回
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810010) < 30 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 440007)  		--您背包中没有足够的战魂碎片
			msg.AddMsgEvent(MsgID, 21, -1)			--此处的-1在玩家选择“取消”之后会作为TalkIndex的值返回
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810010, 30, 401)
			getFaith(MapID, InstanceID, 100, RoleID)
		end
	end
end
aux.RegisterCreatureEvent(5610013, 7, "c5610013_On_Talk")


--妖精
function yaojing_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tyao_table, index)
	end
end
aux.RegisterCreatureEvent(5610011, 13, "yaojing_On_Disappear")
aux.RegisterCreatureEvent(5610012, 13, "yaojing_On_Disappear")
aux.RegisterCreatureEvent(5610013, 13, "yaojing_On_Disappear")

--怪物1
function c6040003_On_Invest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local number = math.random(50)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local creTab = {}
	creTab[4] = {ct = 1099004}
	creTab[5] = {ct = 1099005}
	creTab[6] = {ct = 1099006}
	creTab[7] = {ct = 1099007}
	creTab[8] = {ct = 1099008}
	creTab[9] = {ct = 1099009}
	creTab[10] = {ct = 1099010}
	creTab[11] = {ct = 1099011}
	creTab[12] = {ct = 1099012}
	creTab[13] = {ct = 1099013}
	creTab[14] = {ct = 1099014}
	creTab[15] = {ct = 1099015}
	creTab[16] = {ct = 1099016}
	creTab[17] = {ct = 1099017}
	creTab[18] = {ct = 1099018}
	creTab[19] = {ct = 1099019}
	creTab[20] = {ct = 1099020}
	local lev = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if lev > 100 then
		lev = 100
	end
	lev = math.floor(lev/5)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local c1 = map.MapCreateColCreature(MapID, InstanceID, creTab[lev].ct, x, y, z, "")
	cre.SetCreatureScriptData(MapID, InstanceID, c1, 1, 1, index)
	unit.AddBuff(MapID, InstanceID, c1, 6000701, c1)
	if number < 5 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4810002, 1, -1, 1, 104)
	elseif number < 20 then
		unit.AddBuff(MapID, InstanceID, RoleID, 6100101, RoleID)
	end
end
aux.RegisterCreatureEvent(6040003, 6, "c6040003_On_Invest")

function c6040003_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tcre1_table, index)
	end
end
aux.RegisterCreatureEvent(6040003, 13, "c6040003_On_Disappear")

function guaiwu1_On_Die(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tcre1_table, index)
	end
	map.SetMapScriptData(MapID, InstanceID, 1, 2, map.GetMapScriptData(MapID, InstanceID, 1, 2) + 1)
	if map.GetMapScriptData(MapID, InstanceID, 1, 2) == 250 then
		map.MapCreateColCreature(MapID, InstanceID, P_table[1].creType, P_table[1].x, P_table[1].y, P_table[1].z, "")
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 440104)    --魔神拉瑟贝尔的石像已经出现在【190,643】！
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
end
aux.RegisterCreatureEvent(1099004, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099005, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099006, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099008, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099009, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099010, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099011, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099012, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099013, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099014, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099015, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099016, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099017, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099018, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099019, 4, "guaiwu1_On_Die")
aux.RegisterCreatureEvent(1099020, 4, "guaiwu1_On_Die")

function guaiwu1_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tcre1_table, index)
	end
end
aux.RegisterCreatureEvent(1099004, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099005, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099006, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099007, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099008, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099009, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099010, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099011, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099012, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099013, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099014, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099015, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099016, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099017, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099018, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099019, 13, "guaiwu1_On_Disappear")
aux.RegisterCreatureEvent(1099020, 13, "guaiwu1_On_Disappear")

--怪物2
function c6040006_On_Invest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local creTab = {}
	creTab = {1098001,1098002,1098003,1098004,1098005,1098006,1098007,1098008,1098009}
	local lev = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if lev > 100 then
		lev = 100
	end
	lev = math.floor(lev/10 - 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local c2 = map.MapCreateColCreature(MapID, InstanceID, creTab[lev], x, y, z, "")
	cre.SetCreatureScriptData(MapID, InstanceID, c2, 1, 1, index)
	unit.AddBuff(MapID, InstanceID, c2, 6000701, c2)
end
aux.RegisterCreatureEvent(6040006, 6, "c6040006_On_Invest")

function c6040006_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tcre2_table, index)
	end
end
aux.RegisterCreatureEvent(6040006, 13, "c6040006_On_Disappear")

function guaiwu2_On_Die(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tcre2_table, index)
	end
	map.SetMapScriptData(MapID, InstanceID, 1, 2, map.GetMapScriptData(MapID, InstanceID, 1, 2) + 1)
	if map.GetMapScriptData(MapID, InstanceID, 1, 2) == 250 then
		map.MapCreateColCreature(MapID, InstanceID, P_table[1].creType, P_table[1].x, P_table[1].y, P_table[1].z, "")
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 440104)    --魔神拉瑟贝尔的石像已经出现在【190,643】！
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
end
aux.RegisterCreatureEvent(1098001, 4, "guaiwu2_On_Die")
aux.RegisterCreatureEvent(1098002, 4, "guaiwu2_On_Die")
aux.RegisterCreatureEvent(1098003, 4, "guaiwu2_On_Die")
aux.RegisterCreatureEvent(1098004, 4, "guaiwu2_On_Die")
aux.RegisterCreatureEvent(1098005, 4, "guaiwu2_On_Die")
aux.RegisterCreatureEvent(1098006, 4, "guaiwu2_On_Die")
aux.RegisterCreatureEvent(1098007, 4, "guaiwu2_On_Die")
aux.RegisterCreatureEvent(1098008, 4, "guaiwu2_On_Die")
aux.RegisterCreatureEvent(1098009, 4, "guaiwu2_On_Die")

function guaiwu2_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local index = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if index ~= 0 then
		table.insert(Tcre1_table, index)
	end
end
aux.RegisterCreatureEvent(1098001, 13, "guaiwu2_On_Disappear")
aux.RegisterCreatureEvent(1098002, 13, "guaiwu2_On_Disappear")
aux.RegisterCreatureEvent(1098003, 13, "guaiwu2_On_Disappear")
aux.RegisterCreatureEvent(1098004, 13, "guaiwu2_On_Disappear")
aux.RegisterCreatureEvent(1098005, 13, "guaiwu2_On_Disappear")
aux.RegisterCreatureEvent(1098006, 13, "guaiwu2_On_Disappear")
aux.RegisterCreatureEvent(1098007, 13, "guaiwu2_On_Disappear")
aux.RegisterCreatureEvent(1098008, 13, "guaiwu2_On_Disappear")
aux.RegisterCreatureEvent(1098009, 13, "guaiwu2_On_Disappear")

--boss
function c6040004_On_Invest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 440105)    --魔神拉瑟贝尔已经复活！
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	local bosId = map.MapCreateColCreature(MapID, InstanceID, 1099021, P_table[1].x, P_table[1].y, P_table[1].z, "")    --未指定bossId
	unit.AddBuff(MapID, InstanceID, bosId, 6100801, bosId)
	local creTab1 = {}
	creTab1[4] = {ct = 1099004}
	creTab1[5] = {ct = 1099005}
	creTab1[6] = {ct = 1099006}
	creTab1[7] = {ct = 1099007}
	creTab1[8] = {ct = 1099008}
	creTab1[9] = {ct = 1099009}
	creTab1[10] = {ct = 1099010}
	creTab1[11] = {ct = 1099011}
	creTab1[12] = {ct = 1099012}
	creTab1[13] = {ct = 1099013}
	creTab1[14] = {ct = 1099014}
	creTab1[15] = {ct = 1099015}
	creTab1[16] = {ct = 1099016}
	creTab1[17] = {ct = 1099017}
	creTab1[18] = {ct = 1099018}
	creTab1[19] = {ct = 1099019}
	creTab1[20] = {ct = 1099020}
	local lev1 = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if lev1 > 100 then
		lev1 = 100
	end
	lev1 = math.floor(lev1/5)
	for i = 107,126 do
		local ID = map.MapCreateColCreature(MapID, InstanceID, creTab1[lev1].ct, P_table[i].x, P_table[i].y, P_table[i].z, "")
		unit.AddBuff(MapID, InstanceID, ID, 6000701, ID)
	end
	local creTab2 = {}
	creTab2 = {1098001,1098002,1098003,1098004,1098005,1098006,1098007,1098008,1098009}
	local lev2 = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if lev2 > 100 then
		lev2 = 100
	end
	lev2 = math.floor(lev2/10 - 1)
	for i = 127,136 do
		local ID = map.MapCreateColCreature(MapID, InstanceID, creTab2[lev2], P_table[i].x, P_table[i].y, P_table[i].z, "")
		unit.AddBuff(MapID, InstanceID, ID, 6000701, ID)
	end
	--cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, bosId)
end
aux.RegisterCreatureEvent(6040004, 6, "c6040004_On_Invest")



function c1099021_On_Die(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
	if RoleTbl ~= nil then
		for k, v in pairs(RoleTbl) do
			getFaith(MapID, InstanceID, 500, v)
		end
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 300048)  --魔神拉瑟贝尔已经被击败，所有处于上古妖精庇护所的玩家将获得300点信仰。
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end
aux.RegisterCreatureEvent(1099021, 4, "c1099021_On_Die")


--传送门的对话
function c5610003_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmin = tonumber(os.date("%M"))--分钟
	if TalkIndex == -1 then
	    if curmin == 0 or (curmin>=50 and curmin<=59) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 440106)		--这是一个通往上古妖精庇护所的传送门，只有50级以上的玩家才能通过。你想进入上过妖精庇护所吗？
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 4)			--
			msg.AddMsgEvent(MsgID, 1, 100001)			--“确定”
			msg.AddMsgEvent(MsgID, 21, -1)			--
			msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 442050)		--只有每个小时的最后10分钟才能进入上古妖精庇护所，请您等到下次庇护所开放的时候再来。
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 8)			--
			msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 4 then
		if role.GetRoleLevel(MapID, InstanceID, RoleID) < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 440107)  		--50级以下的玩家无法进入上古妖精庇护所。
			msg.AddMsgEvent(MsgID, 21, -1)
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local curmin = tonumber(os.date("%M"))--分钟
			if curmin == 0 or (curmin>=50 and curmin<=59) then
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3373740328, 128, 48, 340)
				if map.GetMapScriptData(3373740328, SGYJBHS_InstanceID%100000, 1, 3) == 0 then
					s54_OnInit(3373740328, SGYJBHS_InstanceID%100000)
					map.SetMapScriptData(3373740328, SGYJBHS_InstanceID%100000, 1, 3, 1)
				end
				SGYJBHS_From[RoleID] = MapID
			end
		end
	end
end
aux.RegisterCreatureEvent(5610003, 7, "c5610003_On_Talk")

function s54_PlayerEnter(MapID, InstanceID, RoleID)
    local Hour  	= tonumber(os.date("%H"))
	local Min  		= tonumber(os.date("%M"))
	local Second	= tonumber(os.date("%S"))
	local leftSec = 0
	if Min >41 then
	    leftSec = (59 - Min) * 60 + 59 - Second + 1800   --活动剩余时间
	else
	    leftSec = (29 - Min) * 60 + 59 - Second
	end
	--同步客户端时间
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 16,	29)								--
	msg.AddMsgEvent(MsgID, 16,	leftSec)							--剩余时间
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	--庇护所每日人数
	UpdateSeverData(38, 1)
end

--注册进入活动地图事件
aux.RegisterMapEvent("s54", 2, "s54_PlayerEnter")

function s54_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	local mappoint = {}
	mappoint[3424072488] = {x=1094,y=3920,z=1421}
	mappoint[3424072232] = {x=574,y=12973,z=1974}
	mappoint[3424075048] = {x=1618,y=11498,z=2082}
	if SGYJBHS_From[RoleID] ~= nil then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, SGYJBHS_From[RoleID], mappoint[SGYJBHS_From[RoleID]].x+math.random(5), mappoint[SGYJBHS_From[RoleID]].y, mappoint[SGYJBHS_From[RoleID]].z+math.random(5))
	else
	    --local MapID,InstanceID = role.GetRoleMapID(RoleID)
	    local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local Index = 0
		if level < 65 then
		    Index = 3424072488
		elseif level <80 then
		    Index = 3424072232
		else
		    Index = 3424075048
		end
	    role.RoleGotoNewMap(MapID, InstanceID, RoleID, Index, mappoint[Index].x+math.random(5), mappoint[Index].y, mappoint[Index].z+math.random(5))
	end
end
aux.RegisterMapEvent("s54",6,"s54_OnEnterTrigger")


--找到最合适的副本实例
function s54GetOnePerfectMap(MapID, InstanceID, RoleID)

	return SGYJBHS_InstanceID%100000

end

aux.RegisterMapEvent("s54",  14, "s54GetOnePerfectMap")


--玩家在活动中掉线后上线
function s54_CanTakeOverWhenOnline(MapID, RoleID)
	--修正玩家的上线地图和坐标todo
	local mappoint = {}
	mappoint[3424072488] = {x=1094,y=3920,z=1421}
	mappoint[3424072232] = {x=574,y=12973,z=1974}
	mappoint[3424075048] = {x=1618,y=11498,z=2082}
	if SGYJBHS_From[RoleID] ~= nil then
		return SGYJBHS_From[RoleID], mappoint[SGYJBHS_From[RoleID]].x+math.random(5), mappoint[SGYJBHS_From[RoleID]].y, mappoint[SGYJBHS_From[RoleID]].z+math.random(5)
	else
	    --local MapID,InstanceID = role.GetRoleMapID(RoleID)
	   -- local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local Index = 0
		--if level < 65 then
		    Index = 3424072488
		--elseif level <80 then
		--    Index = 3424072232
		--else
		--    Index = 3424075048
		--end
	    return Index, mappoint[Index].x+math.random(5), mappoint[Index].y, mappoint[Index].z+math.random(5)
	end
end

aux.RegisterMapEvent("s54",  12, "s54_CanTakeOverWhenOnline")


-------------------------------------------------------------------------------------------------------
--找到地图的出口和坐标
function s54_GetExporMapAndCoord(MapID, InstanceID, RoleID)
    local mappoint = {}
	mappoint[3424072488] = {x=1094,y=3920,z=1421}
	mappoint[3424072232] = {x=574,y=12973,z=1974}
	mappoint[3424075048] = {x=1618,y=11498,z=2082}
	if SGYJBHS_From[RoleID] ~= nil then
		return 1, SGYJBHS_From[RoleID], mappoint[SGYJBHS_From[RoleID]].x+math.random(5), mappoint[SGYJBHS_From[RoleID]].y, mappoint[SGYJBHS_From[RoleID]].z+math.random(5)
	else
	    local MapID,InstanceID = role.GetRoleMapID(RoleID)
	    local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local Index = 0
		if level < 65 then
		    Index = 3424072488
		elseif level <80 then
		    Index = 3424072232
		else
		    Index = 3424075048
		end
	    return 1, Index, mappoint[Index].x+math.random(5), mappoint[Index].y, mappoint[Index].z+math.random(5)
	end

end

aux.RegisterMapEvent("s54",  13, "s54_GetExporMapAndCoord")


--检测玩家是否能够进入活动场景
function s54_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("s54",  15, "s54_CanEnter")





