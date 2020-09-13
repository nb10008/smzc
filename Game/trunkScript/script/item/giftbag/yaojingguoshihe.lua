function I4700292_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100)
	local temp = 0
	local index = 1
	local tbl = {}

    tbl[1]={4400004,1,-1,20}
    tbl[2]={4400019,1,-1,16}
    tbl[3]={4400022,1,-1,16}
    tbl[4]={4400010,1,-1,10}
    tbl[5]={4400016,1,-1,10}
    tbl[6]={4400007,1,-1,4}
    tbl[7]={4400013,1,-1,4}
    tbl[8]={4400005,1,-1,6}
    tbl[9]={4400020,1,-1,4}
	tbl[10]={4400023,1,-1,4}
    tbl[11]={4400011,1,-1,2}
    tbl[12]={4400017,1,-1,2}
    tbl[13]={4400008,1,-1,1}
    tbl[14]={4400014,1,-1,1}


	for i = 1,14 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700292_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700292, 1, "I4700292_GiftBag")
aux.RegisterItemEvent(4700292, 0, "I4700292_CanUseGiftBag")



function I4700293_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100)
	local temp = 0
	local index = 1
	local tbl = {}
    tbl[1]={4400003,1,-1,18}
    tbl[2]={4400018,1,-1,15}
	tbl[3]={4400021,1,-1,15}
    tbl[4]={4400009,1,-1,10}
    tbl[5]={4400015,1,-1,10}
    tbl[6]={4400006,1,-1,8}
    tbl[7]={4400012,1,-1,8}
    tbl[8]={4400004,1,-1,4}
    tbl[9]={4400019,1,-1,3}
    tbl[10]={4400022,1,-1,3}
    tbl[11]={4400010,1,-1,2}
    tbl[12]={4400016,1,-1,2}
    tbl[13]={4400007,1,-1,1}
    tbl[14]={4400013,1,-1,1}



	for i = 1,14 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700293_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700293, 1, "I4700293_GiftBag")
aux.RegisterItemEvent(4700293, 0, "I4700293_CanUseGiftBag")
