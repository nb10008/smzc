--20645
--需求1：使用物品信号烟花[4010163]后得到燃尽的烟花[4010164]


--接去任务
function i4010163_QuestUsable(MapID, InstanceID, TypeID, TargetID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4010164, 1, 0, 0, 300)


end

function i4010163_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
		return bRet, bIgnore
	end
end


--注册
aux.RegisterItemEvent(4010163, 1, "i4010163_QuestUsable")
aux.RegisterItemEvent(4010163, 0, "i4010163_CanUse")


