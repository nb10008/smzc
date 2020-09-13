--3级新手包
function I3090003_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090004, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010001, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010009, 5, -1, 8, 420)
end

function I3090003_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090003, 1, "I3090003_GiftBag")
aux.RegisterItemEvent(3090003, 0, "I3090003_CanUseGiftBag")

--6级新手包
function I3090004_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090005, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010001, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010009, 5, -1, 8, 420)
end

function I3090004_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090004, 1, "I3090004_GiftBag")
aux.RegisterItemEvent(3090004, 0, "I3090004_CanUseGiftBag")

--11级新手包
function I3090005_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010009, 10, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
end

function I3090005_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090005, 1, "I3090005_GiftBag")
aux.RegisterItemEvent(3090005, 0, "I3090005_CanUseGiftBag")

--16级新手包
function I3090006_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090007, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010001, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010009, 5, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 1, -1, 8, 420)
end

function I3090006_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090006, 1, "I3090006_GiftBag")
aux.RegisterItemEvent(3090006, 0, "I3090006_CanUseGiftBag")

--21级新手包
function I3090007_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010002, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010010, 5, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3070026, 1, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3050071, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080080, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080081, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080082, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080083, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080084, 3, -1, 8, 420)
end

function I3090007_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090007, 1, "I3090007_GiftBag")
aux.RegisterItemEvent(3090007, 0, "I3090007_CanUseGiftBag")

--26级新手包
function I3090008_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090009, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010002, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010010, 5, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3070026, 1, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3050071, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
end

function I3090008_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090008, 1, "I3090008_GiftBag")
aux.RegisterItemEvent(3090008, 0, "I3090008_CanUseGiftBag")

--31级新手包
function I3090009_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010002, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010010, 5, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3070026, 1, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3050071, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3020004, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090016, 1, -1, 8, 420)
end

function I3090009_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090009, 1, "I3090009_GiftBag")
aux.RegisterItemEvent(3090009, 0, "I3090009_CanUseGiftBag")

--36级新手包
function I3090010_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010002, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010010, 5, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3070026, 1, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3050071, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 3020004, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070059, 3, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3090002, 1, -1, 8, 420)
end

function I3090010_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090010, 1, "I3090010_GiftBag")
aux.RegisterItemEvent(3090010, 0, "I3090010_CanUseGiftBag")

--41级新手包
function I3090011_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010003, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010011, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050072, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)

end

function I3090011_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090011, 1, "I3090011_GiftBag")
aux.RegisterItemEvent(3090011, 0, "I3090011_CanUseGiftBag")

--46级新手包
function I3090012_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010003, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010011, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050072, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
end

function I3090012_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090012, 1, "I3090012_GiftBag")
aux.RegisterItemEvent(3090012, 0, "I3090012_CanUseGiftBag")

--51级新手包
function I3090013_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010003, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010011, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050072, 6, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
end

function I3090013_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090013, 1, "I3090013_GiftBag")
aux.RegisterItemEvent(3090013, 0, "I3090013_CanUseGiftBag")

--56级新手包
function I3090014_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090015, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010003, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010011, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050072, 6, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090018, 1, -1, 8, 420)
end

function I3090014_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090014, 1, "I3090014_GiftBag")
aux.RegisterItemEvent(3090014, 0, "I3090014_CanUseGiftBag")

--51级新手包
function I3090015_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070038, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010012, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050073, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020006, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
end

function I3090015_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090015, 1, "I3090015_GiftBag")
aux.RegisterItemEvent(3090015, 0, "I3090015_CanUseGiftBag")

--雪山毛皮时装包
function I3090016_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8141001, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8141002, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8141003, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8141004, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8142001, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8142002, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8142003, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8142004, 1, -1, 8, 420)
	end
end

function I3090016_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090016, 1, "I3090016_GiftBag")
aux.RegisterItemEvent(3090016, 0, "I3090016_CanUseGiftBag")

--300赠点
function I3090002_zengquan(MapID, InstanceID, TypeID, TargetID)
	--修改赠点
	role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 300)
end
aux.RegisterItemEvent(3090002, 1, "I3090002_zengquan")

--小宝石袋
function I3090017_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030120, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030122, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030124, 2, -1, 8, 420)
end

function I3090017_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090017, 1, "I3090017_GiftBag")
aux.RegisterItemEvent(3090017, 0, "I3090017_CanUseGiftBag")

--大宝石袋
function I3090018_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030121, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030123, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030125, 2, -1, 8, 420)
end

function I3090018_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090018, 1, "I3090018_GiftBag")
aux.RegisterItemEvent(3090018, 0, "I3090018_CanUseGiftBag")
