function I4830068_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(1000000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={4700301,1,40000}
	tbl[2]={4700302,2,50000}
	tbl[3]={4820035,1,35000}
	tbl[4]={4700226,1,34500}
	tbl[5]={4800254,1,34500}
	tbl[6]={4700231,1,57000}
	tbl[7]={3060005,120,65000}
	tbl[8]={4820087,4,40000}
	tbl[9]={5600004,1,60000}
	tbl[10]={4820047,2,57000}
	tbl[11]={4820046,2,65000}
	tbl[12]={3500014,1,50000}
	tbl[13]={3500015,1,45000}
	tbl[14]={3500022,1,45000}
	tbl[15]={3500004,1,45000}
	tbl[16]={3500010,1,65000}
	tbl[17]={3020102,1,0}
	tbl[18]={3510002,5,65000}
	tbl[19]={4400031,1,55000}
	tbl[20]={3060060,1,35000}
	tbl[21]={4700320,1,500}
	tbl[22]={4700322,1,500}
	tbl[23]={4820261,1,3090}
	tbl[24]={4820260,1,1400}
	tbl[25]={3060061,1,2}
	tbl[26]={4820179,1,2500}
	tbl[27]={4820256,1,20000}
	tbl[28]={4999005,1,20000}
	tbl[29]={3030119,1,1500}
	tbl[30]={4820180,1,3000}
	tbl[31]={3030050,1,4}
	tbl[32]={3030056,1,4}
	tbl[33]={3020035,1,2500}
	tbl[34]={3060058,1,2000}

	for i = 1,34 do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end
	if index >= 21 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)

end

function I4830068_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830068, 1, "I4830068_GiftBag")
aux.RegisterItemEvent(4830068, 0, "I4830068_CanUseGiftBag")
