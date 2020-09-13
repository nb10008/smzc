--4700197	王者回归礼包
function I4700197_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700209, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700210, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700211, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700212, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700213, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700214, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700199, 1, -1, 8, 420)
end

function I4700197_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 7) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700197, 1, "I4700197_GiftBag")
aux.RegisterItemEvent(4700197, 0, "I4700197_CanUseGiftBag")



--4700209	低级魔神礼包
function I4700209_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8019002, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8019002, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101319, 1, 5, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8029002, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8029002, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101320, 1, 5, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8049002, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8049002, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091910, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101321, 1, 5, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8039002, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8039002, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091911, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101322, 1, 5, 8, 420)
    end
end

function I4700209_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700209, 1, "I4700209_GiftBag")
aux.RegisterItemEvent(4700209, 0, "I4700209_CanUseGiftBag")



--4700212	高级魔神礼包
function I4700212_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8019001, 1, 6, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8019001, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101315, 1, 6, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8029001, 1, 6, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8029001, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101316, 1, 6, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8049001, 1, 6, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8049001, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091908, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101317, 1, 6, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8039001, 1, 6, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8039001, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091909, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101318, 1, 6, 8, 420)
    end
end

function I4700212_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700212, 1, "I4700212_GiftBag")
aux.RegisterItemEvent(4700212, 0, "I4700212_CanUseGiftBag")


--4700210	初级回归奖励
function I4700210_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220903, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211504, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 100, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030128, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020025, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 15, -1, 8, 420)

	local class = role.GetRoleClass(TargetID)
	if class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8160008, 1, 5, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8170008, 1, 5, 8, 420)
	end
	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700153, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700154, 1, -1, 8, 420)
	end
end

function I4700210_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 16) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700210, 1, "I4700210_GiftBag")
aux.RegisterItemEvent(4700210, 0, "I4700210_CanUseGiftBag")


--4700211	高级回归奖励
function I4700211_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020027, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020029, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020072, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030128, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040058, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040023, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090020, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090019, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010026, 1, -1, 8, 420)

	local class = role.GetRoleClass(TargetID)
    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 4650001, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 4650101, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 4650201, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 4650301, 1, 4, 8, 420)
    end
end

function I4700211_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 15) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700211, 1, "I4700211_GiftBag")
aux.RegisterItemEvent(4700211, 0, "I4700211_CanUseGiftBag")



--4700214	神圣衣合成指南
function I4700214_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4700214, 1, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700215, 1, -1, 8, 420)
	return 1

end

function I4700214_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700214, 1, "I4700214_GiftBag")
aux.RegisterItemEvent(4700214, 0, "I4700214_CanUseGiftBag")

--4700215	神圣衣合成指南
function I4700215_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4700215, 1, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700216, 1, -1, 8, 420)
	return 1

end

function I4700215_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700215, 1, "I4700215_GiftBag")
aux.RegisterItemEvent(4700215, 0, "I4700215_CanUseGiftBag")

--4700216	神圣衣合成指南
function I4700216_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4700216, 1, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700217, 1, -1, 8, 420)
	return 1

end

function I4700216_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700216, 1, "I4700216_GiftBag")
aux.RegisterItemEvent(4700216, 0, "I4700216_CanUseGiftBag")

--4700217	神圣衣合成指南
function I4700217_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4700217, 1, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700214, 1, -1, 8, 420)
	return 1

end

function I4700217_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700217, 1, "I4700217_GiftBag")
aux.RegisterItemEvent(4700217, 0, "I4700217_CanUseGiftBag")



--4700199	聚宝盆
function I4700199_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700200, 1, -1, 8, 420)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,585937)
end

function I4700199_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700199, 1, "I4700199_GiftBag")
aux.RegisterItemEvent(4700199, 0, "I4700199_CanUseGiftBag")




--4700200	聚宝盆
function I4700200_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700201, 1, -1, 8, 420)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,1171875)
end

function I4700200_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700200, 1, "I4700200_GiftBag")
aux.RegisterItemEvent(4700200, 0, "I4700200_CanUseGiftBag")




--4700201	聚宝盆
function I4700201_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700202, 1, -1, 8, 420)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,2343750)
end

function I4700201_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700201, 1, "I4700201_GiftBag")
aux.RegisterItemEvent(4700201, 0, "I4700201_CanUseGiftBag")





--4700202	聚宝盆
function I4700202_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700203, 1, -1, 8, 420)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,4687500)
end

function I4700202_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700202, 1, "I4700202_GiftBag")
aux.RegisterItemEvent(4700202, 0, "I4700202_CanUseGiftBag")




--4700203	聚宝盆
function I4700203_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700204, 1, -1, 8, 420)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,9375000)
end

function I4700203_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700203, 1, "I4700203_GiftBag")
aux.RegisterItemEvent(4700203, 0, "I4700203_CanUseGiftBag")




--4700204	聚宝盆
function I4700204_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700205, 1, -1, 8, 420)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,18750000)
end

function I4700204_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700204, 1, "I4700204_GiftBag")
aux.RegisterItemEvent(4700204, 0, "I4700204_CanUseGiftBag")





--4700205	聚宝盆
function I4700205_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700206, 1, -1, 8, 420)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,37500000)
end

function I4700205_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700205, 1, "I4700205_GiftBag")
aux.RegisterItemEvent(4700205, 0, "I4700205_CanUseGiftBag")





--4700206	聚宝盆
function I4700206_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700207, 1, -1, 8, 420)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,75000000)
end

function I4700206_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700206, 1, "I4700206_GiftBag")
aux.RegisterItemEvent(4700206, 0, "I4700206_CanUseGiftBag")





--4700207	聚宝盆
function I4700207_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700208, 1, -1, 8, 420)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,150000000)
end

function I4700207_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700207, 1, "I4700207_GiftBag")
aux.RegisterItemEvent(4700207, 0, "I4700207_CanUseGiftBag")





--4700208	聚宝盆
function I4700208_GiftBag(MapID, InstanceID, TypeID, TargetID)
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
	role.AddRoleExp(MapID, InstanceID, TargetID,300000000)
end

function I4700208_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700208, 1, "I4700208_GiftBag")
aux.RegisterItemEvent(4700208, 0, "I4700208_CanUseGiftBag")
