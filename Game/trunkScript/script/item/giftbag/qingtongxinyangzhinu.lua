function I4700139_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]= {3020082,1,1,1200}
	tbl[2]= {3020091,1,1,5}
	tbl[3]= {3020092,1,1,1}
	tbl[4]= {4090026,1,1,220}
	tbl[5]= {4080097,1,1,1300}
	tbl[6]= {3070019,2,1,1000}
	tbl[7]= {4530103,1,1,800}
	tbl[8]= {4530104,1,1,600}
	tbl[9]= {4700113,1,1,40}
	tbl[10]= {4700136,1,1,54}
	tbl[11]= {4500002,10,1,1000}
	tbl[12]= {3070052,10,1,1000}
	tbl[13]= {5600008,1,1,1000}
	tbl[14]= {4800132,1,1,900}
	tbl[15]= {3070012,1,1,880}

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700139_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700139, 1, "I4700139_GiftBag")
aux.RegisterItemEvent(4700139, 0, "I4700139_CanUseGiftBag")
