function I4700232_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {3070052,8,-1,4000}
	tbl[2] = {5600006,1,-1,2000}
	tbl[3] = {4999005,1,-1,4000}
	tbl[4] = {4820107,50,-1,1500}
	tbl[5] = {3030011,1,-1,5000}
	tbl[6] = {3030003,1,-1,5000}
	tbl[7] = {5600004,1,-1,2000}
	tbl[8] = {4820048,40,-1,9920}
	tbl[9] = {3060059,1,-1,5000}
	tbl[10] = {4820239,1,-1,3000}
	tbl[11] = {4820172,1,-1,6650}
	tbl[12] = {4820084,1,-1,7500}
	tbl[13] = {4500002,8,-1,3000}
	tbl[14] = {3040059,1,-1,600}
	tbl[15] = {3040024,1,-1,810}
	tbl[16] = {4820046,1,-1,5550}
	tbl[17] = {3050068,1,-1,1000}
	tbl[18] = {3050069,1,-1,1000}
	tbl[19] = {3050070,1,-1,1000}
	tbl[20] = {4080071,6,-1,2000}
	tbl[21] = {4700229,1,-1,3000}
	tbl[22] = {4820120,1,-1,750}
	tbl[23] = {4820121,1,-1,750}
	tbl[24] = {4820047,1,-1,5550}
	tbl[25] = {3030095,1,-1,2000}
	tbl[26] = {4800155,1,-1,2000}
	tbl[27] = {5600008,1,-1,2000}
	tbl[28] = {3070021,2,-1,3000}
	tbl[29] = {3070019,2,-1,5000}

	tbl[30] = {4820035,1,-1,1000}
	tbl[31] = {3020042,1,-1,1}
	tbl[32] = {3020037,1,-1,9}
	tbl[33] = {3020092,1,-1,10}
	tbl[34] = {3020091,1,-1,20}
	tbl[35] = {4820180,1,-1,100}
	tbl[36] = {3020090,1,-1,20}
	tbl[37] = {3020089,1,-1,20}
	tbl[38] = {3020088,1,-1,20}
	tbl[39] = {3020033,1,-1,20}
	tbl[40] = {4700231,1,-1,50}
	tbl[41] = {3040060,1,-1,50}
	tbl[42] = {4700230,1,-1,2000}
	tbl[43] = {3040025,1,-1,100}
	tbl[44] = {3020029,1,-1,2000}
	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 35)

	if index >= 30 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

function I4700232_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700232, 1, "I4700232_GiftBag")
aux.RegisterItemEvent(4700232, 0, "I4700232_CanUseGiftBag")
