function I4800231_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	local tbl = {}
	tbl[1] = {4820029,999,-1}
	tbl[2] = {4820029,999,-1}
	tbl[3] = {4090022,999,-1}
	tbl[4] = {4090027,50,-1}
	tbl[5] = {3040060,99,-1}
	tbl[6] = {3040060,99,-1}
	tbl[7] = {3040060,99,-1}
	tbl[8] = {4999005,400,-1}
	tbl[9] = {3020037,5,-1}
	tbl[10] = {3030050,10,-1}
	tbl[11] = {3030056,10,-1}
	tbl[12] = {3030233,20,-1}
	tbl[13] = {3030118,80,-1}
	tbl[14] = {3030119,80,-1}
	tbl[15] = {3020052,99,-1}
	tbl[16] = {3020052,99,-1}
	tbl[17] = {3020052,99,-1}
	tbl[18] = {3020052,99,-1}
	tbl[19] = {4820181,1,-1}
	tbl[20] = {3020013,99,-1}
	tbl[21] = {4820238,100,-1}
	tbl[22] = {4820049,100,-1}
	tbl[23] = {4820255,500,-1}
	tbl[24] = {4820251,150,-1}
	tbl[25] = {4700230,25,-1}
	tbl[26] = {4700231,10,-1}
	tbl[27] = {4820048,999,-1}
	tbl[34] = {3020013,99,-1}
	if class==2 or class==6 or class==7 then
		tbl[28] = {4090028,60,-1}
		tbl[29] = {8013010,1,5}
		tbl[30] = {8013010,1,5}
		tbl[31] = {3040005,99,-1}
		tbl[32] = {3040005,99,-1}
		tbl[33] = {3030310,20,-1}


	elseif class==3 or class==8 or class==9 then
		tbl[28] = {4090029,60,-1}
		tbl[29] = {8023010,1,5}
		tbl[30] = {8023010,1,5}
		tbl[31] = {3040010,99,-1}
		tbl[32] = {3040010,99,-1}
		tbl[33] = {3030310,20,-1}

    elseif class==4 or class==10 or class==11 then
		tbl[28] = {4090030,60,-1}
		tbl[29] = {8043010,1,5}
		tbl[30] = {8043010,1,5}
		tbl[31] = {3040015,99,-1}
		tbl[32] = {3040015,99,-1}
		tbl[33] = {3030330,20,-1}

	elseif class==5 or class==12 or class==13 then
		tbl[28] = {4090031,60,-1}
		tbl[29] = {8033010,1,5}
		tbl[30] = {8033010,1,5}
		tbl[31] = {3040020,99,-1}
		tbl[32] = {3040020,99,-1}
		tbl[33] = {3030330,20,-1}
    end

	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end

end

function I4800231_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 34) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800231, 1, "I4800231_GiftBag")
aux.RegisterItemEvent(4800231, 0, "I4800231_CanUseGiftBag")
