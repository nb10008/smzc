--经验券
function I4820037_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local tbl = {}
	tbl[30] = 2100
	tbl[31] = 2160
	tbl[32] = 2300
	tbl[33] = 2460
	tbl[34] = 2610
	tbl[35] = 2770
	tbl[36] = 2930
	tbl[37] = 3090
	tbl[38] = 3160
	tbl[39] = 3300
	tbl[40] = 3560
	tbl[41] = 3750
	tbl[42] = 3960
	tbl[43] = 4170
	tbl[44] = 5530
	tbl[45] = 5790
	tbl[46] = 6050
	tbl[47] = 6300
	tbl[48] = 6560
	tbl[49] = 6840
	tbl[50] = 7130
	tbl[51] = 7410
	tbl[52] = 7700
	tbl[53] = 8010
	tbl[54] = 8330
	tbl[55] = 8640
	tbl[56] = 8950
	tbl[57] = 9270
	tbl[58] = 9580
	tbl[59] = 11750
	tbl[60] = 12150
	tbl[61] = 12560
	tbl[62] = 12960
	tbl[63] = 13400
	tbl[64] = 13800
	tbl[65] = 14280
	tbl[66] = 14720
	tbl[67] = 15160
	tbl[68] = 15600
	tbl[69] = 18930
	tbl[70] = 19480
	tbl[71] = 20040
	tbl[72] = 20600
	tbl[73] = 21190
	tbl[74] = 21790
	tbl[75] = 25340
	tbl[76] = 26010
	tbl[77] = 26690
	tbl[78] = 27360
	tbl[79] = 28080
	tbl[80] = 28800
	tbl[81] = 29520
	tbl[82] = 30240
	tbl[83] = 31010
	tbl[84] = 31770
	tbl[85] = 36880
	tbl[86] = 37740
	tbl[87] = 38610
	tbl[88] = 39480
	tbl[89] = 40400
	tbl[90] = 41310
	tbl[91] = 42230
	tbl[92] = 43150
	tbl[93] = 44120
	tbl[94] = 45090
	tbl[95] = 50800
	tbl[96] = 51870
	tbl[97] = 52940
	tbl[98] = 54000
	tbl[99] = 55130
	tbl[100] = 56250
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if level > 100 then
		level = 100
	end
    role.AddRoleExp(MapID, InstanceID, TargetID, tbl[level])

end

aux.RegisterItemEvent(4820037, 1, "I4820037_GiftBag")


--经验宝瓶
function I4820043_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local tbl = {}
	tbl[30] = 21000
	tbl[31] = 21600
	tbl[32] = 23000
	tbl[33] = 24600
	tbl[34] = 26100
	tbl[35] = 27700
	tbl[36] = 29300
	tbl[37] = 30900
	tbl[38] = 31600
	tbl[39] = 33000
	tbl[40] = 35600
	tbl[41] = 37500
	tbl[42] = 39600
	tbl[43] = 41700
	tbl[44] = 55300
	tbl[45] = 57900
	tbl[46] = 60500
	tbl[47] = 63000
	tbl[48] = 65600
	tbl[49] = 68400
	tbl[50] = 71300
	tbl[51] = 74100
	tbl[52] = 77000
	tbl[53] = 80100
	tbl[54] = 83300
	tbl[55] = 86400
	tbl[56] = 89500
	tbl[57] = 92700
	tbl[58] = 95800
	tbl[59] = 117500
	tbl[60] = 121500
	tbl[61] = 125600
	tbl[62] = 129600
	tbl[63] = 134000
	tbl[64] = 138000
	tbl[65] = 142800
	tbl[66] = 147200
	tbl[67] = 151600
	tbl[68] = 156000
	tbl[69] = 189300
	tbl[70] = 194800
	tbl[71] = 200400
	tbl[72] = 206000
	tbl[73] = 211900
	tbl[74] = 217900
	tbl[75] = 253400
	tbl[76] = 260100
	tbl[77] = 266900
	tbl[78] = 273600
	tbl[79] = 280800
	tbl[80] = 288000
	tbl[81] = 295200
	tbl[82] = 302400
	tbl[83] = 310100
	tbl[84] = 317700
	tbl[85] = 368800
	tbl[86] = 377400
	tbl[87] = 386100
	tbl[88] = 394800
	tbl[89] = 404000
	tbl[90] = 413100
	tbl[91] = 422300
	tbl[92] = 431500
	tbl[93] = 441200
	tbl[94] = 450900
	tbl[95] = 508000
	tbl[96] = 518700
	tbl[97] = 529400
	tbl[98] = 540000
	tbl[99] = 551300
	tbl[100] = 562500

	for i = 70,100 do
	    tbl[i] = tbl[i] + (i-69)*250000
	end

	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if level > 100 or level < 30 then
	    level = 100
	end
    role.AddRoleExp(MapID, InstanceID, TargetID, tbl[level])
	if math.random(10) == 7 then
	     role.AddRoleExp(MapID, InstanceID, TargetID, tbl[level]*0.5)
	end

end

aux.RegisterItemEvent(4820043, 1, "I4820043_GiftBag")
aux.RegisterItemEvent(4700278, 1, "I4820043_GiftBag")
