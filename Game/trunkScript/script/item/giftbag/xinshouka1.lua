function I4530232_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070057,2,-1}
	tbl[2] = {3070006,1,-1}
	tbl[3] = {4510022,1,-1}
	tbl[4] = {4510023,1,-1}
	tbl[5] = {4300007,1,-1}
	tbl[6] = {3010054,1,-1}
	tbl[7] = {3010055,1,-1}
	tbl[8] = {4530233,1,-1}

	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530232_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530232, 1, "I4530232_GiftBag")
aux.RegisterItemEvent(4530232, 0, "I4530232_CanUseGiftBag")



function I4530233_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070051,15,-1}
	tbl[2] = {3070001,15,-1}
	tbl[3] = {3070011,1,-1}
	tbl[4] = {3070018,5,-1}
	tbl[5] = {4820081,20,-1}
	tbl[6] = {3010035,5,-1}
	tbl[7] = {3070028,5,-1}
	tbl[8] = {3070056,30,-1}
	tbl[9] = {3050007,20,-1}
	tbl[10] = {4530234,1,-1}


	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530233_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530233, 1, "I4530233_GiftBag")
aux.RegisterItemEvent(4530233, 0, "I4530233_CanUseGiftBag")






function I4530234_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070011,1,-1}
	tbl[2] = {3070018,5,-1}
	tbl[3] = {4820081,30,-1}
	tbl[4] = {3010035,5,-1}
	tbl[5] = {3070028,5,-1}
	tbl[6] = {3050007,20,-1}
	tbl[7] = {3090017,1,-1}
	tbl[8] = {3020049,5,-1}
	tbl[9] = {3020009,10,-1}
	tbl[10] = {3020057,20,-1}
	tbl[11] = {3070056,30,-1}
	tbl[12] = {3020005,30,-1}
	tbl[13] = {4530235,1,-1}



	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530234_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 13) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530234, 1, "I4530234_GiftBag")
aux.RegisterItemEvent(4530234, 0, "I4530234_CanUseGiftBag")



function I4530235_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070011,1,-1}
	tbl[2] = {3070018,5,-1}
	tbl[3] = {4820081,30,-1}
	tbl[4] = {3010035,5,-1}
	tbl[5] = {3070028,5,-1}
	tbl[6] = {3050007,20,-1}
	tbl[7] = {3020115,2,-1}
	tbl[8] = {3070057,2,-1}
	tbl[9] = {4510022,1,-1}
	tbl[10] = {4510023,1,-1}
	tbl[11] = {3020011,6,-1}
	tbl[12] = {3020057,10,-1}
	tbl[13] = {3070051,15,-1}
	tbl[14] = {3070056,30,-1}
	tbl[15] = {3070069,5,-1}
	tbl[16] = {4820257,1,-1}
	tbl[17] = {4820249,5,-1}
	tbl[18] = {3060005,5,-1}
	tbl[19] = {4530107,5,-1}




	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530235_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 19) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530235, 1, "I4530235_GiftBag")
aux.RegisterItemEvent(4530235, 0, "I4530235_CanUseGiftBag")



function I4530236_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070057,2,-1}
	tbl[2] = {3070006,1,-1}
	tbl[3] = {4510022,1,-1}
	tbl[4] = {4510023,1,-1}
	tbl[5] = {4300006,1,-1}
	tbl[6] = {3010054,1,-1}
	tbl[7] = {3010055,1,-1}
	tbl[8] = {4530237,1,-1}
	tbl[9] = {3010054,1,-1}
	tbl[10] = {3010055,1,-1}

	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530236_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530236, 1, "I4530236_GiftBag")
aux.RegisterItemEvent(4530236, 0, "I4530236_CanUseGiftBag")





