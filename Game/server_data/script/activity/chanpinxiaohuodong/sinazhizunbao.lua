
function I4830038_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 100, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4222003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211504, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800287, 120, -1, 8, 420)
	if TypeID == 4830033 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8100026, 1, -1, 8, 420)
	elseif TypeID == 4830034 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8121026, 1, -1, 8, 420)
	elseif TypeID == 4830035 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8130026, 1, -1, 8, 420)
	elseif TypeID == 4830036 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8112006, 1, -1, 8, 420)
	elseif TypeID == 4830037 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8112007, 1, -1, 8, 420)
	elseif TypeID == 4830038 then
		
		if role.GetRoleSex(TargetID) == 1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4210901, 1, -1, 8, 420)
		elseif role.GetRoleSex(TargetID) == 0 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4210801, 1, -1, 8, 420)
		end
		role.AddRoleItem(MapID, InstanceID, TargetID, 8090026, 1, -1, 8, 420)
	elseif TypeID == 4830039 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8050026, 1, -1, 8, 420)
	elseif TypeID == 4830041 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8070026, 1, -1, 8, 420)
	end
end

function I4830038_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if TypeID ~= 4830038 then
		if(FreeSize < 11) then
			--提示玩家背包空间不足
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)
			msg.AddMsgEvent(MsgID, 9, 10)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 40
		end
	else
		if(FreeSize < 12) then
			--提示玩家背包空间不足
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)
			msg.AddMsgEvent(MsgID, 9, 11)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 40
		end
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830033, 1, "I4830038_GiftBag")
aux.RegisterItemEvent(4830034, 1, "I4830038_GiftBag")
aux.RegisterItemEvent(4830035, 1, "I4830038_GiftBag")
aux.RegisterItemEvent(4830036, 1, "I4830038_GiftBag")
aux.RegisterItemEvent(4830037, 1, "I4830038_GiftBag")
aux.RegisterItemEvent(4830038, 1, "I4830038_GiftBag")
aux.RegisterItemEvent(4830039, 1, "I4830038_GiftBag")
aux.RegisterItemEvent(4830041, 1, "I4830038_GiftBag")
aux.RegisterItemEvent(4830033, 0, "I4830038_CanUseGiftBag")
aux.RegisterItemEvent(4830034, 0, "I4830038_CanUseGiftBag")
aux.RegisterItemEvent(4830035, 0, "I4830038_CanUseGiftBag")
aux.RegisterItemEvent(4830036, 0, "I4830038_CanUseGiftBag")
aux.RegisterItemEvent(4830037, 0, "I4830038_CanUseGiftBag")
aux.RegisterItemEvent(4830038, 0, "I4830038_CanUseGiftBag")
aux.RegisterItemEvent(4830039, 0, "I4830038_CanUseGiftBag")
aux.RegisterItemEvent(4830041, 0, "I4830038_CanUseGiftBag")

function I4830040_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030140, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030141, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 100, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800039, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800041, 10, -1, 8, 420)

end
function I4830040_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 21) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 21)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830040, 1, "I4830040_GiftBag")
aux.RegisterItemEvent(4830040, 0, "I4830040_CanUseGiftBag")

function I4830042_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 1, -1, 8, 422)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 423)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 424)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008, 1, -1, 8, 425)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830043, 1, -1, 8, 439)

end
function I4830042_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 7) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 7)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830042, 1, "I4830042_GiftBag")
aux.RegisterItemEvent(4830042, 0, "I4830042_CanUseGiftBag")

function I4830043_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 426)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 1, -1, 8, 427)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 428)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 35, -1, 8, 429)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020114, 2, -1, 8, 430)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 50, -1, 8, 431)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 5, -1, 8, 432)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030129, 2, -1, 8, 433)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030132, 2, -1, 8, 434)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030120, 2, -1, 8, 435)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030122, 2, -1, 8, 436)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 10, -1, 8, 437)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 5, -1, 8, 438)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020048, 10, -1, 8, 439)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830044, 1, -1, 8, 439)

end
function I4830043_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 15) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 15)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830043, 1, "I4830043_GiftBag")
aux.RegisterItemEvent(4830043, 0, "I4830043_CanUseGiftBag")

function I4830044_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 60, -1, 8, 440)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 10, -1, 8, 441)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 2, -1, 8, 442)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 2, -1, 8, 443)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030140, 2, -1, 8, 444)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030141, 2, -1, 8, 445)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 446)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070068, 10, -1, 8, 447)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 2, -1, 8, 448)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 50, -1, 8, 449)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020049, 10, -1, 8, 450)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820047, 10, -1, 8, 451)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820046, 10, -1, 8, 452)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 5, -1, 8, 453)

end
function I4830044_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 14) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 14)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830044, 1, "I4830044_GiftBag")
aux.RegisterItemEvent(4830044, 0, "I4830044_CanUseGiftBag")
