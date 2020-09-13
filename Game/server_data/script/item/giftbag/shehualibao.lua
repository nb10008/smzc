function I4530185_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070010,1,-1}
	tbl[2] = {3070028,5,-1}
	tbl[3] = {3070057,2,-1}
	tbl[4] = {3070006,1,-1}
	tbl[5] = {4510022,1,-1}
	tbl[6] = {4510023,1,-1}
	tbl[7] = {3070056,10,-1}
	tbl[8] = {3070010,1,-1}
	tbl[9] = {4530186,1,-1}

	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530185_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530185, 1, "I4530185_GiftBag")
aux.RegisterItemEvent(4530185, 0, "I4530185_CanUseGiftBag")



function I4530186_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	local tbl = {}
	tbl[1] = {3070028,4,-1}
	tbl[2] = {3070011,1,-1}
	tbl[3] = {3070056,20,-1}
	tbl[4] = {3010054,1,-1}
	tbl[5] = {3010055,1,-1}
	tbl[6] = {3030120,2,-1}
	tbl[7] = {3030122,2,-1}
	tbl[8] = {3050007,10,-1}
	tbl[9] = {3010035,1,-1}
	tbl[10] = {3020009,10,-1}
	tbl[11] = {3020057,10,-1}
	tbl[12] = {3020005,15,-1}
	tbl[13] = {4530187,1,-1}
	if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        tbl[14] = {8160004, 1, 5}
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
		tbl[14] = {8170004, 1, 5}
    end
	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530186_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 14) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530186, 1, "I4530186_GiftBag")
aux.RegisterItemEvent(4530186, 0, "I4530186_CanUseGiftBag")



function I4530187_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3050017,30,-1}
	tbl[2] = {3070056,40,-1}
	tbl[3] = {3070051,10,-1}
	tbl[4] = {3030131,2,-1}
	tbl[5] = {3030134,2,-1}
	tbl[6] = {3020010,5,-1}
	tbl[7] = {4530110,3,-1}
	tbl[8] = {4820081,5,-1}
	tbl[9] = {3070057,1,-1}
	tbl[10] = {4800039,2,-1}


	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end


end

function I4530187_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530187, 1, "I4530187_GiftBag")
aux.RegisterItemEvent(4530187, 0, "I4530187_CanUseGiftBag")
