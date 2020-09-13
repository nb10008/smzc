function I4700049_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {5600004,1,1,1350}
	tbl[2] = {4400031,1,1,308}
	tbl[3] = {4500002,10,1,1500}
	tbl[4] = {3070052,10,1,1200}
	tbl[5] = {3020007,4,1,1400}
	tbl[6] = {5610013,1,1,1}
	tbl[7] = {4221204,1,1,1}
	tbl[8] = {4999003,1,1,100}
	tbl[9] = {3030118,1,1,50}
	tbl[10] = {3020052,1,1,325}
	tbl[11] = {4630001,1,1,5}
	tbl[12] = {4630101,1,1,5}
	tbl[13] = {4630201,1,1,5}
	tbl[14] = {3020011,1,1,270}
	tbl[15] = {3020012,1,1,80}
	tbl[16] = {3020061,1,1,50}
	tbl[17] = {3070019,2,1,1000}
	tbl[18] = {3070003,1,1,200}
	tbl[19] = {3070002,20,1,900}
	tbl[20] = {3010020,1,1,1250}

	for i = 1,20 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)
end

function I4700049_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700049, 1, "I4700049_GiftBag")
aux.RegisterItemEvent(4700049, 0, "I4700049_CanUseGiftBag")


function I4700050_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {4820029,1,1,1800}
	tbl[2] = {4820029,20,1,100}
	tbl[3] = {4820029,100,1,25}
	tbl[4] = {3030119,1,1,200}
	tbl[5] = {3070019,3,1,1360}
	tbl[6] = {3070052,15,1,1290}
	tbl[7] = {4080097,1,1,1210}
	tbl[8] = {3070002,20,1,1200}
	tbl[9] = {5600008,1,1,500}
	tbl[10] = {4100615,1,1,10}
	tbl[11] = {4530109,1,1,700}
	tbl[12] = {5600006,1,1,1000}
	tbl[13] = {4820027,5,1,600}
	tbl[14] = {3020087,1,1,4}
	tbl[15] = {3020090,1,1,1}
	for i = 1,15 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)
end

function I4700050_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700050, 1, "I4700050_GiftBag")
aux.RegisterItemEvent(4700050, 0, "I4700050_CanUseGiftBag")