function I4530237_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070051,20,-1}
	tbl[2] = {3070001,20,-1}
	tbl[3] = {3070011,1,-1}
	tbl[4] = {3070018,5,-1}
	tbl[5] = {4820081,45,-1}
	tbl[6] = {3010035,5,-1}
	tbl[7] = {3070028,5,-1}
	tbl[8] = {3050007,20,-1}
	tbl[9] = {4510022,1,-1}
	tbl[10] = {4510023,1,-1}
	tbl[11] = {3020011,6,-1}
	tbl[12] = {3020057,15,-1}
	tbl[13] = {3070068,15,-1}
	tbl[14] = {3070056,35,-1}
	tbl[15] = {3070011,1,-1}


	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530237_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 15) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530237, 1, "I4530237_GiftBag")
aux.RegisterItemEvent(4530237, 0, "I4530237_CanUseGiftBag")


function I4530238_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070057,2,-1}
	tbl[2] = {3070006,1,-1}
	tbl[3] = {4510022,1,-1}
	tbl[4] = {4510023,1,-1}
	tbl[5] = {4300006,1,-1}
	tbl[6] = {3010054,1,-1}
	tbl[7] = {3010055,1,-1}
	tbl[8] = {4530239,1,-1}
	tbl[9] = {3010054,1,-1}
	tbl[10] = {3010055,1,-1}


	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530238_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530238, 1, "I4530238_GiftBag")
aux.RegisterItemEvent(4530238, 0, "I4530238_CanUseGiftBag")






function I4530239_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070051,20,-1}
	tbl[2] = {3070001,20,-1}
	tbl[3] = {3070011,1,-1}
	tbl[4] = {3070018,5,-1}
	tbl[5] = {4820081,45,-1}
	tbl[6] = {3010035,5,-1}
	tbl[7] = {3070028,15,-1}
	tbl[8] = {3050007,20,-1}
	tbl[9] = {4510022,1,-1}
	tbl[10] = {4510023,1,-1}
	tbl[11] = {3020011,4,-1}
	tbl[12] = {3020057,8,-1}
	tbl[13] = {3070068,8,-1}
	tbl[14] = {3070056,45,-1}
	tbl[15] = {3070011,1,-1}
	tbl[16] = {3070011,1,-1}



	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530239_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 16) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530239, 1, "I4530239_GiftBag")
aux.RegisterItemEvent(4530239, 0, "I4530239_CanUseGiftBag")





function I4530240_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070057,2,-1}
	tbl[2] = {3070006,1,-1}
	tbl[3] = {4510022,1,-1}
	tbl[4] = {4510023,1,-1}
	tbl[5] = {4300006,1,-1}
	tbl[6] = {3010054,1,-1}
	tbl[7] = {3010055,1,-1}
	tbl[8] = {4530241,1,-1}
	tbl[9] = {3010054,1,-1}
	tbl[10] = {3010055,1,-1}



	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530240_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530240, 1, "I4530240_GiftBag")
aux.RegisterItemEvent(4530240, 0, "I4530240_CanUseGiftBag")



function I4530241_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3050007,20,-1}
	tbl[2] = {3070051,15,-1}
	tbl[3] = {3070001,15,-1}
	tbl[4] = {3070011,1,-1}
	tbl[5] = {3070018,3,-1}
	tbl[6] = {4820081,30,-1}
	tbl[7] = {3010035,3,-1}
	tbl[8] = {3070028,5,-1}
	tbl[9] = {3020057,8,-1}
	tbl[10] = {3070068,8,-1}
	tbl[11] = {3070056,30,-1}
	tbl[12] = {3070011,1,-1}





	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530241_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530241, 1, "I4530241_GiftBag")
aux.RegisterItemEvent(4530241, 0, "I4530241_CanUseGiftBag")




function I4530198_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070011,1,-1}
	tbl[2] = {3070028,5,-1}
	tbl[3] = {3070006,1,-1}
	tbl[4] = {4221801,1,-1}
	tbl[5] = {4211201,1,-1}
	tbl[6] = {3070056,50,-1}
	tbl[7] = {3070068,15,-1}
	tbl[8] = {3020057,15,-1}
	tbl[9] = {3070051,15,-1}
	tbl[10] = {3070001,15,-1}
	tbl[11] = {3070057,2,-1}



	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530198_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530198, 1, "I4530198_GiftBag")
aux.RegisterItemEvent(4530198, 0, "I4530198_CanUseGiftBag")
