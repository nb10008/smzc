function I4700048_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {5600008,1,1,140}
	tbl[2] = {4820027,3,1,150}
	tbl[3] = {4090022,1,1,133}
	tbl[4] = {4400031,1,1,60}
	tbl[5] = {4500002,8,1,160}
	tbl[6] = {4820027,10,1,25}
	tbl[7] = {3070052,10,1,160}
	tbl[8] = {5610013,1,1,1}
	tbl[9] = {4221204,1,1,1}
	tbl[10] = {3020007,2,1,170}

	for i = 1,10 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700048_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700048, 1, "I4700048_GiftBag")
aux.RegisterItemEvent(4700048, 0, "I4700048_CanUseGiftBag")

