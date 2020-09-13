--生命本源礼包
function I4800288_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800287, 5, -1, 8, 420)

end

function I4800288_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800288, 1, "I4800288_GiftBag")
aux.RegisterItemEvent(4800288, 0, "I4800288_CanUseGiftBag")


--元素妖精礼包
function I4800289_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    local class = role.GetRoleClass(TargetID)
    if class==0 or class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8160039, 1, 5, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8170124, 1, 5, 8, 420)
    end
end

function I4800289_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800289, 1, "I4800289_GiftBag")
aux.RegisterItemEvent(4800289, 0, "I4800289_CanUseGiftBag")
