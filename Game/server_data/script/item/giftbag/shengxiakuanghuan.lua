function I4800137_Use(MapID, InstanceID, TypeID, TargetID)
	local xuan = role.GetRoleItemNum(TargetID,4800139)
	local feng = role.GetRoleItemNum(TargetID,4800140)
	if(xuan >= 1 and feng >= 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800145, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800139, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800140, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif(xuan < 1 or feng < 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800146, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	end
	return 1
end

function I4800137_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bing = role.GetRoleItemNum(TargetID,4800137)
	local huo = role.GetRoleItemNum(TargetID,4800138)
	local sheng = role.GetRoleItemNum(TargetID,4800141)
	local xia = role.GetRoleItemNum(TargetID,4800142)
	local kuang = role.GetRoleItemNum(TargetID,4800143)
	local huan = role.GetRoleItemNum(TargetID,4800144)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (bing < 1 or huo < 1 or sheng < 1 or xia < 1 or kuang < 1 or huan < 1) then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4800137, 1, "I4800137_Use")
aux.RegisterItemEvent(4800137, 0, "I4800137_CanUse")


function I4800138_Use(MapID, InstanceID, TypeID, TargetID)
	local xuan = role.GetRoleItemNum(TargetID,4800139)
	local feng = role.GetRoleItemNum(TargetID,4800140)
	if(xuan >= 1 and feng >= 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800145, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800139, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800140, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif(xuan < 1 or feng < 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800146, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	end
	return 1
end

function I4800138_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bing = role.GetRoleItemNum(TargetID,4800137)
	local huo = role.GetRoleItemNum(TargetID,4800138)
	local sheng = role.GetRoleItemNum(TargetID,4800141)
	local xia = role.GetRoleItemNum(TargetID,4800142)
	local kuang = role.GetRoleItemNum(TargetID,4800143)
	local huan = role.GetRoleItemNum(TargetID,4800144)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (bing < 1 or huo < 1 or sheng < 1 or xia < 1 or kuang < 1 or huan < 1) then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4800138, 1, "I4800138_Use")
aux.RegisterItemEvent(4800138, 0, "I4800138_CanUse")


function I4800139_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800145, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800139, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800140, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	return 1
end

function I4800139_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bing = role.GetRoleItemNum(TargetID,4800137)
	local huo = role.GetRoleItemNum(TargetID,4800138)
	local xuan = role.GetRoleItemNum(TargetID,4800139)
	local feng = role.GetRoleItemNum(TargetID,4800140)
	local sheng = role.GetRoleItemNum(TargetID,4800141)
	local xia = role.GetRoleItemNum(TargetID,4800142)
	local kuang = role.GetRoleItemNum(TargetID,4800143)
	local huan = role.GetRoleItemNum(TargetID,4800144)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (bing < 1 or huo < 1 or xuan < 1 or feng < 1 or sheng < 1 or xia < 1 or kuang < 1 or huan < 1) then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4800139, 1, "I4800139_Use")
aux.RegisterItemEvent(4800139, 0, "I4800139_CanUse")


function I4800140_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800145, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800139, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800140, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	return 1
end

function I4800140_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bing = role.GetRoleItemNum(TargetID,4800137)
	local huo = role.GetRoleItemNum(TargetID,4800138)
	local xuan = role.GetRoleItemNum(TargetID,4800139)
	local feng = role.GetRoleItemNum(TargetID,4800140)
	local sheng = role.GetRoleItemNum(TargetID,4800141)
	local xia = role.GetRoleItemNum(TargetID,4800142)
	local kuang = role.GetRoleItemNum(TargetID,4800143)
	local huan = role.GetRoleItemNum(TargetID,4800144)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (bing < 1 or huo < 1 or xuan < 1 or feng < 1 or sheng < 1 or xia < 1 or kuang < 1 or huan < 1) then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4800140, 1, "I4800140_Use")
aux.RegisterItemEvent(4800140, 0, "I4800140_CanUse")


