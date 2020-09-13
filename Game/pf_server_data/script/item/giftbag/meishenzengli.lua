--美神的赠礼
function I4530111_CanUse(MapID, InstanceID, ItemTypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
		return bRet, bIgnore
	end
end

aux.RegisterItemEvent(4530111, 0, "I4530111_CanUse")

function I4530111_Use(MapID, InstanceID, ItemTypeID, TargetID)
	local num = math.random(1000)
	if num < 350 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050111, 1, -1, 8, 420)
	elseif num < 500 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050111, 2, -1, 8, 420)
	elseif num < 550 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050111, 3, -1, 8, 420)
	elseif num < 590 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050112, 1, -1, 8, 420)
	elseif num < 600 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050112, 2, -1, 8, 420)
	elseif num < 660 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050301, 1, -1, 8, 420)
	elseif num < 720 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050302, 1, -1, 8, 420)
	elseif num < 790 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050401, 1, -1, 8, 420)
	elseif num < 860 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050402, 1, -1, 8, 420)
	elseif num < 930 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050403, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 3050404, 1, -1, 8, 420)
	end
end
aux.RegisterItemEvent(4530111, 1, "I4530111_Use")
