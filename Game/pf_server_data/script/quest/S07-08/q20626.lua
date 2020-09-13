--20626使用溶血草后（4010154），消除一个玉昙花（4010155），获得一个乾元散--（4010156）。


--可否使用函数
function i4010154_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
		return bRet, bIgnore
	end
    local k = role.GetRoleItemNum(TargetID, 4010155)
    if  k >= 1 then
        return 0, false
    else
        return 32, false
    end
    
end

--使用效果
function i4010154_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    role.RemoveFromRole(MapID, InstanceID, TargetID, 4010155, 1, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4010156, 1, -1, 8, 420)

end

--注册
aux.RegisterItemEvent(4010154, 1, "i4010154_QuestUsable")
aux.RegisterItemEvent(4010154, 0, "i4010154_CanUse")

