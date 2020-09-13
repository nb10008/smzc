function I4800169_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举

	local tbl = {}
	tbl[1] = {8220398,1,6}
	tbl[2] = {4820046,999,-1}
	tbl[3] = {4820047,999,-1}
	tbl[4] = {4820045,100,-1}
	tbl[5] = {4820083,999,-1}
	tbl[6] = {4820083,999,-1}
	tbl[7] = {4820104,1,-1}
	tbl[8] = {4820141,20,-1}
	tbl[9] = {4820161,20,-1}
	tbl[10] = {4820171,999,-1}
	tbl[11] = {4820171,999,-1}
	tbl[12] = {4820102,30,-1}
	tbl[13] = {4820103,30,-1}
	tbl[14] = {4820048,999,-1}
	tbl[15] = {4820048,999,-1}
	tbl[16] = {3060061,5,-1}
	tbl[17] = {3030118,20,-1}
	tbl[18] = {3070070,50,-1}
	tbl[19] = {3020037,2,-1}
	tbl[20] = {3020017,15,-1}
	tbl[21] = {4820177,999,-1}
	tbl[22] = {4820178,999,-1}

	for i = 1,#tbl do
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end

    local class = role.GetRoleClass(TargetID)

    if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8010017, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8020017, 1, 4, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8040017, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8030017, 1, 4, 8, 420)
    end

end

function I4800169_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 24) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800169, 1, "I4800169_GiftBag")
aux.RegisterItemEvent(4800169, 0, "I4800169_CanUseGiftBag")





function I4800170_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070056,999,-1}
	tbl[2] = {3070056,999,-1}
	tbl[3] = {4820038,999,-1}
	tbl[4] = {4820038,999,-1}
	tbl[5] = {4820039,999,-1}
	tbl[6] = {4820039,999,-1}
	tbl[7] = {4820078,999,-1}
	tbl[8] = {4820078,999,-1}
	tbl[9] = {4820079,999,-1}
	tbl[10] = {4820079,999,-1}
	tbl[11] = {3020084,99,-1}
	tbl[12] = {3020087,99,-1}
	tbl[13] = {3020090,99,-1}
	tbl[14] = {3070070,999,-1}
	tbl[15] = {4800171,1,-1}
	tbl[16] = {3010021,1,-1}
	tbl[17] = {3010021,1,-1}
	tbl[18] = {3010021,1,-1}
	tbl[19] = {3010021,1,-1}
	tbl[20] = {3010021,1,-1}
	tbl[21] = {3010021,1,-1}
	tbl[22] = {3010021,1,-1}
	tbl[23] = {3010021,1,-1}
	tbl[24] = {3010021,1,-1}
	tbl[25] = {3010026,1,-1}
	tbl[26] = {3010026,1,-1}
	tbl[27] = {3010026,1,-1}
	tbl[28] = {3010026,1,-1}
	tbl[29] = {3010026,1,-1}
	tbl[30] = {3010026,1,-1}
	tbl[31] = {3010026,1,-1}
	tbl[32] = {3010026,1,-1}
	tbl[33] = {3010026,1,-1}
	tbl[34] = {3010026,1,-1}
	tbl[35] = {4090022,999,-1}
	tbl[36] = {4090022,999,-1}
	tbl[37] = {4090026,20,-1}
	tbl[38] = {4090029,15,-1}
	tbl[39] = {3010021,1,-1}

	for i = 1,#tbl do
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end
end

function I4800170_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 39) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800170, 1, "I4800170_GiftBag")
aux.RegisterItemEvent(4800170, 0, "I4800170_CanUseGiftBag")




function I4800171_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 8023009, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8023009, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8028003, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8028003, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8050409, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8050903, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8060409, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8060903, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8070409, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8070903, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8080409, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8080903, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8090409, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8090903, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8100409, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8101305, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8110309, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8110309, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8110903, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8110903, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8121309, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8121903, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8130309, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8130905, 1, 5, 8, 420)



end

function I4800171_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 25) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800171, 1, "I4800171_GiftBag")
aux.RegisterItemEvent(4800171, 0, "I4800171_CanUseGiftBag")
