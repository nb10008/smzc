function I4700287_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(1000000)
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={4700229,1,-1,77000}
	tbl[2]={4700230,1,-1,69000}
	tbl[3]={4820061,15,-1,59000}
	tbl[4]={4510020,6,-1,49000}
	tbl[5]={4820239,1,-1,44000}
	tbl[6]={4400052,1,-1,49000}
	tbl[7]={4820047,2,-1,59000}
	tbl[8]={4820046,2,-1,59000}
	tbl[9]={4500002,8,-1,64000}
	tbl[10]={3070051,8,-1,64000}
	tbl[11]={5600003,1,-1,49000}
	tbl[12]={4400053,1,-1,39000}
	tbl[13]={4530109,3,-1,59000}
	tbl[14]={3030316,1,-1,15000}
	tbl[15]={3030336,1,-1,15000}
	tbl[16]={4820035,1,-1,25000}
	tbl[17]={4700231,1,-1,46000}
	tbl[18]={4820256,1,-1,25000}
	tbl[19]={5600008,1,-1,19000}
	tbl[20]={5600006,1,-1,29000}
	tbl[21]={3070011,1,-1,49000}




	tbl[22]={3020035,1,-1,400}
	tbl[23]={3020030,1,-1,20000}
	tbl[24]={4820179,1,-1,3000}
	tbl[25]={3020089,1,-1,1000}
	tbl[26]={3020091,1,-1,400}
	tbl[27]={3020092,1,-1,300}
	tbl[28]={4222304,1,-1,400}
	tbl[29]={3090035,1,-1,2000}
	tbl[30]={4212008,1,-1,400}
	tbl[31]={3030318,1,-1,3000}
	tbl[32]={3030320,1,-1,500}
	tbl[33]={3030338,1,-1,3000}
	tbl[34]={3030340,1,-1,500}
	tbl[35]={4820180,1,-1,2000}
	tbl[36]={3020042,1,-1,2}
	tbl[37]={3020037,1,-1,98}




	for i = 1,37 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	local sex = role.GetRoleSex(TargetID)
	if index == 29 then
		if sex == 1 then
			tbl[29][1] = 3090035
		else
			tbl[29][1] = 3090036
		end
	end

		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700287_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700287, 1, "I4700287_GiftBag")
aux.RegisterItemEvent(4700287, 0, "I4700287_CanUseGiftBag")
