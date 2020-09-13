function I4700250_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100000000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {3070052,8,-1,5500000}
	tbl[2] = {5600004,1,-1,3500000}
	tbl[3] = {3040060,1,-1,300000}
	tbl[4] = {3040025,1,-1,300000}
	tbl[5] = {3020029,1,-1,3000000}
	tbl[6] = {4090031,1,-1,1500000}
	tbl[7] = {4090029,1,-1,1500000}
	tbl[8] = {4820047,2,-1,3500000}
	tbl[9] = {4820046,2,-1,4000000}
	tbl[10] = {4820108,3,-1,3500000}
	tbl[11] = {3070018,2,-1,5500000}
	tbl[12] = {4500002,8,-1,5500000}
	tbl[13] = {4400031,1,-1,5000000}
	tbl[14] = {3070023,1,-1,5000000}
	tbl[15] = {3060059,1,-1,3500000}
	tbl[16] = {3060060,1,-1,3000000}
	tbl[17] = {4090028,1,-1,1500000}
	tbl[18] = {4700230,1,-1,5000000}
	tbl[19] = {4700229,1,-1,6000000}
	tbl[20] = {4800155,1,-1,3490000}
	tbl[21] = {4800154,1,-1,3000000}
	tbl[22] = {4820048,50,-1,4000000}
	tbl[23] = {4820061,15,-1,5000000}
	tbl[24] = {4820087,1,-1,2000000}
	tbl[25] = {4090030,1,-1,1500000}
	tbl[26] = {4090019,1,-1,3000000}
	tbl[27] = {4090021,1,-1,3000000}
	tbl[28] = {4820239,1,-1,5500000}

	tbl[29] = {3020042,1,-1,1}
	tbl[30] = {3020037,1,-1,9}
	tbl[31] = {3020035,1,-1,40000}
	tbl[32] = {3020087,1,-1,150000}
	tbl[33] = {3020088,1,-1,100000}
	tbl[34] = {3020091,1,-1,30000}
	tbl[35] = {3020092,1,-1,20000}
	tbl[36] = {4820180,1,-1,160000}
	tbl[37] = {4820179,1,-1,300000}
	tbl[38] = {3060058,1,-1,209990}
	tbl[39] = {4700231,1,-1,500000}
	tbl[40] = {4700248,1,-1,20000}
	tbl[41] = {4700249,1,-1,20000}
	tbl[42] = {4820252,1,-1,60000}
	tbl[43] = {4090020,1,-1,300000}
	tbl[44] = {4090027,1,-1,1000000}
	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

	if index >= 29 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

function I4700250_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700250, 1, "I4700250_GiftBag")
aux.RegisterItemEvent(4700250, 0, "I4700250_CanUseGiftBag")
