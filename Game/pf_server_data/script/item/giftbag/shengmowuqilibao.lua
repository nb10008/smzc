function I4700288_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if class == 0 or class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8019003, 1, 6, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8029003, 1, 6, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8049003, 1, 6, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8039003, 1, 6, 8, 420)
    end
end

function I4700288_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700288, 1, "I4700288_GiftBag")
aux.RegisterItemEvent(4700288, 0, "I4700288_CanUseGiftBag")

function I4700289_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if class == 0 or class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8050912, 1, 6, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8050913, 1, 6, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8051912, 1, 6, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8051913, 1, 6, 8, 420)
    end
end

function I4700289_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700289, 1, "I4700289_GiftBag")
aux.RegisterItemEvent(4700289, 0, "I4700289_CanUseGiftBag")


function I4700290_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if class == 0 or class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8060912, 1, 6, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8060913, 1, 6, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8061912, 1, 6, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8061913, 1, 6, 8, 420)
    end

end

function I4700290_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700290, 1, "I4700290_GiftBag")
aux.RegisterItemEvent(4700290, 0, "I4700290_CanUseGiftBag")

function I4700291_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if class == 0 or class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8090912, 1, 6, 8, 420)
    elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8090913, 1, 6, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8091912, 1, 6, 8, 420)
    elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8091913, 1, 6, 8, 420)
    end
end

function I4700291_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700291, 1, "I4700291_GiftBag")
aux.RegisterItemEvent(4700291, 0, "I4700291_CanUseGiftBag")

