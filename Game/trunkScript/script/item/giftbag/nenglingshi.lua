
function I3500058_Use(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(1,8)
	if r ==1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500049, 1, -1, 8, 420)
	elseif r ==2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500050, 1, -1, 8, 420)
	elseif r ==3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500051, 1, -1, 8, 420)
	elseif r ==4 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500052, 1, -1, 8, 420)
	elseif r ==5 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500053, 1, -1, 8, 420)
	elseif r ==6 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500054, 1, -1, 8, 420)
	elseif r ==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500055, 1, -1, 8, 420)
	elseif r ==8 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500056, 1, -1, 8, 420)
	end
end

function I3500058_CanUse(MapID, InstanceID, TypeID, TargetID)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end

	return 0,false
end

aux.RegisterItemEvent(3500058, 1, "I3500058_Use")
aux.RegisterItemEvent(3500058, 0, "I3500058_CanUse")
function I3500057_Use(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(1,8)
	if r ==1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500041, 1, -1, 8, 420)
	elseif r ==2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500042, 1, -1, 8, 420)
	elseif r ==3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500043, 1, -1, 8, 420)
	elseif r ==4 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500044, 1, -1, 8, 420)
	elseif r ==5 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500045, 1, -1, 8, 420)
	elseif r ==6 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500046, 1, -1, 8, 420)
	elseif r ==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500047, 1, -1, 8, 420)
	elseif r ==8 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3500048, 1, -1, 8, 420)
	end
end

function I3500057_CanUse(MapID, InstanceID, TypeID, TargetID)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end

	return 0,false
end

aux.RegisterItemEvent(3500057, 1, "I3500057_Use")
aux.RegisterItemEvent(3500057, 0, "I3500057_CanUse")