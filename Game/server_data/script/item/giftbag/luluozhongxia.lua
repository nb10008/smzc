function I4700138_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]= {4500002,2,1,2500}
	tbl[2]= {3070052,2,1,2500}
	tbl[3]= {3070055,4,1,2500}
	tbl[4]= {5610013,1,1,5}
	tbl[5]= {4221208,1,1,5}
	tbl[6]= {4800132,1,1,600}
	tbl[7]= {5600004,1,1,800}
	tbl[8]= {4400024,1,1,690}
	tbl[9]= {3030031,1,1,200}
	tbl[10]= {3030039,1,1,200}

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700138_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700138, 1, "I4700138_GiftBag")
aux.RegisterItemEvent(4700138, 0, "I4700138_CanUseGiftBag")
