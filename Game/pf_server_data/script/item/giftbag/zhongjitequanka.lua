function I4530182_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {3070010,1,-1}
	tbl[2] = {3070028,5,-1}
	tbl[3] = {3070057,2,-1}
	tbl[4] = {3070006,1,-1}
	tbl[5] = {4510022,1,-1}
	tbl[6] = {4510023,1,-1}
	tbl[7] = {3050007,20,-1}
	tbl[8] = {3070010,1,-1}
	tbl[9] = {4530183,1,-1}

	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530182_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530182, 1, "I4530182_GiftBag")
aux.RegisterItemEvent(4530182, 0, "I4530182_CanUseGiftBag")



function I4530183_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
	local sex = role.GetRoleSex(TargetID)
	local tbl = {}
	tbl[1] = {3070028,5,-1}
	tbl[2] = {3070011,1,-1}
	tbl[3] = {3070056,40,-1}
	tbl[4] = {3070051,20,-1}
	tbl[5] = {3070018,4,-1}
	tbl[6] = {3010054,1,-1}
	tbl[7] = {3010055,1,-1}
	tbl[8] = {3030120,4,-1}
	tbl[9] = {3030122,4,-1}
	tbl[10] = {3040056,2,-1}
	tbl[11] = {3040021,2,-1}
	tbl[12] = {3050007,20,-1}
	tbl[13] = {3010035,4,-1}
	tbl[14] = {3020010,15,-1}
	tbl[15] = {3020057,15,-1}
	tbl[16] = {3020005,20,-1}
	tbl[17] = {4530184,1,-1}
	tbl[20] = {3070011,1,-1}
	if sex == 1 then
		tbl[18] = {4700175,1,-1}
	else
		tbl[18] = {4700176,1,-1}
	end
	if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        tbl[19] = {8160008, 1, 5}
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
		tbl[19] = {8170008, 1, 5}
    end
	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end



end

function I4530183_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 20) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530183, 1, "I4530183_GiftBag")
aux.RegisterItemEvent(4530183, 0, "I4530183_CanUseGiftBag")



function I4530184_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local tbl = {}
	tbl[1] = {8220001,1,5}
	tbl[2] = {4800039,5,-1}
	tbl[3] = {4820028,300,-1}
	tbl[4] = {3070056,80,-1}
	tbl[5] = {3070051,20,-1}
	tbl[6] = {3030131,4,-1}
	tbl[7] = {3030134,4,-1}
	tbl[8] = {3020069,3,-1}
	tbl[9] = {3020027,1,-1}
	tbl[10] = {4530110,8,-1}
	tbl[11] = {4820081,15,-1}
	tbl[12] = {3070057,2,-1}
	tbl[13] = {3070011,1,-1}
	tbl[14] = {4300008,1,-1}
	tbl[15] = {4090020,1,-1}
	tbl[16] = {4090019,1,-1}
	tbl[17] = {4090021,1,-1}
	tbl[18] = {3040058,3,-1}
	tbl[19] = {3040023,3,-1}
	tbl[20] = {4400031,1,-1}
	tbl[21] = {4221203,1,-1}
	tbl[22] = {4211704,1,-1}
	tbl[23] = {3070011,1,-1}


	for i = 1,#tbl do
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[i][1], tbl[i][2], tbl[i][3], 8, 420)
	end
	local class = role.GetRoleClass(TargetID)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650001, 1, -1, 8, 420)
	elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650101, 1, -1, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650201, 1, -1, 8, 420)
	elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650301, 1, -1, 8, 420)
    end


end

function I4530184_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 24) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530184, 1, "I4530184_GiftBag")
aux.RegisterItemEvent(4530184, 0, "I4530184_CanUseGiftBag")
