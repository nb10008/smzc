function tianlongjinfeng_GiftBag(MapID, InstanceID, TypeID, TargetID)
	
	local tbl = {}
	tbl[4700319]={9750147,9750148,9750149}
	tbl[4700320]={9750150,9750151,9750152}
	tbl[4700321]={9750153,9750154,9750155}
	tbl[4700322]={9750156,9750157,9750158}
	
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID][1], 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID][2], 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID][3], 1, -1, 8, 420)

end

function tianlongjinfeng_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700319, 1, "tianlongjinfeng_GiftBag")
aux.RegisterItemEvent(4700320, 1, "tianlongjinfeng_GiftBag")
aux.RegisterItemEvent(4700321, 1, "tianlongjinfeng_GiftBag")
aux.RegisterItemEvent(4700322, 1, "tianlongjinfeng_GiftBag")
aux.RegisterItemEvent(4700319, 0, "tianlongjinfeng_CanUseGiftBag")
aux.RegisterItemEvent(4700320, 0, "tianlongjinfeng_CanUseGiftBag")
aux.RegisterItemEvent(4700321, 0, "tianlongjinfeng_CanUseGiftBag")
aux.RegisterItemEvent(4700322, 0, "tianlongjinfeng_CanUseGiftBag")