function I4800141_Use(MapID, InstanceID, TypeID, TargetID)
	local bing = role.GetRoleItemNum(TargetID,4800137)
	local huo = role.GetRoleItemNum(TargetID,4800138)
	local xuan = role.GetRoleItemNum(TargetID,4800139)
	local feng = role.GetRoleItemNum(TargetID,4800140)
	if(bing >= 1 and huo >= 1 and xuan >= 1 and feng >= 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800145, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800139, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800140, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif((xuan < 1 or feng < 1) and bing >= 1 and huo >= 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800146, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif(bing < 1 or huo < 1 or xuan < 1 or feng < 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800147, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	end
	return 1
end

function I4800141_CanUse(MapID, InstanceID, TypeID, TargetID)
	local sheng = role.GetRoleItemNum(TargetID,4800141)
	local xia = role.GetRoleItemNum(TargetID,4800142)
	local kuang = role.GetRoleItemNum(TargetID,4800143)
	local huan = role.GetRoleItemNum(TargetID,4800144)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (sheng < 1 or xia < 1 or kuang < 1 or huan < 1) then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4800141, 1, "I4800141_Use")
aux.RegisterItemEvent(4800141, 0, "I4800141_CanUse")


function I4800142_Use(MapID, InstanceID, TypeID, TargetID)
	local bing = role.GetRoleItemNum(TargetID,4800137)
	local huo = role.GetRoleItemNum(TargetID,4800138)
	local xuan = role.GetRoleItemNum(TargetID,4800139)
	local feng = role.GetRoleItemNum(TargetID,4800140)
	local sheng = role.GetRoleItemNum(TargetID,4800141)
	local kuang = role.GetRoleItemNum(TargetID,4800143)
	if(bing >= 1 and huo >= 1 and xuan >= 1 and feng >= 1 and sheng >=1 and kuang >=1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800145, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800139, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800140, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif((xuan < 1 or feng < 1) and bing >= 1 and huo >= 1 and sheng >=1 and kuang >=1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800146, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif((bing < 1 or huo < 1 or xuan < 1 or feng < 1)  and sheng >=1 and kuang >=1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800147, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif((bing < 1 or huo < 1 or xuan < 1 or feng < 1 or sheng < 1) and kuang >=1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800148, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif(bing < 1 or huo < 1 or xuan < 1 or feng < 1 or sheng < 1 or kuang < 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800149, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	end
	return 1
end

function I4800142_CanUse(MapID, InstanceID, TypeID, TargetID)
	local xia = role.GetRoleItemNum(TargetID,4800142)
	local huan = role.GetRoleItemNum(TargetID,4800144)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (xia < 1 or huan < 1) then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4800142, 1, "I4800142_Use")
aux.RegisterItemEvent(4800142, 0, "I4800142_CanUse")


function I4800143_Use(MapID, InstanceID, TypeID, TargetID)
	local bing = role.GetRoleItemNum(TargetID,4800137)
	local huo = role.GetRoleItemNum(TargetID,4800138)
	local xuan = role.GetRoleItemNum(TargetID,4800139)
	local feng = role.GetRoleItemNum(TargetID,4800140)
	local sheng = role.GetRoleItemNum(TargetID,4800141)
	if(bing >= 1 and huo >= 1 and xuan >= 1 and feng >= 1 and sheng >= 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800145, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800139, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800140, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif((xuan < 1 or feng < 1) and bing >= 1 and huo >= 1 and sheng >= 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800146, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif((bing < 1 or huo < 1 or xuan < 1 or feng < 1) and sheng >= 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800147, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif(bing < 1 or huo < 1 or xuan < 1 or feng < 1 or sheng < 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800148, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	end
	return 1
end

function I4800143_CanUse(MapID, InstanceID, TypeID, TargetID)
	local xia = role.GetRoleItemNum(TargetID,4800142)
	local kuang = role.GetRoleItemNum(TargetID,4800143)
	local huan = role.GetRoleItemNum(TargetID,4800144)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (xia < 1 or kuang < 1 or huan < 1) then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4800143, 1, "I4800143_Use")
aux.RegisterItemEvent(4800143, 0, "I4800143_CanUse")


function I4800144_Use(MapID, InstanceID, TypeID, TargetID)
	local bing = role.GetRoleItemNum(TargetID,4800137)
	local huo = role.GetRoleItemNum(TargetID,4800138)
	local xuan = role.GetRoleItemNum(TargetID,4800139)
	local feng = role.GetRoleItemNum(TargetID,4800140)
	local sheng = role.GetRoleItemNum(TargetID,4800141)
	local kuang = role.GetRoleItemNum(TargetID,4800143)
	if(bing >= 1 and huo >= 1 and xuan >= 1 and feng >= 1 and sheng >=1 and kuang >=1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800145, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800139, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800140, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif((xuan < 1 or feng < 1) and bing >= 1 and huo >= 1 and sheng >=1 and kuang >=1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800146, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800137, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800138, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif((bing < 1 or huo < 1 or xuan < 1 or feng < 1)  and sheng >=1 and kuang >=1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800147, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800141, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif((bing < 1 or huo < 1 or xuan < 1 or feng < 1 or sheng < 1) and kuang >=1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800148, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800143, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	elseif(bing < 1 or huo < 1 or xuan < 1 or feng < 1 or sheng < 1 or kuang < 1) then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800149, 1, -1, 8, 420)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800142, 1, 101)
	    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800144, 1, 101)
	end
	return 1
end

function I4800144_CanUse(MapID, InstanceID, TypeID, TargetID)
	local xia = role.GetRoleItemNum(TargetID,4800142)
	local huan = role.GetRoleItemNum(TargetID,4800144)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (xia < 1 or huan < 1) then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4800144, 1, "I4800144_Use")
aux.RegisterItemEvent(4800144, 0, "I4800144_CanUse")



--夏狂欢
function I4800148_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 2) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800148_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3090101, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070069, 1, -1, 8, 420)

end

aux.RegisterItemEvent(4800148, 1, "I4800148_GiftBag")
aux.RegisterItemEvent(4800148, 0, "I4800148_CanUseGiftBag")


--欢夏
function I4800149_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 3) then
        bRet = 40
    end

    return bRet, bIgnore
end

function I4800149_GiftBag(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070068, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)

end

aux.RegisterItemEvent(4800149, 1, "I4800149_GiftBag")
aux.RegisterItemEvent(4800149, 0, "I4800149_CanUseGiftBag")
