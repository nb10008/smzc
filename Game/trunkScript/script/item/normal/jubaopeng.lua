



--4700238	聚宝盆
function I4700238_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700239, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8017008, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8027008, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8047008, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8037008, 1, 4, 8, 420)
    end
end

function I4700238_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700238, 1, "I4700238_GiftBag")
aux.RegisterItemEvent(4700238, 0, "I4700238_CanUseGiftBag")




--4700239	聚宝盆
function I4700239_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700240, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 4, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050808, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050812, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051808, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051812, 1, 4, 8, 420)
    end
end

function I4700239_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700239, 1, "I4700239_GiftBag")
aux.RegisterItemEvent(4700239, 0, "I4700239_CanUseGiftBag")




--4700240	聚宝盆
function I4700240_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700241, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 4, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060808, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060812, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061808, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061812, 1, 4, 8, 420)
    end
end

function I4700240_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700240, 1, "I4700240_GiftBag")
aux.RegisterItemEvent(4700240, 0, "I4700240_CanUseGiftBag")





--4700241	聚宝盆
function I4700241_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700242, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 4, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070808, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070812, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071808, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071812, 1, 4, 8, 420)
    end
end

function I4700241_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700241, 1, "I4700241_GiftBag")
aux.RegisterItemEvent(4700241, 0, "I4700241_CanUseGiftBag")




--4700242	聚宝盆
function I4700242_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700243, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 4, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080808, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080812, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081808, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081812, 1, 4, 8, 420)
    end
end

function I4700242_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700242, 1, "I4700242_GiftBag")
aux.RegisterItemEvent(4700242, 0, "I4700242_CanUseGiftBag")




--4700243	聚宝盆
function I4700243_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700244, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 4, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090808, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090812, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091808, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091812, 1, 4, 8, 420)
    end
end

function I4700243_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700243, 1, "I4700243_GiftBag")
aux.RegisterItemEvent(4700243, 0, "I4700243_CanUseGiftBag")





--4700244	聚宝盆
function I4700244_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700245, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 4, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101208, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101212, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101216, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101220, 1, 4, 8, 420)
    end
end

function I4700244_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700244, 1, "I4700244_GiftBag")
aux.RegisterItemEvent(4700244, 0, "I4700244_CanUseGiftBag")





--4700245	聚宝盆
function I4700245_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700246, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 4, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8110808, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8110808, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8111808, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8111808, 1, 4, 8, 420)
    end
end

function I4700245_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700245, 1, "I4700245_GiftBag")
aux.RegisterItemEvent(4700245, 0, "I4700245_CanUseGiftBag")





--4700246	聚宝盆
function I4700246_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700247, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 4, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8121808, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8121808, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8122808, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8122808, 1, 4, 8, 420)
    end
end

function I4700246_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700246, 1, "I4700246_GiftBag")
aux.RegisterItemEvent(4700246, 0, "I4700246_CanUseGiftBag")





--4700247	聚宝盆
function I4700247_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 4, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130808, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130808, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130812, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130812, 1, 4, 8, 420)
    end
end

function I4700247_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700247, 1, "I4700247_GiftBag")
aux.RegisterItemEvent(4700247, 0, "I4700247_CanUseGiftBag")
