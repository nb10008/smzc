
--腾讯彩灯元宵卡
function I4530224_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530225, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,3070057,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070006,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4222003,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010054,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010055,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022,1, -1, 8, 420)

	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID,4700248,1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID,4700249,1, -1, 8, 420)
	end

end

function I4530224_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530224, 1, "I4530224_GiftBag")
aux.RegisterItemEvent(4530224, 0, "I4530224_CanUseGiftBag")

function I4530225_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530226, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,3070056,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070051,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010035,5, -1, 8, 420)

end

function I4530225_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530225, 1, "I4530225_GiftBag")
aux.RegisterItemEvent(4530225, 0, "I4530225_CanUseGiftBag")


function I4530226_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530227, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,4820081,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020010,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020005,50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4530177,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020114,2, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
	if class==2 or class==6 or class==7 or class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID,8160010,1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 or class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID,8170011,1, 4, 8, 420)
    end


end

function I4530226_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530226, 1, "I4530226_GiftBag")
aux.RegisterItemEvent(4530226, 0, "I4530226_CanUseGiftBag")

function I4530227_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530228, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,4820081,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020010,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020005,50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4530178,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070080,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020114,2, -1, 8, 420)


end

function I4530227_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530227, 1, "I4530227_GiftBag")
aux.RegisterItemEvent(4530227, 0, "I4530227_CanUseGiftBag")

function I4530228_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID,4820081,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070069,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020005,50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3090030,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020118,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030147,4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022,1, -1, 8, 420)



end

function I4530228_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 13) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530228, 1, "I4530228_GiftBag")
aux.RegisterItemEvent(4530228, 0, "I4530228_CanUseGiftBag")


