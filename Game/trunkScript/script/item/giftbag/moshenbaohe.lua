--魔神宝盒
function I4800246_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	local b = role.GetRoleItemNum(TargetID,4800239)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	if(b < 1) then
		--提示玩家背包空间不足
		bRet = 103
	end
	return bRet, bIgnore
end

function I4800246_GiftBag(MapID, InstanceID, TypeID, TargetID)
    --800个箱子产出金币10000金，400个60代币，300个80代币，300个90代币，2件60级魔神，1件80级魔神，1件90级魔神，2个1000金
    --20%给10金，8%给100金，25%给60代币2个，24%给80代币1-3个，23%给90代币1-3个
    role.RemoveFromRole(MapID, InstanceID, TargetID, 4800239, 1, 101)
	local k = math.random(800)
	if k >= 799 then
	    role.AddRoleSilver(MapID, InstanceID, TargetID, 10000000, 420)
	elseif k == 798 then
	    local tbl = {8050310,8050410,8051310,8051410,8060310,8060410,8061310,8061410,8070310,8070410,8071310,8071410,8080310,8080410,8081310,8081410,8090310,8090410,8091310,8091410,8100310,8100410,8100510,8100610,8110310,8111310,8121310,8122310,8130310,8130410}
	    role.AddRoleItem(MapID, InstanceID, TargetID, tbl[math.random(#tbl)], 1, 5, 8, 420)
    elseif k == 797 then
	    local tbl = {8050309,8050409,8051309,8051409,8060309,8060409,8061309,8061409,8070309,8070409,8071309,8071409,8080309,8080409,8081309,8081409,8090309,8090409,8091309,8091409,8100309,8100409,8100509,8100609,8110309,8111309,8121309,8122309,8130309,8130409}
	    role.AddRoleItem(MapID, InstanceID, TargetID, tbl[math.random(#tbl)], 1, 5, 8, 420)
	elseif k >= 795 then
	    local tbl = {8050307,8050407,8051307,8051407,8060307,8060407,8061307,8061407,8070307,8070407,8071307,8071407,8080307,8080407,8081307,8081407,8090307,8090407,8091307,8091407,8100307,8100407,8100507,8100607,8110307,8111307,8121307,8122307,8130307,8130407}
	    role.AddRoleItem(MapID, InstanceID, TargetID, tbl[math.random(#tbl)], 1, 5, 8, 420)
	else
	    local q = math.random(100)
		if q <= 20 then
		    role.AddRoleSilver(MapID, InstanceID, TargetID, 100000, 420)
		elseif q <= 28 then
		    role.AddRoleSilver(MapID, InstanceID, TargetID, 1000000, 420)
		elseif q <= 53 then
		    role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, 2, -1, 8, 420)
        elseif q <= 77 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, q%3+1, -1, 8, 420)
        elseif q <= 100 then
		    role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, q%3+1, -1, 8, 420)
		end
	end
end

aux.RegisterItemEvent(4800246, 1, "I4800246_GiftBag")
aux.RegisterItemEvent(4800246, 0, "I4800246_CanUseGiftBag")




--上古神装宝盒
function I4800247_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	local b = role.GetRoleItemNum(TargetID,4800239)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	if(b < 2) then
		--提示玩家背包空间不足
		bRet = 103
	end
	return bRet, bIgnore
end

function I4800247_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4800239, 2, 101)
    local k = math.random(200)
	if k == 100 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4820191 + math.random(36), 1, -1, 8, 420)
	elseif k <= 20 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800248, 2, -1, 8, 420)
	else
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4800248, 1, -1, 8, 420)
	end
end

aux.RegisterItemEvent(4800247, 1, "I4800247_GiftBag")
aux.RegisterItemEvent(4800247, 0, "I4800247_CanUseGiftBag")




