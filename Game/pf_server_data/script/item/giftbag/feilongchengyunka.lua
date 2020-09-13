--·ÉÁú³ËÔÆ¿¨
function I4530215_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530216, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,3070057,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070006,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510023,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4300006,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010054,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010054,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010055,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010055,1, -1, 8, 420)

end

function I4530215_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530215, 1, "I4530215_GiftBag")
aux.RegisterItemEvent(4530215, 0, "I4530215_CanUseGiftBag")

function I4530216_GiftBag(MapID, InstanceID, TypeID, TargetID)



	role.AddRoleItem(MapID, InstanceID, TargetID,3070051,25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081,45, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010035,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3050007,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510023,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020011,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070068,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056,30, -1, 8, 420)

end

function I4530216_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 15) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530216, 1, "I4530216_GiftBag")
aux.RegisterItemEvent(4530216, 0, "I4530216_CanUseGiftBag")




