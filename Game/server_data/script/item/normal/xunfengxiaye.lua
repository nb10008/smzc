function I4700047_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {4530100,1,1,615}
	tbl[2] = {4530103,1,1,600}
	tbl[3] = {4080097,1,1,1100}
	tbl[4] = {4820024,1,1,900}
	tbl[5] = {4530108,5,1,500}
	tbl[6] = {4820024,20,1,100}
	tbl[7] = {3030119,1,1,40}
	tbl[8] = {3020087,1,1,10}
	tbl[9] = {3020090,1,1,5}
	tbl[10] = {5600006,1,1,900}
	tbl[11] = {3070019,3,1,1020}
	tbl[12] = {5600008,1,1,780}
	tbl[13] = {4090010,5,1,1200}
	tbl[14] = {4500002,10,1,1050}
	tbl[15] = {4090011,5,1,1180}


	for i = 1,15 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700047_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700047, 1, "I4700047_GiftBag")
aux.RegisterItemEvent(4700047, 0, "I4700047_CanUseGiftBag")
