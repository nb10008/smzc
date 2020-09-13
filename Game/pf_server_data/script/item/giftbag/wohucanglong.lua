--ÎÔ»¢²ØÁú¿¨
function I4530219_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530220, 1, -1, 8, 420)

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

function I4530219_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530219, 1, "I4530219_GiftBag")
aux.RegisterItemEvent(4530219, 0, "I4530219_CanUseGiftBag")

function I4530220_GiftBag(MapID, InstanceID, TypeID, TargetID)



	role.AddRoleItem(MapID, InstanceID, TargetID,3050007,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070051,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010035,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070068,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056,30, -1, 8, 420)



end

function I4530220_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530220, 1, "I4530220_GiftBag")
aux.RegisterItemEvent(4530220, 0, "I4530220_CanUseGiftBag")


