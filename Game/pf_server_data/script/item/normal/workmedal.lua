--51ÀÍ¶¯½±ÕÂ
function I4800098_Use(MapID, InstanceID, TypeID, TargetID)
	local exp_lv = {}
	exp_lv[11] = 22800
	exp_lv[12] = 24850
	exp_lv[13] = 26850
	exp_lv[14] = 28900
	exp_lv[15] = 41300
	exp_lv[16] = 44100
	exp_lv[17] = 46900
	exp_lv[18] = 49750
	exp_lv[19] = 52600
	exp_lv[20] = 72950
	exp_lv[21] = 76600
	exp_lv[22] = 80300
	exp_lv[23] = 84000
	exp_lv[24] = 87750
	exp_lv[25] = 91550
	exp_lv[26] = 95350
	exp_lv[27] = 99200
	exp_lv[28] = 103050
	exp_lv[29] = 140650
	exp_lv[30] = 249500
	exp_lv[31] = 266250
	exp_lv[32] = 285750
	exp_lv[33] = 305400
	exp_lv[34] = 325200
	exp_lv[35] = 345150
	exp_lv[36] = 365300
	exp_lv[37] = 505750
	exp_lv[38] = 505750
	exp_lv[39] = 514100
	exp_lv[40] = 544950
	exp_lv[41] = 576000
	exp_lv[42] = 607350
	exp_lv[43] = 710050
	exp_lv[44] = 746050
	exp_lv[45] = 746050
	exp_lv[46] = 818850
	exp_lv[47] = 855600
	exp_lv[48] = 896400
	exp_lv[49] = 1171900
	exp_lv[50] = 1223650
	exp_lv[51] = 1275750
	exp_lv[52] = 1333000
	exp_lv[53] = 1390650
	exp_lv[54] = 1448750
	exp_lv[55] = 1507200
	exp_lv[56] = 1566100
	exp_lv[57] = 1625400
	exp_lv[58] = 2028000
	exp_lv[59] = 2106000
	exp_lv[60] = 2184600
	exp_lv[61] = 2263700
	exp_lv[62] = 2349250
	exp_lv[63] = 2522150
	exp_lv[64] = 2609500
	exp_lv[65] = 2697400
	exp_lv[66] = 2785900
	exp_lv[67] = 3969000
	exp_lv[68] = 4097550
	exp_lv[69] = 4226900
	exp_lv[70] = 4226900
	exp_lv[71] = 4365300
	exp_lv[72] = 4365300
	exp_lv[73] = 4504600
	exp_lv[74] = 4644750
	exp_lv[75] = 4785850
	exp_lv[76] = 4927850
	exp_lv[77] = 5070750
	exp_lv[78] = 7834350
	exp_lv[79] = 8064000
	exp_lv[80] = 8295150
	exp_lv[81] = 8527700
	exp_lv[82] = 8774450
	exp_lv[83] = 9022700
	exp_lv[84] = 9272500
	exp_lv[85] = 9523800
	exp_lv[86] = 9776700
	exp_lv[87] = 10031050
	exp_lv[88] = 10300000
	exp_lv[89] = 14094000
	exp_lv[90] = 14456900
	exp_lv[91] = 14821950
	exp_lv[92] = 15206700
	exp_lv[93] = 15593800
	exp_lv[94] = 15983100
	exp_lv[95] = 16374750
	exp_lv[96] = 16768650
	exp_lv[97] = 17164800
	exp_lv[98] = 17581200
	exp_lv[99] = 18000000
	exp_lv[100] = 18000000
	local 	rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if rolelevel > 100 then
		rolelevel = 100
	end
	if rolelevel <=	10 then
		role.AddRoleExp(MapID, InstanceID, TargetID, exp_lv[11])
	else
		role.AddRoleExp(MapID, InstanceID, TargetID, exp_lv[rolelevel])
	end

 end
 aux.RegisterItemEvent(4800098, 1, "I4800098_Use")
