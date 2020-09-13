function I4700140_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {5600004,1,-1,800}
	tbl[2] = {4400031,1,-1,200}
	tbl[3] = {4500002,10,-1,750}
	tbl[4] = {4800132,1,-1,800}
	tbl[5] = {4999004,1,-1,200}
	tbl[6] = {3020016,1,-1,150}
	tbl[7] = {3070003,1,-1,300}
	tbl[8] = {3070004,1,-1,300}
	tbl[9] = {3100099,1,-1,400}
	tbl[10] = {3100109,1,-1,400}
	tbl[11] = {3100119,1,-1,400}
	tbl[12] = {3100129,1,-1,400}
	tbl[13] = {3100139,1,-1,400}
	tbl[14] = {3100149,1,-1,400}
	tbl[15] = {3100159,1,-1,400}
	tbl[16] = {3100169,1,-1,400}
	tbl[17] = {3020082,1,-1,350}
	tbl[18] = {3070052,10,-1,750}
	tbl[19] = {4080097,1,-1,776}
	tbl[20] = {3030095,1,-1,650}
	tbl[21] = {3030101,1,-1,650}
	tbl[22] = {3020095,1,-1,1}
	tbl[23] = {3020090,1,-1,8}
	tbl[24] = {3020092,1,-1,5}
	tbl[25] = {4211503,1,-1,5}
	tbl[26] = {4220804,1,-1,5}
	tbl[27] = {3060058,1,-1,100}

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

	if index >= 22 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

function I4700140_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700140, 1, "I4700140_GiftBag")
aux.RegisterItemEvent(4700140, 0, "I4700140_CanUseGiftBag")
