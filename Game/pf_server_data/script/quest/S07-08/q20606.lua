--20606烟花祭灵
--需求1：使用物品烟花[4010142]后得到物品烟花碎屑[4010143]


--接去任务
function i4010142_QuestUsable(MapID, InstanceID, TypeID, TargetID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4010143, 1, 0, 0, 300)


end

function i4010142_CanUse(MapID, InstanceID, TypeID, TargetID)
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
aux.RegisterItemEvent(4010142, 1, "i4010142_QuestUsable")
aux.RegisterItemEvent(4010142, 0, "i4010142_CanUse")


