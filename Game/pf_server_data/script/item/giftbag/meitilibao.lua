

--17173圣魔大礼包
function I4520001_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520020, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)

	--判断玩家性别

	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then

		role.AddRoleItem(MapID, InstanceID, TargetID, 4210601, 1, -1, 8, 420)
	else

		role.AddRoleItem(MapID, InstanceID, TargetID, 4210701, 1, -1, 8, 420)
	end
end


function I4520001_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520001, 1, "I4520001_GiftBag")
aux.RegisterItemEvent(4520001, 0, "I4520001_CanUseGiftBag")


--5617圣魔大礼包
function I4520002_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8200001, 1, -1, 8, 420)


end


function I4520002_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520002, 1, "I4520002_GiftBag")
aux.RegisterItemEvent(4520002, 0, "I4520002_CanUseGiftBag")




--U9圣魔大礼包
function I4520003_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8200002, 1, -1, 8, 420)


end


function I4520003_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520003, 1, "I4520003_GiftBag")
aux.RegisterItemEvent(4520003, 0, "I4520003_CanUseGiftBag")



--766特权大礼包
function I4520004_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520031, 1, -1, 8, 420)


end


function I4520004_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520004, 1, "I4520004_GiftBag")
aux.RegisterItemEvent(4520004, 0, "I4520004_CanUseGiftBag")




--新浪特权大礼包
function I4520005_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)

	--判断玩家性别

	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then

		role.AddRoleItem(MapID, InstanceID, TargetID, 4210801, 1, -1, 8, 420)
	else

		role.AddRoleItem(MapID, InstanceID, TargetID, 4210901, 1, -1, 8, 420)
	end
end


function I4520005_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520005, 1, "I4520005_GiftBag")
aux.RegisterItemEvent(4520005, 0, "I4520005_CanUseGiftBag")



--电玩巴士大礼包
function I4520006_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520016, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211001, 1, -1, 8, 420)


end


function I4520006_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520006, 1, "I4520006_GiftBag")
aux.RegisterItemEvent(4520006, 0, "I4520006_CanUseGiftBag")



--圣魔网易专属包
function I4520007_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520026, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8200004, 1, -1, 8, 420)


end


function I4520007_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520007, 1, "I4520007_GiftBag")
aux.RegisterItemEvent(4520007, 0, "I4520007_CanUseGiftBag")




--硅谷动力圣魔包
function I4520008_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520027, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8200005, 1, -1, 8, 420)


end


function I4520008_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520008, 1, "I4520008_GiftBag")
aux.RegisterItemEvent(4520008, 0, "I4520008_CanUseGiftBag")




--52PK圣魔特权包
function I4520009_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520019, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520028, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8200006, 1, -1, 8, 420)


end


function I4520009_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520009, 1, "I4520009_GiftBag")
aux.RegisterItemEvent(4520009, 0, "I4520009_CanUseGiftBag")

--信仰礼包
function I4520033_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4520034, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520035, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211101, 1, -1, 8, 420)


end


function I4520033_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4520033, 1, "I4520033_GiftBag")
aux.RegisterItemEvent(4520033, 0, "I4520033_CanUseGiftBag")




















