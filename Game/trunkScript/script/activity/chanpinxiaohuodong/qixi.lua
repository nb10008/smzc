function I4810051_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={10000,3700}
	tbl[2]={50000,3500}
	tbl[3]={100000,2550}
	tbl[4]={1000000,140}
	tbl[5]={5000000,110}


	for i = 1,5 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end
	
	role.AddRoleSilver(MapID,InstanceID,TargetID,tbl[index][1], 420)
end
aux.RegisterItemEvent(4810051, 1, "I4810051_GiftBag")

function I4810052_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	
	if level <= 29 then
		role.AddRoleExp(MapID, InstanceID, TargetID, 8000)
	elseif level <= 49 then
		role.AddRoleExp(MapID, InstanceID, TargetID, 80000)
	elseif level <= 69 then
		role.AddRoleExp(MapID, InstanceID, TargetID, 180000)
	elseif level <= 79 then
		role.AddRoleExp(MapID, InstanceID, TargetID, 380000)
	elseif level <= 89 then
		role.AddRoleExp(MapID, InstanceID, TargetID, 880000)
	elseif level <= 100 then
		role.AddRoleExp(MapID, InstanceID, TargetID, 2880000)
	end

end
aux.RegisterItemEvent(4810052, 1, "I4810052_GiftBag")


function I4810053_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local x , y , z = unit.GetPosition(MapID,InstanceID,TargetID)

	map.MapCreateCreature(MapID,InstanceID, 6040201, x, y, z)

end
aux.RegisterItemEvent(4810053, 1, "I4810053_GiftBag")