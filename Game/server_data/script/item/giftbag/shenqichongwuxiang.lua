--神奇宠物箱
function I4510022_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]= {4210102,1,-1,500}
	tbl[2]= {4210202,1,-1,500}
	tbl[3]= {4210301,1,-1,500}
	tbl[4]= {4210401,1,-1,500}
	tbl[5]= {4210501,1,-1,500}
	tbl[6]= {4210601,1,-1,500}
	tbl[7]= {4210701,1,-1,500}
	tbl[8]= {4210801,1,-1,500}
	tbl[9]= {4210901,1,-1,500}
	tbl[10]= {4211001,1,-1,500}
	tbl[11]= {4211102,1,-1,500}
	tbl[12]= {4211202,1,-1,500}
	tbl[13]= {4211302,1,-1,500}
	tbl[14]= {4211402,1,-1,500}
	tbl[15]= {4211502,1,-1,500}
	tbl[16]= {4211602,1,-1,500}
	tbl[17]= {4211702,1,-1,500}
	tbl[18]= {4211802,1,-1,500}
	tbl[19]= {4211902,1,-1,500}
	tbl[20]= {4212002,1,-1,500}

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4510022_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4510022, 1, "I4510022_GiftBag")
aux.RegisterItemEvent(4510022, 0, "I4510022_CanUseGiftBag")


--神奇坐骑箱
function I4510023_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]= {4220102,1,-1,570}
	tbl[2]= {4220202,1,-1,550}
	tbl[3]= {4220205,1,-1,550}
	tbl[4]= {4220302,1,-1,570}
	tbl[5]= {4220402,1,-1,550}
	tbl[6]= {4220502,1,-1,550}
	tbl[7]= {4220602,1,-1,550}
	tbl[8]= {4220605,1,-1,550}
	tbl[9]= {4220802,1,-1,550}
	tbl[10]= {4220902,1,-1,570}
	tbl[11]= {4221002,1,-1,570}
	tbl[12]= {4221102,1,-1,550}
	tbl[13]= {4221202,1,-1,550}
	tbl[14]= {4221206,1,-1,570}
	tbl[15]= {4221302,1,-1,550}
	tbl[16]= {4221402,1,-1,550}
	tbl[17]= {4221502,1,-1,550}
	tbl[18]= {4221602,1,-1,550}


	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4510023_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4510023, 1, "I4510023_GiftBag")
aux.RegisterItemEvent(4510023, 0, "I4510023_CanUseGiftBag")

