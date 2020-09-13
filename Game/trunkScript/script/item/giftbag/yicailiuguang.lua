function I4700235_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {3030302,1,-1,60}
	tbl[2] = {4820046,2,-1,840}
	tbl[3] = {4820047,2,-1,850}
	tbl[4] = {3070019,2,-1,820}
	tbl[5] = {4700231,1,-1,50}
	tbl[6] = {4700230,1,-1,750}
	tbl[7] = {4700229,1,-1,850}
	tbl[8] = {4820120,1,-1,200}
	tbl[9] = {4820121,1,-1,200}
	tbl[10] = {4820035,1,-1,250}
	tbl[11] = {4820172,1,-1,500}
	tbl[12] = {4820239,1,-1,200}
	tbl[13] = {4999004,1,-1,330}
	tbl[14] = {4820180,1,-1,22}
	tbl[15] = {4820048,50,-1,1000}
	tbl[16] = {5600006,1,-1,400}
	tbl[17] = {5600004,1,-1,640}
	tbl[18] = {4500002,8,-1,750}
	tbl[19] = {3070052,8,-1,750}
	tbl[20] = {4820108,3,-1,350}
	tbl[21] = {3030382,1,-1,60}
	tbl[22] = {3030342,1,-1,60}
	tbl[23] = {3030327,1,-1,60}
	tbl[24] = {3030385,1,-1,2}
	tbl[25] = {3030345,1,-1,2}
	tbl[26] = {3030330,1,-1,2}
	tbl[27] = {3030325,1,-1,2}


	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700235_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700235, 1, "I4700235_GiftBag")
aux.RegisterItemEvent(4700235, 0, "I4700235_CanUseGiftBag")
