function I4700234_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {4500002,3,-1,600}
	tbl[2] = {3070052,3,-1,600}
	tbl[3] = {4800131,1,-1,600}
	tbl[4] = {4090010,1,-1,650}
	tbl[5] = {4800039,1,-1,300}
	tbl[6] = {4820024,8,-1,700}
	tbl[7] = {4820028,8,-1,700}
	tbl[8] = {4800041,1,-1,150}
	tbl[9] = {4530108,3,-1,440}
	tbl[10] = {4530109,3,-1,440}
	tbl[11] = {3060058,1,-1,5}
	tbl[12] = {4090022,2,-1,700}
	tbl[13] = {4999003,1,-1,400}
	tbl[14] = {4820048,20,-1,950}
	tbl[15] = {5600004,1,-1,600}
	tbl[16] = {4820047,1,-1,600}
	tbl[17] = {4820046,1,-1,600}
	tbl[18] = {3020027,1,-1,300}
	tbl[19] = {3020029,1,-1,15}
	tbl[20] = {4090011,1,-1,650}



	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 35)
end

function I4700234_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700234, 1, "I4700234_GiftBag")
aux.RegisterItemEvent(4700234, 0, "I4700234_CanUseGiftBag")

