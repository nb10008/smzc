

--Éñ±ø
function I4820243_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local Equip = {8019004,8019004,8050914,8060914,8070914,8080914,8090914,8101327,8110920,8110920,8121912,8130923}
	local Stone = {3030006,3030006,3030034,3030034,3030034,3030042,3030042,3030042,3030006,3030006,3030006,3030006}
	local Shengyin = {3040023,3040023,3040058,3040058,3040058,3040058,3040058,3040058,3040023,3040023,3040023,3040023}
    local function ADDItem(MapID, InstanceID, TargetID,index)
		local high,low = role.AddRoleItem(MapID, InstanceID, TargetID, Equip[index], 1, 6, 8, 420)
		role.SetEquipStrengthLevel(TargetID,high,low,20)
		role.SetGrowEquipLevel(TargetID,high,low,3074)
		for i=1,5 do
			role.DigHoleOnEquip(TargetID,high,low)
			role.PutStoneOnEquip(TargetID,high,low,Stone[index])
		end
		for i=1,10 do
			role.SetEquipAdditionalAbility(TargetID,high,low,Shengyin[index])
		end
	end
    for i =1,12 do
	    ADDItem(MapID, InstanceID, TargetID,i)
	end
end


function I4820243_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820243, 1, "I4820243_GiftBag")
aux.RegisterItemEvent(4820243, 0, "I4820243_CanUseGiftBag")


--Ðþôá
function I4820244_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local Equip = {8029004,8029004,8050915,8060915,8070915,8080915,8090915,8101328,8110921,8110921,8121913,8130924}
	local Stone = {3030006,3030006,3030034,3030034,3030034,3030042,3030042,3030042,3030006,3030006,3030006,3030006}
	local Shengyin = {3040023,3040023,3040058,3040058,3040058,3040058,3040058,3040058,3040023,3040023,3040023,3040023}
    local function ADDItem(MapID, InstanceID, TargetID,index)
		local high,low = role.AddRoleItem(MapID, InstanceID, TargetID, Equip[index], 1, 6, 8, 420)
		role.SetEquipStrengthLevel(TargetID,high,low,20)
		role.SetGrowEquipLevel(TargetID,high,low,3074)
		for i=1,5 do
			role.DigHoleOnEquip(TargetID,high,low)
			role.PutStoneOnEquip(TargetID,high,low,Stone[index])
		end
		for i=1,10 do
			role.SetEquipAdditionalAbility(TargetID,high,low,Shengyin[index])
		end
	end
    for i =1,12 do
	    ADDItem(MapID, InstanceID, TargetID,i)
	end
end


function I4820244_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820244, 1, "I4820244_GiftBag")
aux.RegisterItemEvent(4820244, 0, "I4820244_CanUseGiftBag")

--Ç¬À¤
function I4820245_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local Equip = {8049004,8049004,8051914,8061914,8071914,8081914,8091914,8101329,8111920,8111920,8122912,8130925}
	local Stone = {3030014,3030014,3030034,3030034,3030034,3030042,3030042,3030042,3030014,3030014,3030014,3030014}
	local Shengyin = {3040023,3040023,3040058,3040058,3040058,3040058,3040058,3040058,3040023,3040023,3040023,3040023}
    local function ADDItem(MapID, InstanceID, TargetID,index)
		local high,low = role.AddRoleItem(MapID, InstanceID, TargetID, Equip[index], 1, 6, 8, 420)
		role.SetEquipStrengthLevel(TargetID,high,low,20)
		role.SetGrowEquipLevel(TargetID,high,low,3074)
		for i=1,5 do
			role.DigHoleOnEquip(TargetID,high,low)
			role.PutStoneOnEquip(TargetID,high,low,Stone[index])
		end
		for i=1,10 do
			role.SetEquipAdditionalAbility(TargetID,high,low,Shengyin[index])
		end
	end
    for i =1,12 do
	    ADDItem(MapID, InstanceID, TargetID,i)
	end
end


function I4820245_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820245, 1, "I4820245_GiftBag")
aux.RegisterItemEvent(4820245, 0, "I4820245_CanUseGiftBag")

--ÏÉÒô
function I4820246_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local Equip = {8039004,8039004,8051915,8061915,8071915,8081915,8091915,8101330,8111921,8111921,8122913,8130926}
	local Stone = {3030014,3030014,3030034,3030034,3030034,3030042,3030042,3030042,3030014,3030014,3030014,3030014}
	local Shengyin = {3040023,3040023,3040058,3040058,3040058,3040058,3040058,3040058,3040023,3040023,3040023,3040023}
    local function ADDItem(MapID, InstanceID, TargetID,index)
		local high,low = role.AddRoleItem(MapID, InstanceID, TargetID, Equip[index], 1, 6, 8, 420)
		role.SetEquipStrengthLevel(TargetID,high,low,20)
		role.SetGrowEquipLevel(TargetID,high,low,3074)
		for i=1,5 do
			role.DigHoleOnEquip(TargetID,high,low)
			role.PutStoneOnEquip(TargetID,high,low,Stone[index])
		end
		for i=1,10 do
			role.SetEquipAdditionalAbility(TargetID,high,low,Shengyin[index])
		end
	end
    for i =1,12 do
	    ADDItem(MapID, InstanceID, TargetID,i)
	end
end


function I4820246_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820246, 1, "I4820246_GiftBag")
aux.RegisterItemEvent(4820246, 0, "I4820246_CanUseGiftBag")

















