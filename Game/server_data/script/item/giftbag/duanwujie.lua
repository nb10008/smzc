function I4800036_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	local tbl1 = {}
	tbl1[1] = {4212001,1,0,958}
	tbl1[2] = {4212002,1,0,42}
	tbl1[3] = {4800032,2,0,1121}
	tbl1[4] = {4090023,1,0,918}
	tbl1[5] = {3020067,1,0,640}
	tbl1[6] = {4530107,1,0,930}
	tbl1[7] = {8170115,1,3,1221}
	tbl1[8] = {8170114,1,3,1011}
	tbl1[9] = {4080087,1,0,1306}
	tbl1[10] = {4090017,1,0,1353}
    tbl1[11] = {4800038,1,0,500}

	local tbl2 = {}
	tbl2[1] = {4212002,1,0,702}
	tbl2[2] = {4212004,1,0,52}
	tbl2[3] = {3020080,1,0,381}
	tbl2[4] = {8170116,1,4,279}
	tbl2[5] = {8170118,1,4,251}
	tbl2[6] = {8170117,1,4,267}
	tbl2[7] = {4800032,3,0,1131}
	tbl2[8] = {4090020,1,0,737}
	tbl2[9] = {4090013,2,0,1101}
	tbl2[10] = {4090010,1,0,341}
	tbl2[11] = {4090011,1,0,431}
	tbl2[12] = {8170115,1,3,972}
	tbl2[13] = {8170114,1,3,1021}
	tbl2[14] = {3020010,1,0,840}
	tbl2[15] = {3030121,1,0,497}
	tbl2[16] = {3030123,1,0,497}
	tbl2[17] = {4800038,1,0,500}

    if TypeID == 4800036 then
	    tbl = tbl1
	else
	    tbl = tbl2
	end

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4800036_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800036, 1, "I4800036_GiftBag")
aux.RegisterItemEvent(4800036, 0, "I4800036_CanUseGiftBag")
aux.RegisterItemEvent(4800037, 1, "I4800036_GiftBag")
aux.RegisterItemEvent(4800037, 0, "I4800036_CanUseGiftBag")

