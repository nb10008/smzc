
--4级宝石袋
function I4530095_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(100)
	local b = {3030004,3030012,3030018,3030032,3030040,3030075,3030083,3030067,3030088}
	local c = {13,13,13,13,13,13,13,4,5}
	local k,j = 0,0
	for i=1, 9 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530095_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530095, 1, "I4530095_GiftBag")
aux.RegisterItemEvent(4530095, 0, "I4530095_CanUseGiftBag")


--5级宝石袋
function I4530096_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(100)
	local b = {3030005,3030013,3030019,3030033,3030041,3030076,3030084,3030068,3030089}
	local c = {13,13,13,13,13,13,13,4,5}
	local k,j = 0,0
	for i=1, 9 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530096_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530096, 1, "I4530096_GiftBag")
aux.RegisterItemEvent(4530096, 0, "I4530096_CanUseGiftBag")



--6级宝石袋
function I4530097_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(100)
	local b = {3030006,3030014,3030020,3030034,3030042,3030077,3030085,3030069,3030090}
	local c = {13,13,13,13,13,13,13,4,5}
	local k,j = 0,0
	for i=1, 9 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530097_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530097, 1, "I4530097_GiftBag")
aux.RegisterItemEvent(4530097, 0, "I4530097_CanUseGiftBag")







--7级宝石袋
function I4530098_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(100)
	local b = {3030007,3030015,3030021,3030035,3030043,3030078,3030086,3030070,3030091}
	local c = {13,13,13,13,13,13,13,4,5}
	local k,j = 0,0
	for i=1, 9 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530098_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530098, 1, "I4530098_GiftBag")
aux.RegisterItemEvent(4530098, 0, "I4530098_CanUseGiftBag")


--8级宝石袋
function I4530099_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(100)
	local b = {3030008,3030016,3030022,3030036,3030044,3030079,3030087,3030071,3030092}
	local c = {13,13,13,13,13,13,13,4,5}
	local k,j = 0,0
	for i=1, 9 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530099_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530099, 1, "I4530099_GiftBag")
aux.RegisterItemEvent(4530099, 0, "I4530099_CanUseGiftBag")



--3级圣印袋（加伤）
function I4530100_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(1000)
	local b = {3040003,3040008,3040013,3040018,3040023}
	local c = {225,225,225,225,100}
	local k,j = 0,0
	for i=1, 5 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530100_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530100, 1, "I4530100_GiftBag")
aux.RegisterItemEvent(4530100, 0, "I4530100_CanUseGiftBag")


--4级圣印袋（加伤）
function I4530101_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(1000)
	local b = {3040004,3040009,3040014,3040019,3040024}
	local c = {225,225,225,225,100}
	local k,j = 0,0
	for i=1, 5 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530101_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530101, 1, "I4530101_GiftBag")
aux.RegisterItemEvent(4530101, 0, "I4530101_CanUseGiftBag")




--5级圣印袋（加伤）
function I4530102_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(1000)
	local b = {3040005,3040010,3040015,3040020,3040025}
	local c = {225,225,225,225,100}
	local k,j = 0,0
	for i=1, 5 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530102_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530102, 1, "I4530102_GiftBag")
aux.RegisterItemEvent(4530102, 0, "I4530102_CanUseGiftBag")

--3级圣印袋（减伤）
function I4530103_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(1000)
	local b = {3040028,3040033,3040038,3040043,3040048,3040053,3040058}
	local c = {200,200,200,200,75,75,50}
	local k,j = 0,0
	for i=1, 7 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530103_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530103, 1, "I4530103_GiftBag")
aux.RegisterItemEvent(4530103, 0, "I4530103_CanUseGiftBag")





--4级圣印袋（减伤）
function I4530104_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(1000)
	local b = {3040029,3040034,3040039,3040044,3040049,3040054,3040059}
	local c = {200,200,200,200,75,75,50}
	local k,j = 0,0
	for i=1, 7 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530104_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530104, 1, "I4530104_GiftBag")
aux.RegisterItemEvent(4530104, 0, "I4530104_CanUseGiftBag")




--5级圣印袋（减伤）
function I4530105_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(1000)
	local b = {3040030,3040035,3040040,3040045,3040050,3040055,3040060}
	local c = {200,200,200,200,75,75,50}
	local k,j = 0,0
	for i=1, 7 do
	    k = k + c[i]
		if a <= k then ----------------and a > k - c[i] then
		    j = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, b[j], 1, -1, 8, 420)
end


function I4530105_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530105, 1, "I4530105_GiftBag")
aux.RegisterItemEvent(4530105, 0, "I4530105_CanUseGiftBag")




function I4530106_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(1,10000)
			if a >= 1 and a <= 1900 		then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500002, 5, -1, 8, 420)
				elseif a > 1900 and a<= 3730 		then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070029, 2, -1, 8, 420)
				elseif a >	3730	and	a<=	5230	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070052, 15, -1, 8, 420)
				elseif	a >	5230	and	a<=	6530	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020052, 1, -1, 8, 420)
				elseif	a >	6530	and	a<=	7830	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 1, -1, 8, 420)
				elseif	a >	7830	and	a<=	8330	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070045, 2, -1, 8, 420)
				elseif	a > 8330	and	a<=	8830	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020053, 1, -1, 8, 420)
				elseif	a > 8830	and	a<=	9340	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 1, -1, 8, 420)
				elseif	a > 9340	and	a<=	9540	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030118, 1, -1, 8, 420)
				elseif	a >	9540	and	a<=	9740	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030067, 1, -1, 8, 420)
				elseif	a >	9740	and	a<=	9920	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020012, 1, -1, 8, 420)
				elseif	a > 9920 	and	a<=	9970	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020012, 2, -1, 8, 420)
				elseif	a > 9970	and	a<=	9990	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050110, 1, -1, 8, 420)
				elseif	a > 9990	and	a<=	9995	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221208, 1, -1, 8, 420)
				elseif	a > 9995	and	a<=	10000	then
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020033, 1, -1, 8, 420)
			end
	end

function I4530106_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530106, 1, "I4530106_GiftBag")
aux.RegisterItemEvent(4530106, 0, "I4530106_CanUseGiftBag")





--彩橙果篮
function I4530107_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	if role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 30813) == true then
		HuoDeLiLian(MapID, InstanceID, TargetID, 52)
		LiLian_CheakComplete(MapID, InstanceID, TargetID)
	end
	local a = math.random(6)
	role.AddRoleItem(MapID, InstanceID, TargetID,4400003+3*a , 1, -1, 8, 420)
end


function I4530107_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530107, 1, "I4530107_GiftBag")
aux.RegisterItemEvent(4530107, 0, "I4530107_CanUseGiftBag")


--彩紫果篮
function I4530108_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	if role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 30814) == true then
		HuoDeLiLian(MapID, InstanceID, TargetID, 10)
		LiLian_CheakComplete(MapID, InstanceID, TargetID)
	end
	local a = math.random(6)
	role.AddRoleItem(MapID, InstanceID, TargetID,4400004+3*a , 1, -1, 8, 420)
end


function I4530108_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530108, 1, "I4530108_GiftBag")
aux.RegisterItemEvent(4530108, 0, "I4530108_CanUseGiftBag")


--彩粉果篮
function I4530109_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local a = math.random(6)

	role.AddRoleItem(MapID, InstanceID, TargetID,4400005+3*a , 1, -1, 8, 420)
end


function I4530109_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 2 then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530109, 1, "I4530109_GiftBag")
aux.RegisterItemEvent(4530109, 0, "I4530109_CanUseGiftBag")

































