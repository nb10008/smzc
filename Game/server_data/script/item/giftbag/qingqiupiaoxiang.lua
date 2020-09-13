function I4810055_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={3100226,1,1425}
	tbl[2]={3100228,1,1425}
	tbl[3]={3100230,1,1430}
	tbl[4]={3100234,1,1430}
	tbl[5]={3100236,1,1430}
	tbl[6]={3100238,1,1430}
	tbl[7]={3100240,1,1430}
	

	for i = 1,7 do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end
	
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)

end
aux.RegisterItemEvent(4810055, 1, "I4810055_GiftBag")

function I4810055_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4810055, 0, "I4810055_CanUseGiftBag")

function I4810056_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={3100227,1,1425}
	tbl[2]={3100229,1,1425}
	tbl[3]={3100231,1,1430}
	tbl[4]={3100235,1,1430}
	tbl[5]={3100237,1,1430}
	tbl[6]={3100239,1,1430}
	tbl[7]={3100241,1,1430}

	for i = 1,7 do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)

end
aux.RegisterItemEvent(4810056, 1, "I4810056_GiftBag")

function I4810056_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4810056, 0, "I4810056_CanUseGiftBag")

function I4810057_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={9800301,1,2500}
	tbl[2]={9800302,1,2500}
	tbl[3]={9800303,1,2500}
	tbl[4]={9800304,1,2500}


	for i = 1,4 do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)

end
aux.RegisterItemEvent(4810057, 1, "I4810057_GiftBag")

function I4810057_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4810057, 0, "I4810057_CanUseGiftBag")


function I4810058_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={4530097,1,30}
	tbl[2]={4530096,1,50}
	tbl[3]={3500058,1,900}
	tbl[4]={3500057,1,80}
	tbl[5]={3500022,1,450}
	tbl[6]={3500039,1,700}
	tbl[7]={3500201,1,800}
	tbl[8]={3500203,1,850}
	tbl[9]={3500206,1,350}
	tbl[10]={3500207,1,100}
	tbl[11]={3500208,1,20}
	tbl[12]={3540103,1,10}
	tbl[13]={3561001,1,80}
	tbl[14]={4212028,1,5}
	tbl[15]={4810057,1,5}
	tbl[16]={4820047,3,950}
	tbl[17]={4820046,3,950}
	tbl[18]={4500002,3,950}
	tbl[19]={3070051,3,950}
	tbl[20]={4820061,3,950}
	tbl[21]={4810055,1,450}
	tbl[22]={4810056,1,370}

	for i = 1,22 do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end
	if index == 1 or index == 2 or index == 4 or index == 11 or index == 12 or index == 14 or index == 15 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)

end
aux.RegisterItemEvent(4810058, 1, "I4810058_GiftBag")

function I4810058_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4810058, 0, "I4810058_CanUseGiftBag")