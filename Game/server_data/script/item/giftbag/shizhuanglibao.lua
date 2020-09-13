--克洛华兹套装
function I4700069_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8409001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8409002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8409004, 1, k3, 8, 420)
end

function I4700069_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700069, 1, "I4700069_GiftBag")
aux.RegisterItemEvent(4700069, 0, "I4700069_CanUseGiftBag")

--休闲套装
function I4700070_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8413001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8413002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8413004, 1, k3, 8, 420)
end

function I4700070_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700070, 1, "I4700070_GiftBag")
aux.RegisterItemEvent(4700070, 0, "I4700070_CanUseGiftBag")

--时尚套装
function I4700066_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8401001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8401002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8401003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8401004, 1, k4, 8, 420)
end

function I4700066_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700066, 1, "I4700066_GiftBag")
aux.RegisterItemEvent(4700066, 0, "I4700066_CanUseGiftBag")

--火红热情套装
function I4700067_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8403001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8403002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8403003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8403004, 1, k4, 8, 420)
end

function I4700067_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700067, 1, "I4700067_GiftBag")
aux.RegisterItemEvent(4700067, 0, "I4700067_CanUseGiftBag")

--甜美英伦风套装
function I4700068_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8406001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8406002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8406003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8406004, 1, k4, 8, 420)
end

function I4700068_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700068, 1, "I4700068_GiftBag")
aux.RegisterItemEvent(4700068, 0, "I4700068_CanUseGiftBag")


--简约套装
function I4700112_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8402001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8402002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8402004, 1, k3, 8, 420)
end

function I4700112_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700112, 1, "I4700112_GiftBag")
aux.RegisterItemEvent(4700112, 0, "I4700112_CanUseGiftBag")

--炫紫套装
function I4700113_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8404001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8404002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8404003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8404004, 1, k4, 8, 420)
end

function I4700113_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700113, 1, "I4700113_GiftBag")
aux.RegisterItemEvent(4700113, 0, "I4700113_CanUseGiftBag")

--粉色柔情套装
function I4700114_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8405001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8405002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8405003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8405004, 1, k4, 8, 420)
end

function I4700114_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700114, 1, "I4700114_GiftBag")
aux.RegisterItemEvent(4700114, 0, "I4700114_CanUseGiftBag")

--性感诱惑套装
function I4700115_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8407001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8407002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8407004, 1, k3, 8, 420)
end

function I4700115_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700115, 1, "I4700115_GiftBag")
aux.RegisterItemEvent(4700115, 0, "I4700115_CanUseGiftBag")

--黑风物语套装
function I4700116_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8408001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8408002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8408004, 1, k3, 8, 420)
end

function I4700116_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700116, 1, "I4700116_GiftBag")
aux.RegisterItemEvent(4700116, 0, "I4700116_CanUseGiftBag")

--启示套装
function I4700117_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8410001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8410002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8410004, 1, k3, 8, 420)
end

function I4700117_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700117, 1, "I4700117_GiftBag")
aux.RegisterItemEvent(4700117, 0, "I4700117_CanUseGiftBag")

--帅气英伦风套装
function I4700118_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8411002, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8411004, 1, k2, 8, 420)
end

function I4700118_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700118, 1, "I4700118_GiftBag")
aux.RegisterItemEvent(4700118, 0, "I4700118_CanUseGiftBag")

--战争绝响套装
function I4700119_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8412001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8412002, 1, k2, 8, 420)
end

function I4700119_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700119, 1, "I4700119_GiftBag")
aux.RegisterItemEvent(4700119, 0, "I4700119_CanUseGiftBag")

--闪耀奢华套装
function I4700120_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8414001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8414002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8414003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8414004, 1, k4, 8, 420)
end

function I4700120_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700120, 1, "I4700120_GiftBag")
aux.RegisterItemEvent(4700120, 0, "I4700120_CanUseGiftBag")

--紫罗兰风情套装
function I4700121_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8415001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8415002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8415003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8415004, 1, k4, 8, 420)
end

function I4700121_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700121, 1, "I4700121_GiftBag")
aux.RegisterItemEvent(4700121, 0, "I4700121_CanUseGiftBag")

--高贵之花套装
function I4700122_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8416001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8416002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8416003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8416004, 1, k4, 8, 420)
end

function I4700122_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700122, 1, "I4700122_GiftBag")
aux.RegisterItemEvent(4700122, 0, "I4700122_CanUseGiftBag")

--咖蓝魅影套装
function I4700123_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8417001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8417002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8417003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8417004, 1, k4, 8, 420)
end

function I4700123_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700123, 1, "I4700123_GiftBag")
aux.RegisterItemEvent(4700123, 0, "I4700123_CanUseGiftBag")

