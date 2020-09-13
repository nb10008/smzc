function I4700146_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {3040033,1,-1,480}
	tbl[2] = {3040038,1,-1,480}
	tbl[3] = {4999004,1,-1,200}
	tbl[4] = {4530108,3,-1,500}
	tbl[5] = {3020082,1,-1,200}
	tbl[6] = {3040043,1,-1,480}
	tbl[7] = {3040048,1,-1,300}
	tbl[8] = {3070002,3,-1,800}
	tbl[9] = {4080097,1,-1,650}
	tbl[10] = {4500002,3,-1,800}
	tbl[11] = {4800132,1,-1,650}
	tbl[12] = {5600004,1,-1,650}
	tbl[13] = {3040053,1,-1,300}
	tbl[14] = {3040058,1,-1,300}
	tbl[15] = {3040003,1,-1,480}
	tbl[16] = {3040008,1,-1,480}
	tbl[17] = {3040013,1,-1,480}
	tbl[18] = {3040018,1,-1,480}
	tbl[19] = {3040023,1,-1,480}
	tbl[20] = {3040028,1,-1,480}

	tbl[21] = {3060058,1,-1,100}
	tbl[22] = {3060060,1,-1,200}
	tbl[23] = {3020089,1,-1,15}
	tbl[24] = {3020090,1,-1,5}
	tbl[25] = {4211103,1,-1,5}
	tbl[26] = {4221004,1,-1,5}
	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

	if index >= 21 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

function I4700146_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700146, 1, "I4700146_GiftBag")
aux.RegisterItemEvent(4700146, 0, "I4700146_CanUseGiftBag")
