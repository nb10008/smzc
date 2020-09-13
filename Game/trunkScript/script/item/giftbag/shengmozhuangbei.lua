--对戒包
function I4700223_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    local class = role.GetRoleClass(TargetID)
    if class==1 or class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8110916, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8110917, 1, 6, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8110918, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8110919, 1, 6, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8111916, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8111917, 1, 6, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8111918, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8111919, 1, 6, 8, 420)
    end
end

function I4700223_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700223, 1, "I4700223_GiftBag")
aux.RegisterItemEvent(4700223, 0, "I4700223_CanUseGiftBag")


--防具包
function I4700224_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020042, 1, -1, 8, 420)
    if class==1 or class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060912, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090912, 1, 6, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060913, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090913, 1, 6, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061912, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091912, 1, 6, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061913, 1, 6, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091913, 1, 6, 8, 420)
    end
end

function I4700224_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700224, 1, "I4700224_GiftBag")
aux.RegisterItemEvent(4700224, 0, "I4700224_CanUseGiftBag")


--战靴包
function I4700225_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020037, 1, -1, 8, 420)
    if class==1 or class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090912, 1, 6, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090913, 1, 6, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091912, 1, 6, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091913, 1, 6, 8, 420)
    end
end

function I4700225_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700225, 1, "I4700225_GiftBag")
aux.RegisterItemEvent(4700225, 0, "I4700225_CanUseGiftBag")




--神魂晶包
function I4700226_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 8220403, 1, 6, 8, 420)

end

function I4700226_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700226, 1, "I4700226_GiftBag")
aux.RegisterItemEvent(4700226, 0, "I4700226_CanUseGiftBag")



--声望称号包
function I4700227_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 8220404, 1, 6, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820181, 1, -1, 8, 420)

end

function I4700227_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700227, 1, "I4700227_GiftBag")
aux.RegisterItemEvent(4700227, 0, "I4700227_CanUseGiftBag")



--圣魔护手包
function I4700228_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    local class = role.GetRoleClass(TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8220404, 1, 6, 8, 420)
    if class==1 or class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080912, 1, 6, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080913, 1, 6, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081912, 1, 6, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081913, 1, 6, 8, 420)
    end

end

function I4700228_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700228, 1, "I4700228_GiftBag")
aux.RegisterItemEvent(4700228, 0, "I4700228_CanUseGiftBag")





--8级成长箱
function I4700229_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local waigong={8010018,8020018,8050018,8060018,8070018,8080018,8090018,8100018,8110018,8121018,8130018}
	local neigong={8030018,8040018,8051018,8061018,8071018,8081018,8091018,8100018,8111018,8122018,8130018}
    local class = role.GetRoleClass(TargetID)
    if class==1 or class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, waigong[math.random(11)], 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, waigong[math.random(11)], 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, neigong[math.random(11)], 1, 4, 8,420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, neigong[math.random(11)], 1, 4, 8, 420)
    end

end

function I4700229_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700229, 1, "I4700229_GiftBag")
aux.RegisterItemEvent(4700229, 0, "I4700229_CanUseGiftBag")


--9级成长箱
function I4700230_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local waigong={8010018,8020018,8050018,8060018,8070018,8080018,8090018,8100018,8110018,8121018,8130018}
	local neigong={8030018,8040018,8051018,8061018,8071018,8081018,8091018,8100018,8111018,8122018,8130018}
    local class = role.GetRoleClass(TargetID)
    if class==1 or class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, waigong[math.random(11)]+1, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, waigong[math.random(11)]+1, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, neigong[math.random(11)]+1, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, neigong[math.random(11)]+1, 1, 4, 8, 420)
    end

end

function I4700230_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700230, 1, "I4700230_GiftBag")
aux.RegisterItemEvent(4700230, 0, "I4700230_CanUseGiftBag")



--10级成长箱
function I4700231_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local waigong={8010018,8020018,8050018,8060018,8070018,8080018,8090018,8100018,8110018,8121018,8130018}
	local neigong={8030018,8040018,8051018,8061018,8071018,8081018,8091018,8100018,8111018,8122018,8130018}
    local class = role.GetRoleClass(TargetID)
    if class==1 or class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, waigong[math.random(11)]+2, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, waigong[math.random(11)]+2, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, neigong[math.random(11)]+2, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, neigong[math.random(11)]+2, 1, 4, 8, 420)
    end

end

function I4700231_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700231, 1, "I4700231_GiftBag")
aux.RegisterItemEvent(4700231, 0, "I4700231_CanUseGiftBag")



--火红拉丁包
function I4700233_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700159, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700160, 1, -1, 8, 420)
	end

end

function I4700233_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700233, 1, "I4700233_GiftBag")
aux.RegisterItemEvent(4700233, 0, "I4700233_CanUseGiftBag")