--优雅淑女套装
function I4700124_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8418001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8418002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8418003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8418004, 1, k4, 8, 420)
end

function I4700124_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700124, 1, "I4700124_GiftBag")
aux.RegisterItemEvent(4700124, 0, "I4700124_CanUseGiftBag")

--黑莲舞者套装
function I4700125_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8419001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8419002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8419003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8419004, 1, k4, 8, 420)
end

function I4700125_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700125, 1, "I4700125_GiftBag")
aux.RegisterItemEvent(4700125, 0, "I4700125_CanUseGiftBag")

--中国队长套装
function I4700126_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8420001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8420002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8420003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8420004, 1, k4, 8, 420)
end

function I4700126_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700126, 1, "I4700126_GiftBag")
aux.RegisterItemEvent(4700126, 0, "I4700126_CanUseGiftBag")

--火焰印记套装
function I4700127_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8421002, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8421003, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8421004, 1, k3, 8, 420)
end

function I4700127_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700127, 1, "I4700127_GiftBag")
aux.RegisterItemEvent(4700127, 0, "I4700127_CanUseGiftBag")

--自由领域套装
function I4700128_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8422001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8422002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8422004, 1, k3, 8, 420)
end

function I4700128_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700128, 1, "I4700128_GiftBag")
aux.RegisterItemEvent(4700128, 0, "I4700128_CanUseGiftBag")

--圣洁信仰套装
function I4700129_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8423001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8423002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8423003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8423004, 1, k4, 8, 420)
end

function I4700129_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700129, 1, "I4700129_GiftBag")
aux.RegisterItemEvent(4700129, 0, "I4700129_CanUseGiftBag")

--黑骑士套装
function I4700130_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8424002, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8424004, 1, k2, 8, 420)
end

function I4700130_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700130, 1, "I4700130_GiftBag")
aux.RegisterItemEvent(4700130, 0, "I4700130_CanUseGiftBag")

--吸血鬼优雅套装
function I4700131_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8425001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8425002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8425003, 1, k3, 8, 420)
end

function I4700131_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700131, 1, "I4700131_GiftBag")
aux.RegisterItemEvent(4700131, 0, "I4700131_CanUseGiftBag")

--阳光休闲套装
function I4700132_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8426002, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8426003, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8426004, 1, k3, 8, 420)
end

function I4700132_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700132, 1, "I4700132_GiftBag")
aux.RegisterItemEvent(4700132, 0, "I4700132_CanUseGiftBag")

--幽暗套装
function I4700133_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8427002, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8427004, 1, k2, 8, 420)
end

function I4700133_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700133, 1, "I4700133_GiftBag")
aux.RegisterItemEvent(4700133, 0, "I4700133_CanUseGiftBag")

--五道杠套装
function I4700134_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8428001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8428002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8428003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8428004, 1, k4, 8, 420)
end

function I4700134_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700134, 1, "I4700134_GiftBag")
aux.RegisterItemEvent(4700134, 0, "I4700134_CanUseGiftBag")

--纯净悲鸣套装
function I4700135_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8429002, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8429003, 1, k2, 8, 420)
end

function I4700135_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700135, 1, "I4700135_GiftBag")
aux.RegisterItemEvent(4700135, 0, "I4700135_CanUseGiftBag")

--宁静天堂套装
function I4700136_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8430001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8430002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8430004, 1, k3, 8, 420)
end

function I4700136_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700136, 1, "I4700136_GiftBag")
aux.RegisterItemEvent(4700136, 0, "I4700136_CanUseGiftBag")

--干练套装
function I4700137_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8431001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8431002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8431004, 1, k3, 8, 420)
end

function I4700137_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700137, 1, "I4700137_GiftBag")
aux.RegisterItemEvent(4700137, 0, "I4700137_CanUseGiftBag")



--复古潮流男
function I4700153_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300004, 1, k4, 8, 420)
end

function I4700153_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700153, 1, "I4700153_GiftBag")
aux.RegisterItemEvent(4700153, 0, "I4700153_CanUseGiftBag")



--复古潮流女
function I4700154_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300011, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300012, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300013, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300014, 1, k4, 8, 420)
end

function I4700154_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700154, 1, "I4700154_GiftBag")
aux.RegisterItemEvent(4700154, 0, "I4700154_CanUseGiftBag")



--梦幻自由男
function I4700155_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300101, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300102, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300103, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300104, 1, k4, 8, 420)
end

function I4700155_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700155, 1, "I4700155_GiftBag")
aux.RegisterItemEvent(4700155, 0, "I4700155_CanUseGiftBag")



