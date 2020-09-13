--新人王特权卡
function I4530229_GiftBag(MapID, InstanceID, TypeID, TargetID)

	local tbl = {}
	tbl[1] = {3070010,1,-1}
	tbl[2] = {3070028,1,-1}
	tbl[3] = {3070057,1,-1}
	tbl[4] = {3070006,1,-1}
	tbl[5] = {4510022,1,-1}
	tbl[6] = {4510023,1,-1}
	tbl[7] = {3050007,5,-1}
	tbl[8] = {4530230,1,-1}


	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end

end

function I4530229_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530229, 1, "I4530229_GiftBag")
aux.RegisterItemEvent(4530229, 0, "I4530229_CanUseGiftBag")



--新人王特权卡
function I4530230_GiftBag(MapID, InstanceID, TypeID, TargetID)

	local tbl = {}
	tbl[1] = {3070028,2,-1}
	tbl[2] = {3070011,1,-1}
	tbl[3] = {3070056,20,-1}
	tbl[4] = {3070051,5,-1}
	tbl[5] = {3010056,1,-1}
	tbl[6] = {3010060,1,-1}
	tbl[7] = {3030121,2,-1}
	tbl[8] = {3030123,2,-1}
	tbl[9] = {3050007,10,-1}
	tbl[10] = {3010035,1,-1}
	tbl[11] = {3070068,5,-1}
	tbl[12] = {3020057,5,-1}
	tbl[13] = {3020005,20,-1}
	tbl[14] = {4530231,1,-1}



	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end

end

function I4530230_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 14) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530230, 1, "I4530230_GiftBag")
aux.RegisterItemEvent(4530230, 0, "I4530230_CanUseGiftBag")



--新人王特权卡
function I4530231_GiftBag(MapID, InstanceID, TypeID, TargetID)

	local tbl = {}
	tbl[1] = {3070056,30,-1}
	tbl[2] = {3070051,10,-1}
	tbl[3] = {3030131,1,-1}
	tbl[4] = {3030134,1,-1}
	tbl[5] = {3070068,7,-1}
	tbl[6] = {3020069,1,-1}
	tbl[7] = {4530110,2,-1}
	tbl[8] = {4820081,5,-1}
	tbl[9] = {3070057,1,-1}
	tbl[10] = {3070011,1,-1}
	tbl[11] = {4300006,1,-1}
	tbl[12] = {3020057,7,-1}
	tbl[13] = {3070028,5,-1}




	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end

end

function I4530231_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 13) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530231, 1, "I4530231_GiftBag")
aux.RegisterItemEvent(4530231, 0, "I4530231_CanUseGiftBag")
