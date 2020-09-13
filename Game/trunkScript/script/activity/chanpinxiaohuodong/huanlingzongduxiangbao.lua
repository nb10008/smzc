
function I4830112_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local tab = {}
	tab[1] = {3020116, 2}
	tab[2] = {4800242, 5}
	tab[3] = {3070051, 20}
	tab[4] = {3070056, 100}
	tab[5] = {3070006, 1}
	tab[6] = {4212014, 1}
	tab[7] = {4300008, 1}
	tab[8] = {4800287, 120}
	tab[9] = {8112008, 1}
	tab[10] = {8112009, 1}
	tab[11] = {4820081, 50}
	tab[12] = {4220302, 1}
	for i = 1,12 do
		role.AddRoleItem(MapID, InstanceID, TargetID, tab[i][1], tab[i][2], -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830112, 1, "I4830112_GiftBag")

function I4830112_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830112, 0, "I4830112_CanUseGiftBag")

function I4830113_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local tab = {}
	tab[1] = {3020116, 2}
	tab[2] = {4800242, 5}
	tab[3] = {3070051, 20}
	tab[4] = {3070056, 100}
	tab[5] = {3070006, 1}
	tab[6] = {4212014, 1}
	tab[7] = {4300008, 1}
	tab[8] = {4800287, 120}
	tab[9] = {8112008, 1}
	tab[10] = {8112009, 1}
	tab[11] = {4820081, 50}
	tab[12] = {4220402, 1}
	for i = 1,12 do
		role.AddRoleItem(MapID, InstanceID, TargetID, tab[i][1], tab[i][2], -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830113, 1, "I4830113_GiftBag")

function I4830113_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830113, 0, "I4830113_CanUseGiftBag")


function I4830114_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local tab = {}
	tab[1] = {3020116, 2}
	tab[2] = {4800242, 5}
	tab[3] = {3070051, 20}
	tab[4] = {3070056, 100}
	tab[5] = {3070006, 1}
	tab[6] = {4212014, 1}
	tab[7] = {4300008, 1}
	tab[8] = {4800287, 120}
	tab[9] = {8112008, 1}
	tab[10] = {8112009, 1}
	tab[11] = {4820081, 50}
	tab[12] = {4220502, 1}
	for i = 1,12 do
		role.AddRoleItem(MapID, InstanceID, TargetID, tab[i][1], tab[i][2], -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830114, 1, "I4830114_GiftBag")

function I4830114_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830114, 0, "I4830114_CanUseGiftBag")


function I4830115_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local tab = {}
	tab[1] = {3020116, 2}
	tab[2] = {4800242, 5}
	tab[3] = {3070051, 20}
	tab[4] = {3070056, 100}
	tab[5] = {3070006, 1}
	tab[6] = {4212014, 1}
	tab[7] = {4300008, 1}
	tab[8] = {4800287, 120}
	tab[9] = {8112008, 1}
	tab[10] = {8112009, 1}
	tab[11] = {4820081, 50}
	tab[12] = {4220802, 1}
	for i = 1,12 do
		role.AddRoleItem(MapID, InstanceID, TargetID, tab[i][1], tab[i][2], -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830115, 1, "I4830115_GiftBag")

function I4830115_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830115, 0, "I4830115_CanUseGiftBag")


function I4830116_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local tab = {}
	tab[1] = {3020116, 2}
	tab[2] = {4800242, 5}
	tab[3] = {3070051, 20}
	tab[4] = {3070056, 100}
	tab[5] = {3070006, 1}
	tab[6] = {4212014, 1}
	tab[7] = {4300008, 1}
	tab[8] = {4800287, 120}
	tab[9] = {8112008, 1}
	tab[10] = {8112009, 1}
	tab[11] = {4820081, 50}
	tab[12] = {4220902, 1}
	for i = 1,12 do
		role.AddRoleItem(MapID, InstanceID, TargetID, tab[i][1], tab[i][2], -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830116, 1, "I4830116_GiftBag")

function I4830116_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830116, 0, "I4830116_CanUseGiftBag")


function I4830117_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local tab = {}
	tab[1] = {3020116, 2}
	tab[2] = {4800242, 5}
	tab[3] = {3070051, 20}
	tab[4] = {3070056, 100}
	tab[5] = {3070006, 1}
	tab[6] = {4212014, 1}
	tab[7] = {4300008, 1}
	tab[8] = {4800287, 120}
	tab[9] = {8112008, 1}
	tab[10] = {8112009, 1}
	tab[11] = {4820081, 50}
	tab[12] = {4221202, 1}
	for i = 1,12 do
		role.AddRoleItem(MapID, InstanceID, TargetID, tab[i][1], tab[i][2], -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830117, 1, "I4830117_GiftBag")

function I4830117_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830117, 0, "I4830117_CanUseGiftBag")


function I4830118_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local tab = {}
	tab[1] = {3020116, 2}
	tab[2] = {4800242, 5}
	tab[3] = {3070051, 20}
	tab[4] = {3070056, 100}
	tab[5] = {3070006, 1}
	tab[6] = {4212014, 1}
	tab[7] = {4300008, 1}
	tab[8] = {4800287, 120}
	tab[9] = {8112008, 1}
	tab[10] = {8112009, 1}
	tab[11] = {4820081, 50}
	tab[12] = {4221302, 1}

	tab[13] = {3030130, 2}
	tab[14] = {3030133, 2}
	tab[15] = {3030121, 2}
	tab[16] = {3030123, 2}
	
	for i = 1,16 do
		role.AddRoleItem(MapID, InstanceID, TargetID, tab[i][1], tab[i][2], -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830118, 1, "I4830118_GiftBag")

function I4830118_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 16) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830118, 0, "I4830118_CanUseGiftBag")
