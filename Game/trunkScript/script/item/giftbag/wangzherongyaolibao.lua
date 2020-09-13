function I4530194_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {4820081,20,-1}
	tbl[2] = {4800039,1,-1}
	tbl[3] = {4800041,1,-1}
	tbl[4] = {3030128,4,-1}
	tbl[5] = {4090022,10,-1}
	tbl[6] = {4090009,2,-1}
	tbl[7] = {4090025,1,-1}
	tbl[8] = {4700209,1,-1}
	tbl[9] = {4530195,1,-1}

	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530194_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530194, 1, "I4530194_GiftBag")
aux.RegisterItemEvent(4530194, 0, "I4530194_CanUseGiftBag")



function I4530195_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {4820081,30,-1}
	tbl[2] = {4800039,2,-1}
	tbl[3] = {4800041,2,-1}
	tbl[4] = {3030131,4,-1}
	tbl[5] = {4090022,20,-1}
	tbl[6] = {4090009,3,-1}
	tbl[7] = {4090025,2,-1}
	tbl[8] = {4530196,1,-1}

	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530195_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530195, 1, "I4530195_GiftBag")
aux.RegisterItemEvent(4530195, 0, "I4530195_CanUseGiftBag")



function I4530196_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {4820081,40,-1}
	tbl[2] = {4800039,3,-1}
	tbl[3] = {4800041,3,-1}
	tbl[4] = {3030134,4,-1}
	tbl[5] = {4090022,30,-1}
	tbl[6] = {4090009,4,-1}
	tbl[7] = {4090025,3,-1}
	tbl[8] = {4700212,1,-1}


	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530196_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530196, 1, "I4530196_GiftBag")
aux.RegisterItemEvent(4530196, 0, "I4530196_CanUseGiftBag")
