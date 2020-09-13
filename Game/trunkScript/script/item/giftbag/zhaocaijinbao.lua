function I4700260_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(1000000)
	local temp = 0
	local index = 1
	local tbl = {}




    tbl[1]={3020030,1,-1,20000}
    tbl[2]={4820109,15,-1,40000}
    tbl[3]={4820120,1,-1,20000}
    tbl[4]={4820121,1,-1,20000}
    tbl[5]={4090027,1,-1,20000}
    tbl[6]={4090011,8,-1,25000}
    tbl[7]={4700229,1,-1,55000}
    tbl[8]={4700230,1,-1,55000}
    tbl[9]={4700231,1,-1,13000}
	tbl[10]={4820239,1,-1,40000}
    tbl[11]={3020011,1,-1,35000}
    tbl[12]={4400031,1,-1,55000}
    tbl[13]={3070023,2,-1,55000}
    tbl[14]={4820087,1,-1,25000}
    tbl[15]={4090010,8,-1,25000}
    tbl[16]={4090030,1,-1,17000}
    tbl[17]={4090028,1,-1,17000}
    tbl[18]={4090031,1,-1,17000}
    tbl[19]={4090029,1,-1,17000}
    tbl[20]={4820061,15,-1,50000}
    tbl[21]={4820047,2,-1,40000}
    tbl[22]={4820046,2,-1,45000}
    tbl[23]={4500002,8,-1,55000}
    tbl[24]={3070051,8,-1,55000}
    tbl[25]={5600003,1,-1,40000}
    tbl[26]={4800154,1,-1,50000}
    tbl[27]={4800155,1,-1,45500}
	tbl[28]={4800156,1,-1,15500}

    tbl[29]={4820256,1,-1,10000}
	tbl[30]={3020012,1,-1,8000}
	tbl[31]={3020008,1,-1,5500}
	tbl[32]={4820179,1,-1,2000}
	tbl[33]={4820180,1,-1,1500}
	tbl[34]={4820252,1,-1,1000}
	tbl[35]={3020089,1,-1,1000}
	tbl[36]={3090035,1,-1,1000}
    tbl[37]={3090036,1,-1,1000}
	tbl[38]={4212008,1,-1,400}
	tbl[39]={4222104,1,-1,400}
	tbl[40]={3020092,1,-1,300}
    tbl[41]={3020091,1,-1,400}
	tbl[42]={3020035,1,-1,400}
    tbl[43]={3020047,1,-1,1}
    tbl[44]={3020042,1,-1,2}
    tbl[45]={3020037,1,-1,97}

	for i = 1,45 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end
	if index >= 29 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700260_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700260, 1, "I4700260_GiftBag")
aux.RegisterItemEvent(4700260, 0, "I4700260_CanUseGiftBag")
