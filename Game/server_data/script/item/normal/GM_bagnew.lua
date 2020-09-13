--追杀GM的宝箱
function I4530002_Use(MapID, InstanceID, TypeID, TargetID)
	local itemid = {3020012,3020011,3020008,3020016,3020010,3020007,3070002,3070052,3070018,3070055,3070023}
    for i = 1,11 do
		local num = role.GetRoleItemNum(TargetID, itemid[i])
		if num ~= nil and num > 0 then
			role.RemoveFromRole(MapID, InstanceID, TargetID, itemid[i], num, 420)
		end
	end
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020012, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020008, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020016, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020007, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070002, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070052, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 200, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070055, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070023, 5, -1, 8, 420)
	--[[罪恶值 -3000     74
物防   50000     13
法防   50000     15
物攻   10000     12
法攻   10000     14
级别   80级      592238960
血量   500000    8 9
魔法   500000    10 11
移动速度 20000   28]]
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	while rolelevel < 99 do
	    role.AddRoleExp(MapID, InstanceID, TargetID, 99999999)
	    rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	end
	local k1 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 74)
	local k2 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 13)
	local k3 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 15)
	local k4 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 12)
	local k5 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 14)
	local k6 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 8)
	local k7 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 9)
	local k8 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 10)
	local k9 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 11)
	local k10 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 28)
	local k11 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 66)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 74, -6000-k1)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 13, 50000-k2)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 15, 50000-k3)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 12, 10000-k4)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 14, 10000-k5)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 8, 500000-k6)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 9, 500000-k7)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 10, 500000-k8)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 11, 500000-k9)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 28, 20000-k10)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 66, 20000-k11)


end

function I4530002_CanUse(MapID, InstanceID, TypeID, TargetID)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		return 40, false
	end
	return 0,false
end

aux.RegisterItemEvent(4530002, 1, "I4530002_Use")
aux.RegisterItemEvent(4530002, 0, "I4530002_CanUse")

