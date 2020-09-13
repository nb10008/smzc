--11级礼包
function I4800199_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080071,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070064,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070065,1, -1, 8, 420)
end

function I4800199_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800199, 1, "I4800199_GiftBag")
aux.RegisterItemEvent(4800199, 0, "I4800199_CanUseGiftBag")



--15级礼包
function I4800200_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060001,50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070022,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020048,10, -1, 8, 420)
end

function I4800200_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800200, 1, "I4800200_GiftBag")
aux.RegisterItemEvent(4800200, 0, "I4800200_CanUseGiftBag")



--20级礼包
function I4800201_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050005,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050001,50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050031,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030126,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057,1, -1, 8, 420)
end

function I4800201_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800201, 1, "I4800201_GiftBag")
aux.RegisterItemEvent(4800201, 0, "I4800201_CanUseGiftBag")



--25级礼包
function I4800202_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050006,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020004,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820250,10, -1, 8, 420)
end

function I4800202_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800202, 1, "I4800202_GiftBag")
aux.RegisterItemEvent(4800202, 0, "I4800202_CanUseGiftBag")



--30级礼包
function I4800203_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010037,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060002,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116,3, -1, 8, 420)
end

function I4800203_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800203, 1, "I4800203_GiftBag")
aux.RegisterItemEvent(4800203, 0, "I4800203_CanUseGiftBag")



--35级礼包
function I4800204_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051607,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051608,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051609,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020080,5, -1, 8, 420)
end

function I4800204_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800204, 1, "I4800204_GiftBag")
aux.RegisterItemEvent(4800204, 0, "I4800204_CanUseGiftBag")



--40级礼包
function I4800205_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060003,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050032,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030104,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030105,2, -1, 8, 420)
end

function I4800205_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800205, 1, "I4800205_GiftBag")
aux.RegisterItemEvent(4800205, 0, "I4800205_CanUseGiftBag")




--45级礼包
function I4800206_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030130,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030133,2, -1, 8, 420)
end

function I4800206_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800206, 1, "I4800206_GiftBag")
aux.RegisterItemEvent(4800206, 0, "I4800206_CanUseGiftBag")




--50级礼包
function I4800207_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050016,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060004,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030130,4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030133,4, -1, 8, 420)
end

function I4800207_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800207, 1, "I4800207_GiftBag")
aux.RegisterItemEvent(4800207, 0, "I4800207_CanUseGiftBag")




--55级礼包
function I4800208_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050062,4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050063,4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050064,4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030088,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020049,15, -1, 8, 420)
end

function I4800208_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800208, 1, "I4800208_GiftBag")
aux.RegisterItemEvent(4800208, 0, "I4800208_CanUseGiftBag")




--60级礼包
function I4800209_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030135,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020081,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100005,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100015,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100025,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100035,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100045,1, -1, 8, 420)
end

function I4800209_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800209, 1, "I4800209_GiftBag")
aux.RegisterItemEvent(4800209, 0, "I4800209_CanUseGiftBag")




--65级礼包
function I4800210_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020029,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820080,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040003,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040008,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040013,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040018,1, -1, 8, 420)
end

function I4800210_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800210, 1, "I4800210_GiftBag")
aux.RegisterItemEvent(4800210, 0, "I4800210_CanUseGiftBag")




--70级礼包
function I4800211_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070022,5, -1, 8, 420)
end

function I4800211_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800211, 1, "I4800211_GiftBag")
aux.RegisterItemEvent(4800211, 0, "I4800211_CanUseGiftBag")




--75级礼包
function I4800212_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020030,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820043,2, -1, 8, 420)
end

function I4800212_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800212, 1, "I4800212_GiftBag")
aux.RegisterItemEvent(4800212, 0, "I4800212_CanUseGiftBag")




--80级礼包
function I4800213_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820035,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800041,2, -1, 8, 420)
end

function I4800213_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800213, 1, "I4800213_GiftBag")
aux.RegisterItemEvent(4800213, 0, "I4800213_CanUseGiftBag")
