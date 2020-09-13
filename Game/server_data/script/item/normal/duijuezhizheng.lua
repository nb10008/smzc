function duijuezhizheng_Use(MapID, InstanceID, TypeID, TargetID)
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
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if rolelevel > 100 then
	    rolelevel = 100
	end
	if rolelevel >= 30 then
		role.AddRoleExp(MapID, InstanceID, TargetID, tbl[rolelevel]*0.5)
	else
		role.AddRoleExp(MapID, InstanceID, TargetID, rolelevel*700)
	end
end
aux.RegisterItemEvent(4800282, 1, "duijuezhizheng_Use")