--梦幻自由女
function I4700156_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300111, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300112, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300113, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300114, 1, k4, 8, 420)
end

function I4700156_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700156, 1, "I4700156_GiftBag")
aux.RegisterItemEvent(4700156, 0, "I4700156_CanUseGiftBag")



--炫酷牛仔礼包[男]
function I4700157_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300201, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300202, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300203, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300204, 1, k4, 8, 420)
end

function I4700157_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700157, 1, "I4700157_GiftBag")
aux.RegisterItemEvent(4700157, 0, "I4700157_CanUseGiftBag")



--炫酷牛仔礼包[女]
function I4700158_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300211, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300212, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300213, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300214, 1, k4, 8, 420)
end

function I4700158_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700158, 1, "I4700158_GiftBag")
aux.RegisterItemEvent(4700158, 0, "I4700158_CanUseGiftBag")



--火红拉丁礼包[男]
function I4700159_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300303, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300304, 1, k4, 8, 420)
end

function I4700159_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700159, 1, "I4700159_GiftBag")
aux.RegisterItemEvent(4700159, 0, "I4700159_CanUseGiftBag")



--火红拉丁礼包[女]
function I4700160_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300311, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300312, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300313, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8300314, 1, k4, 8, 420)
end

function I4700160_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700160, 1, "I4700160_GiftBag")
aux.RegisterItemEvent(4700160, 0, "I4700160_CanUseGiftBag")



--时尚蕾丝礼包
function I4700161_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8340301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8340302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8340304, 1, k4, 8, 420)
end

function I4700161_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700161, 1, "I4700161_GiftBag")
aux.RegisterItemEvent(4700161, 0, "I4700161_CanUseGiftBag")



--雪纺礼包
function I4700162_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8341301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8341302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8341304, 1, k4, 8, 420)
end

function I4700162_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700162, 1, "I4700162_GiftBag")
aux.RegisterItemEvent(4700162, 0, "I4700162_CanUseGiftBag")



--纯白露肩礼包
function I4700163_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8343303, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8343302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8343304, 1, k4, 8, 420)
end

function I4700163_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700163, 1, "I4700163_GiftBag")
aux.RegisterItemEvent(4700163, 0, "I4700163_CanUseGiftBag")



--性感蕾丝礼包
function I4700164_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8344301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8344302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8344303, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8344304, 1, k4, 8, 420)
end

function I4700164_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700164, 1, "I4700164_GiftBag")
aux.RegisterItemEvent(4700164, 0, "I4700164_CanUseGiftBag")



--泡泡礼包
function I4700165_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8342301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8342302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8342304, 1, k4, 8, 420)
end

function I4700165_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700165, 1, "I4700165_GiftBag")
aux.RegisterItemEvent(4700165, 0, "I4700165_CanUseGiftBag")



--小不乖礼包
function I4700166_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8345301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8345302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8345303, 1, k4, 8, 420)
end

function I4700166_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700166, 1, "I4700166_GiftBag")
aux.RegisterItemEvent(4700166, 0, "I4700166_CanUseGiftBag")



--贵族晚礼礼包
function I4700167_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8346301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8346302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8346303, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8346304, 1, k4, 8, 420)
end

function I4700167_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700167, 1, "I4700167_GiftBag")
aux.RegisterItemEvent(4700167, 0, "I4700167_CanUseGiftBag")



--殿堂礼包
function I4700168_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8347301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8347302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8347304, 1, k4, 8, 420)
end

function I4700168_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700168, 1, "I4700168_GiftBag")
aux.RegisterItemEvent(4700168, 0, "I4700168_CanUseGiftBag")



--黑精灵礼包
function I4700169_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8348301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8348302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8348304, 1, k4, 8, 420)
end

function I4700169_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700169, 1, "I4700169_GiftBag")
aux.RegisterItemEvent(4700169, 0, "I4700169_CanUseGiftBag")



--修闲礼包
function I4700170_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8349302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8349304, 1, k4, 8, 420)
end

function I4700170_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700170, 1, "I4700170_GiftBag")
aux.RegisterItemEvent(4700170, 0, "I4700170_CanUseGiftBag")



--黑色魅力礼包
function I4700171_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8350301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8350302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8350303, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8350304, 1, k4, 8, 420)
end

function I4700171_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700171, 1, "I4700171_GiftBag")
aux.RegisterItemEvent(4700171, 0, "I4700171_CanUseGiftBag")



--黑色诱惑礼包
function I4700172_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8351301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8351302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8351303, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8351304, 1, k4, 8, 420)
end

function I4700172_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700172, 1, "I4700172_GiftBag")
aux.RegisterItemEvent(4700172, 0, "I4700172_CanUseGiftBag")



