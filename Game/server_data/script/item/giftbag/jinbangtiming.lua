function I4700087_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {3030353,1,0,10}
	tbl[2] = {4800129,1,0,500}
	tbl[3] = {3020090,1,0,1}
	tbl[4] = {3030383,1,0,10}
	tbl[5] = {3030119,1,0,50}
	tbl[6] = {3100097,5,0,300}
	tbl[7] = {3100127,5,0,300}
	tbl[8] = {4211103,1,0,10}
	tbl[9] = {4221504,1,0,10}
	tbl[10] = {5600004,1,0,1000}
    tbl[11] = {5600008,1,0,909}
	tbl[12] = {3070013,1,0,500}
	tbl[13] = {3070019,5,0,700}
	tbl[14] = {4400031,1,0,500}
	tbl[15] = {4820027,5,0,600}
	tbl[16] = {3070006,1,0,200}
	tbl[17] = {3070052,10,0,1000}
	tbl[18] = {4400024,1,0,500}
	tbl[19] = {4500002,20,0,800}
	tbl[20] = {4820028,5,0,700}
	tbl[21] = {4820029,1,0,200}
	tbl[22] = {4820024,1,0,300}
	tbl[23] = {3020052,1,0,200}
	tbl[24] = {4400047,5,0,700}

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700087_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700087, 1, "I4700087_GiftBag")
aux.RegisterItemEvent(4700087, 0, "I4700087_CanUseGiftBag")
