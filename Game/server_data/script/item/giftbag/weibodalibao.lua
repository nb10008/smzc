function I4800290_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070011,1,-1}
	tbl[2] = {3070028,5,-2}
	tbl[3] = {3070022,5,-3}
	tbl[4] = {3070006,1,-4}
	tbl[5] = {4530110,20,-5}
	tbl[6] = {3070018,2,-6}
	tbl[7] = {3020081,2,-7}
	tbl[8] = {4300006,1,-8}
	tbl[9] = {4820081,10,-9}
	tbl[10] = {3070011,1,-1}
	tbl[11] = {3070011,1,-1}



	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4800290_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800290, 1, "I4800290_GiftBag")
aux.RegisterItemEvent(4800290, 0, "I4800290_CanUseGiftBag")



function I4800291_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {4800292,1,-1}
	tbl[2] = {3070051,10,-1}
	tbl[3] = {3070056,10,-1}
	tbl[4] = {3051607,1,-1}
	tbl[5] = {3051608,1,-1}
	tbl[6] = {3051609,1,-1}
	tbl[7] = {3070060,20,-1}
	tbl[8] = {3070091,2,-1}



	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4800291_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800291, 1, "I4800291_GiftBag")
aux.RegisterItemEvent(4800291, 0, "I4800291_CanUseGiftBag")






function I4800292_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(5)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {4221801,1,-1}
	tbl[2] = {4221901,1,-1}
	tbl[3] = {4221501,1,-1}
	tbl[4] = {4222001,1,-1}
	tbl[5] = {4222101,1,-1}
	for i = 1,#tbl do
	    temp = temp + 1
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)



end

function I4800292_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800292, 1, "I4800292_GiftBag")
aux.RegisterItemEvent(4800292, 0, "I4800292_CanUseGiftBag")


function I4700295_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID,8161011, 1, 6, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8171011, 1, 6, 8, 420)
	end


end

function I4700295_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700295, 1, "I4700295_GiftBag")
aux.RegisterItemEvent(4700295, 0, "I4700295_CanUseGiftBag")

