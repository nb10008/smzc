function I4530138_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local dressMale = {4700046,4700069,4700070,4700116,4700117,4700118,4700119,4700126,4700127,
					4700128,4700129,4700130,4700131,4700132,4700133,4700134,4700135,4700136,
					4700153,4700155,4700157,4700159,4700173,4700175}
	local dressFemale = {4700045,4700066,4700067,4700068,4700112,4700113,4700114,4700115,4700120,
						4700121,4700122,4700123,4700124,4700125,4700137,4700154,4700156,4700158,
						4700160,4700161,4700162,4700163,4700164,4700165,4700166,4700167,4700168,
						4700169,4700170,4700171,4700172,4700174,4700176,4700177,4700178,4700179,
						4700180,4700181,4700182,4700183,4700184}
	local gender = role.GetRoleSex(TargetID)
	if gender == 1 then
		local i = math.random(#dressMale)
		role.AddRoleItem(MapID, InstanceID, TargetID, dressMale[i], 1, -1, 8, 420)
	elseif gender == 0 then
		local i = math.random(#dressFemale)
		role.AddRoleItem(MapID, InstanceID, TargetID, dressFemale[i], 1, -1, 8, 420)
	end
end

function I4530138_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530138, 1, "I4530138_GiftBag")
aux.RegisterItemEvent(4530138, 0, "I4530138_CanUseGiftBag")
