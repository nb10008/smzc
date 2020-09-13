function I4700262_GiftBag(MapID, InstanceID, TypeID, TargetID) --女的
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700214, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700266, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700264, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700265, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700212, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700209, 1, -1, 8, 420)
end

function I4700262_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700262, 1, "I4700262_GiftBag")
aux.RegisterItemEvent(4700262, 0, "I4700262_CanUseGiftBag")



function I4700266_GiftBag(MapID, InstanceID, TypeID, TargetID) --聚宝盆一
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4700267, 1, -1, 8, 420)   ----聚宝盆二

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8013009, 1, 5, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8023009, 1, 5, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8033009, 1, 5, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8043009, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 16, -1, 8, 420)
end

function I4700266_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700266, 1, "I4700266_GiftBag")
aux.RegisterItemEvent(4700266, 0, "I4700266_CanUseGiftBag")

function I4700267_GiftBag(MapID, InstanceID, TypeID, TargetID) ----聚宝盆二
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700268, 1, -1, 8, 420)   ----聚宝盆三

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8050309, 1, 5, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8050409, 1, 5, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8051309, 1, 5, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8051409, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
end

function I4700267_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700267, 1, "I4700267_GiftBag")
aux.RegisterItemEvent(4700267, 0, "I4700267_CanUseGiftBag")

function I4700268_GiftBag(MapID, InstanceID, TypeID, TargetID) ----聚宝盆三
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700269, 1, -1, 8, 420)   ----聚宝盆四

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8060309, 1, 5, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8060409, 1, 5, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8061309, 1, 5, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8061409, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
end

function I4700268_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700268, 1, "I4700268_GiftBag")
aux.RegisterItemEvent(4700268, 0, "I4700268_CanUseGiftBag")

function I4700269_GiftBag(MapID, InstanceID, TypeID, TargetID) ----聚宝盆四
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700270, 1, -1, 8, 420)   ----聚宝盆五

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8070309, 1, 5, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8070409, 1, 5, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8071309, 1, 5, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8071409, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
end

function I4700269_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700269, 1, "I4700269_GiftBag")
aux.RegisterItemEvent(4700269, 0, "I4700269_CanUseGiftBag")

function I4700270_GiftBag(MapID, InstanceID, TypeID, TargetID) ----聚宝盆五
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700271, 1, -1, 8, 420)   ----聚宝盆六

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8080309, 1, 5, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8080409, 1, 5, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8081309, 1, 5, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8081409, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
end

function I4700270_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700270, 1, "I4700270_GiftBag")
aux.RegisterItemEvent(4700270, 0, "I4700270_CanUseGiftBag")



function I4700271_GiftBag(MapID, InstanceID, TypeID, TargetID) ----聚宝盆六
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700272, 1, -1, 8, 420)   ----聚宝盆七

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8090309, 1, 5, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8090409, 1, 5, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8091309, 1, 5, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8091409, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
end

function I4700271_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700271, 1, "I4700271_GiftBag")
aux.RegisterItemEvent(4700271, 0, "I4700271_CanUseGiftBag")

function I4700272_GiftBag(MapID, InstanceID, TypeID, TargetID) ----聚宝盆七
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700273, 1, -1, 8, 420)   ----聚宝盆八

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8100309, 1, 5, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8100409, 1, 5, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8100509, 1, 5, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8100609, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
end

function I4700272_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700272, 1, "I4700272_GiftBag")
aux.RegisterItemEvent(4700272, 0, "I4700272_CanUseGiftBag")

function I4700273_GiftBag(MapID, InstanceID, TypeID, TargetID) ----聚宝盆八
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700274, 1, -1, 8, 420)   ----聚宝盆九

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 or class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8110309, 1, 5, 8, 420)
	elseif class==4 or class==10 or class==11 or class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8111309, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
end

function I4700273_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700273, 1, "I4700273_GiftBag")
aux.RegisterItemEvent(4700273, 0, "I4700273_CanUseGiftBag")

function I4700274_GiftBag(MapID, InstanceID, TypeID, TargetID) ----聚宝盆九
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700275, 1, -1, 8, 420)   ----聚宝盆十

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 or class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8121309, 1, 5, 8, 420)
	elseif class==4 or class==10 or class==11 or class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8122309, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
end

function I4700274_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700274, 1, "I4700274_GiftBag")
aux.RegisterItemEvent(4700274, 0, "I4700274_CanUseGiftBag")

function I4700275_GiftBag(MapID, InstanceID, TypeID, TargetID) ----聚宝盆十
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 40, -1, 8, 420)
	if class==2 or class==6 or class==7 or class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8130309, 1, 5, 8, 420)
	elseif class==4 or class==10 or class==11 or class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8130409, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4090022, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090025, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009, 8, -1, 8, 420)
end

function I4700275_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700275, 1, "I4700275_GiftBag")
aux.RegisterItemEvent(4700275, 0, "I4700275_CanUseGiftBag")


function I4700264_GiftBag(MapID, InstanceID, TypeID, TargetID) ----金龙·初级回归奖励
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举

	role.AddRoleItem(MapID, InstanceID, TargetID,4221703,1,-1,8, 420)

	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4830026, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 4830027, 1, -1, 8, 420)
	end

	local class = role.GetRoleClass(TargetID)
	if class==2 or class==6 or class==7 or class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID,8160008,1,5,8, 420)
	elseif class==4 or class==10 or class==11 or class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID,8170008,1,5,8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID,4212007,1,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070014,1,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070006,1,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070003,2,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070004,2,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,20,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056,100,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030147,4,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030148,4,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020116,6,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020118,4,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3060005,50,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070051,10,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820087,15,-1,8, 420)
end

function I4700264_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 17) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700264, 1, "I4700264_GiftBag")
aux.RegisterItemEvent(4700264, 0, "I4700264_CanUseGiftBag")


function I4700265_GiftBag(MapID, InstanceID, TypeID, TargetID) ----金龙·高级回归奖励
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举


	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4830026, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 4830027, 1, -1, 8, 420)
	end


	--判断玩家职业
	local class = role.GetRoleClass(TargetID)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID,4650001,1,4,8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID,4650101,1,4,8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID,4650201,1,4,8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID,4650301,1,4,8, 420)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID,3020119,6,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020121,6,-1,8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,3020071,10,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4300008,1,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030143,4,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030144,4,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4400054,30,-1,8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,3040058,3,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3040023,3,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4090020,1,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4090019,1,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4090021,1,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820047,30,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820046,30,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820172,5,-1,8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4999004,10,-1,8, 420)
end

function I4700265_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 18) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700265, 1, "I4700265_GiftBag")
aux.RegisterItemEvent(4700265, 0, "I4700265_CanUseGiftBag")








