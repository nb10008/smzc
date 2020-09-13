function I4700276_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local gold = math.random(1,99)

	role.AddRoleSilver(MapID, InstanceID, TargetID, gold*10000, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 10, -1, 8, 420)


end

function I4700276_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40

		--判断金钱是否超出上限
    elseif(freegold > 9999009999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306002)--需要至少x格背包空间才可继续操作
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700276, 1, "I4700276_GiftBag")
aux.RegisterItemEvent(4700276, 0, "I4700276_CanUseGiftBag")
