function I4820240_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3020103, 3, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030142, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4600001, 8, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 4820021, 5, -1, 8, 420)

	local k = math.random(30)
	if k == 16 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 3020024+math.random(3), 1, -1, 8, 420)
    end
end

function I4820240_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820240, 1, "I4820240_GiftBag")
aux.RegisterItemEvent(4820240, 0, "I4820240_CanUseGiftBag")

function I4820241_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3020103, 6, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030142, 6, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4600001, 18, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 4820021, 12, -1, 8, 420)
	local k = math.random(20)
	if k == 16 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 3020024+math.random(3), 1, -1, 8, 420)
    end
end

function I4820241_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820241, 1, "I4820241_GiftBag")
aux.RegisterItemEvent(4820241, 0, "I4820241_CanUseGiftBag")

function I4820242_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3020103, 10, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3030142, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4600001, 30, -1, 8, 420)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 4820021, 20, -1, 8, 420)
	local k = math.random(30)
	if k == 6 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 3020024+math.random(3), 1, -1, 8, 420)
    end
end

function I4820242_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820242, 1, "I4820242_GiftBag")
aux.RegisterItemEvent(4820242, 0, "I4820242_CanUseGiftBag")

function I3020104_Use(MapID, InstanceID, TypeID, TargetID)
    local tbl = {}
	tbl[3020104] = {20,3020027}
	tbl[3020105] = {60,3020030}
	tbl[3020106] = {300,3020032}
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID][2], 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3020103, tbl[TypeID][1], 101)
	--role.RemoveFromRole(MapID, InstanceID, TargetID, TypeID, 1, 101)
	--return 1
end

function I3020104_CanUse(MapID, InstanceID, TypeID, TargetID)
    local tbl = {}
	tbl[3020104] = {20,3020027}
	tbl[3020105] = {60,3020030}
	tbl[3020106] = {300,3020032}

	local b = role.GetRoleItemNum(TargetID,3020103)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if b < tbl[TypeID][1] then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(3020104, 1, "I3020104_Use")
aux.RegisterItemEvent(3020104, 0, "I3020104_CanUse")
aux.RegisterItemEvent(3020105, 1, "I3020104_Use")
aux.RegisterItemEvent(3020105, 0, "I3020104_CanUse")
aux.RegisterItemEvent(3020106, 1, "I3020104_Use")
aux.RegisterItemEvent(3020106, 0, "I3020104_CanUse")

function I3050088_Use(MapID, InstanceID, TypeID, TargetID)
    local tbl = {}
	tbl[3050088] = {10,3030131}
	tbl[3050089] = {20,3030138}
	tbl[3050090] = {10,3030134}
	tbl[3050091] = {20,3030139}
	tbl[3050092] = {15,3030128}
	tbl[3050093] = {30,3030137}
	tbl[3050094] = {10,3030121}
	tbl[3050095] = {30,3030140}
	tbl[3050096] = {10,3030123}
	tbl[3050097] = {30,3030141}

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID][2], 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030142, tbl[TypeID][1], 101)
	--role.RemoveFromRole(MapID, InstanceID, TargetID, TypeID, 1, 101)
	--return 1
end

function I3050088_CanUse(MapID, InstanceID, TypeID, TargetID)
    local tbl = {}
	tbl[3050088] = {10,3030131}
	tbl[3050089] = {20,3030138}
	tbl[3050090] = {10,3030134}
	tbl[3050091] = {20,3030139}
	tbl[3050092] = {15,3030128}
	tbl[3050093] = {30,3030137}
	tbl[3050094] = {10,3030121}
	tbl[3050095] = {30,3030140}
	tbl[3050096] = {10,3030123}
	tbl[3050097] = {30,3030141}

	local b = role.GetRoleItemNum(TargetID,3030142)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if b < tbl[TypeID][1] then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(3050088, 1, "I3050088_Use")
aux.RegisterItemEvent(3050088, 0, "I3050088_CanUse")
aux.RegisterItemEvent(3050089, 1, "I3050088_Use")
aux.RegisterItemEvent(3050089, 0, "I3050088_CanUse")
aux.RegisterItemEvent(3050090, 1, "I3050088_Use")
aux.RegisterItemEvent(3050090, 0, "I3050088_CanUse")
aux.RegisterItemEvent(3050091, 1, "I3050088_Use")
aux.RegisterItemEvent(3050091, 0, "I3050088_CanUse")
aux.RegisterItemEvent(3050092, 1, "I3050088_Use")
aux.RegisterItemEvent(3050092, 0, "I3050088_CanUse")
aux.RegisterItemEvent(3050093, 1, "I3050088_Use")
aux.RegisterItemEvent(3050093, 0, "I3050088_CanUse")
aux.RegisterItemEvent(3050094, 1, "I3050088_Use")
aux.RegisterItemEvent(3050094, 0, "I3050088_CanUse")
aux.RegisterItemEvent(3050095, 1, "I3050088_Use")
aux.RegisterItemEvent(3050095, 0, "I3050088_CanUse")
aux.RegisterItemEvent(3050096, 1, "I3050088_Use")
aux.RegisterItemEvent(3050096, 0, "I3050088_CanUse")
aux.RegisterItemEvent(3050097, 1, "I3050088_Use")
aux.RegisterItemEvent(3050097, 0, "I3050088_CanUse")