--春节礼包[男]
function I4700173_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8363001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8363002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8363004, 1, k4, 8, 420)
end

function I4700173_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700173, 1, "I4700173_GiftBag")
aux.RegisterItemEvent(4700173, 0, "I4700173_CanUseGiftBag")



--春节礼包[女]
function I4700174_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8363011, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8363012, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8363013, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8363014, 1, k4, 8, 420)
end

function I4700174_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700174, 1, "I4700174_GiftBag")
aux.RegisterItemEvent(4700174, 0, "I4700174_CanUseGiftBag")



--浓情蜜意礼包[男]
function I4700175_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8383001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8383002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8383004, 1, k4, 8, 420)
end

function I4700175_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700175, 1, "I4700175_GiftBag")
aux.RegisterItemEvent(4700175, 0, "I4700175_CanUseGiftBag")



--浓情蜜意礼包[女]
function I4700176_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8383011, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8383012, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8383014, 1, k4, 8, 420)
end

function I4700176_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700176, 1, "I4700176_GiftBag")
aux.RegisterItemEvent(4700176, 0, "I4700176_CanUseGiftBag")



--浪漫之约礼包[女]
function I4700177_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8393011, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8393012, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8393013, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8393014, 1, k4, 8, 420)
end

function I4700177_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700177, 1, "I4700177_GiftBag")
aux.RegisterItemEvent(4700177, 0, "I4700177_CanUseGiftBag")



--粉红兔女郎礼包
function I4700178_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8373001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8373002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8373003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8373004, 1, k4, 8, 420)
end

function I4700178_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700178, 1, "I4700178_GiftBag")
aux.RegisterItemEvent(4700178, 0, "I4700178_CanUseGiftBag")



--雪白霜华礼包
function I4700179_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8394001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8394002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8394003, 1, k3, 8, 420)
end

function I4700179_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700179, 1, "I4700179_GiftBag")
aux.RegisterItemEvent(4700179, 0, "I4700179_CanUseGiftBag")



--无瑕公主礼包
function I4700180_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8395001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8395002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8395003, 1, k3, 8, 420)
end

function I4700180_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700180, 1, "I4700180_GiftBag")
aux.RegisterItemEvent(4700180, 0, "I4700180_CanUseGiftBag")



--魅影迷踪礼包
function I4700181_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8396001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8396002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8396003, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8396004, 1, k3, 8, 420)
end

function I4700181_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700181, 1, "I4700181_GiftBag")
aux.RegisterItemEvent(4700181, 0, "I4700181_CanUseGiftBag")



--玲珑之花礼包
function I4700182_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8397001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8397002, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8397003, 1, k3, 8, 420)
end

function I4700182_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700182, 1, "I4700182_GiftBag")
aux.RegisterItemEvent(4700182, 0, "I4700182_CanUseGiftBag")



--绚烂七花礼包
function I4700183_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8398001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8398002, 1, k2, 8, 420)
end

function I4700183_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700183, 1, "I4700183_GiftBag")
aux.RegisterItemEvent(4700183, 0, "I4700183_CanUseGiftBag")



--倾城贵人礼包
function I4700184_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8399001, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8399003, 1, k2, 8, 420)
end

function I4700184_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700184, 1, "I4700184_GiftBag")
aux.RegisterItemEvent(4700184, 0, "I4700184_CanUseGiftBag")


--中式唐装礼包[男]
function I4700218_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440301, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440302, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440303, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440304, 1, k4, 8, 420)
end

function I4700218_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700218, 1, "I4700218_GiftBag")
aux.RegisterItemEvent(4700218, 0, "I4700218_CanUseGiftBag")



--中式唐装礼包[女]
function I4700219_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440311, 1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440312, 1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440313, 1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8440314, 1, k4, 8, 420)
end

function I4700219_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700219, 1, "I4700219_GiftBag")
aux.RegisterItemEvent(4700219, 0, "I4700219_CanUseGiftBag")



--2011圣诞时装礼包[男]
function I4700248_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举

	role.AddRoleItem(MapID, InstanceID, TargetID, 8450301, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8450302, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8450303, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8450304, 1, 6, 8, 420)
end

function I4700248_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700248, 1, "I4700248_GiftBag")
aux.RegisterItemEvent(4700248, 0, "I4700248_CanUseGiftBag")





--2011圣诞时装礼包[女]
function I4700249_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举

	role.AddRoleItem(MapID, InstanceID, TargetID, 8450311, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8450312, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8450313, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8450314, 1, 6, 8, 420)
end

function I4700249_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700249, 1, "I4700249_GiftBag")
aux.RegisterItemEvent(4700249, 0, "I4700249_CanUseGiftBag")
