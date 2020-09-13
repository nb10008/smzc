--0	1	2	3	4	5	6	7	8	9	10	11	12
--新手	神兵	玄翎	乾坤	仙音	王者	神甲	幻矢	占星	雷神	破魔	神侍	天仪
function shenmozaowu_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830048, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830049, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830050, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830051, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830052, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830053, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830054, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830055, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830056, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830057, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830058, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830059, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830060, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830061, 0, "shenmozaowu_CanUseGiftBag")
aux.RegisterItemEvent(4830062, 0, "shenmozaowu_CanUseGiftBag")

function shenmozaowu_GiftBag(MapID, InstanceID, TypeID, TargetID)
	
	local tbl = {}
	tbl[4830048]={4830051,4830054,4830057,4830060}
	tbl[4830049]={4830052,4830055,4830058,4830061}
	tbl[4830050]={4830053,4830056,4830059,4830062}
	local class = role.GetRoleClass(TargetID)
	if class == 2 or class == 6 or class == 7 or class == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID][1], 1, -1, 8, 420)
	elseif class == 3 or class == 8 or class == 9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID][2], 1, -1, 8, 420)
	elseif class == 4 or class == 10 or class == 11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID][3], 1, -1, 8, 420)
	elseif class == 5 or class == 12 or class == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID][4], 1, -1, 8, 420)
	end

end
aux.RegisterItemEvent(4830048, 1, "shenmozaowu_GiftBag")
aux.RegisterItemEvent(4830049, 1, "shenmozaowu_GiftBag")
aux.RegisterItemEvent(4830050, 1, "shenmozaowu_GiftBag")

function I4830051_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8013007,35}
	tbl[2]={8023007,35}
	tbl[3]={8050307,60}
	tbl[4]={8060307,60}
	tbl[5]={8070307,60}
	tbl[6]={8080307,60}
	tbl[7]={8090307,60}
	tbl[8]={8100307,60}
	tbl[9]={8110307,55}
	tbl[10]={8121307,55}
	tbl[11]={8130307,55}
	tbl[12]={8017007,30}
	tbl[13]={8027007,30}
	tbl[14]={8050807,40}
	tbl[15]={8060807,40}
	tbl[16]={8070807,40}
	tbl[17]={8080807,40}
	tbl[18]={8090807,40}
	tbl[19]={8101207,40}
	tbl[20]={8110807,35}
	tbl[21]={8121807,35}
	tbl[22]={8130807,35}

	for i = 1,22 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830051, 1, "I4830051_GiftBag")
function I4830054_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8013007,35}
	tbl[2]={8023007,35}
	tbl[3]={8050407,60}
	tbl[4]={8060407,60}
	tbl[5]={8070407,60}
	tbl[6]={8080407,60}
	tbl[7]={8090407,60}
	tbl[8]={8100407,60}
	tbl[9]={8110307,55}
	tbl[10]={8121307,55}
	tbl[11]={8130307,55}
	tbl[12]={8017007,30}
	tbl[13]={8027007,30}
	tbl[14]={8050811,40}
	tbl[15]={8060811,40}
	tbl[16]={8070811,40}
	tbl[17]={8080811,40}
	tbl[18]={8090811,40}
	tbl[19]={8101211,40}
	tbl[20]={8110807,35}
	tbl[21]={8121807,35}
	tbl[22]={8130807,35}
	for i = 1,22 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830054, 1, "I4830054_GiftBag")
function I4830057_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8033007,35}
	tbl[2]={8043007,35}
	tbl[3]={8051307,60}
	tbl[4]={8061307,60}
	tbl[5]={8071307,60}
	tbl[6]={8081307,60}
	tbl[7]={8091307,60}
	tbl[8]={8100507,60}
	tbl[9]={8111307,55}
	tbl[10]={8122307,55}
	tbl[11]={8130407,55}
	tbl[12]={8037007,30}
	tbl[13]={8047007,30}
	tbl[14]={8051807,40}
	tbl[15]={8061807,40}
	tbl[16]={8071807,40}
	tbl[17]={8081807,40}
	tbl[18]={8091807,40}
	tbl[19]={8101215,40}
	tbl[20]={8111807,35}
	tbl[21]={8122807,35}
	tbl[22]={8130811,35}
	for i = 1,22 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830057, 1, "I4830057_GiftBag")
function I4830060_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8033007,35}
	tbl[2]={8043007,35}
	tbl[3]={8051407,60}
	tbl[4]={8061407,60}
	tbl[5]={8071407,60}
	tbl[6]={8081407,60}
	tbl[7]={8091407,60}
	tbl[8]={8100607,60}
	tbl[9]={8111307,55}
	tbl[10]={8122307,55}
	tbl[11]={8130407,55}
	tbl[12]={8037007,30}
	tbl[13]={8047007,30}
	tbl[14]={8051811,40}
	tbl[15]={8061811,40}
	tbl[16]={8071811,40}
	tbl[17]={8081811,40}
	tbl[18]={8091811,40}
	tbl[19]={8101219,40}
	tbl[20]={8111807,35}
	tbl[21]={8122807,35}
	tbl[22]={8130811,35}

	for i = 1,22 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830060, 1, "I4830060_GiftBag")
function I4830052_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8013009,35}
	tbl[2]={8023009,35}
	tbl[3]={8050309,60}
	tbl[4]={8060309,60}
	tbl[5]={8070309,60}
	tbl[6]={8080309,60}
	tbl[7]={8090309,60}
	tbl[8]={8100309,60}
	tbl[9]={8110309,55}
	tbl[10]={8121309,55}
	tbl[11]={8130309,55}
	tbl[12]={8018003,30}
	tbl[13]={8028003,30}
	tbl[14]={8050903,40}
	tbl[15]={8060903,40}
	tbl[16]={8070903,40}
	tbl[17]={8080903,40}
	tbl[18]={8090903,40}
	tbl[19]={8101305,40}
	tbl[20]={8110903,35}
	tbl[21]={8121903,35}
	tbl[22]={8130905,35}

	for i = 1,22 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830052, 1, "I4830052_GiftBag")
function I4830055_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8013009,35}
	tbl[2]={8023009,35}
	tbl[3]={8050409,60}
	tbl[4]={8060409,60}
	tbl[5]={8070409,60}
	tbl[6]={8080409,60}
	tbl[7]={8090409,60}
	tbl[8]={8100409,60}
	tbl[9]={8110309,55}
	tbl[10]={8121309,55}
	tbl[11]={8130309,55}
	tbl[12]={8018003,30}
	tbl[13]={8028003,30}
	tbl[14]={8050903,40}
	tbl[15]={8060903,40}
	tbl[16]={8070903,40}
	tbl[17]={8080903,40}
	tbl[18]={8090903,40}
	tbl[19]={8101305,40}
	tbl[20]={8110903,35}
	tbl[21]={8121903,35}
	tbl[22]={8130905,35}

	for i = 1,22 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830055, 1, "I4830055_GiftBag")
function I4830058_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8033009,35}
	tbl[2]={8043009,35}
	tbl[3]={8051309,60}
	tbl[4]={8061309,60}
	tbl[5]={8071309,60}
	tbl[6]={8081309,60}
	tbl[7]={8091309,60}
	tbl[8]={8100509,60}
	tbl[9]={8111309,55}
	tbl[10]={8122309,55}
	tbl[11]={8130409,55}
	tbl[12]={8038003,30}
	tbl[13]={8048003,30}
	tbl[14]={8051903,40}
	tbl[15]={8061903,40}
	tbl[16]={8071903,40}
	tbl[17]={8081903,40}
	tbl[18]={8091903,40}
	tbl[19]={8101306,40}
	tbl[20]={8111903,35}
	tbl[21]={8122903,35}
	tbl[22]={8130906,35}


	for i = 1,22 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830058, 1, "I4830058_GiftBag")
function I4830061_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(1000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8033009,35}
	tbl[2]={8043009,35}
	tbl[3]={8051409,60}
	tbl[4]={8061409,60}
	tbl[5]={8071409,60}
	tbl[6]={8081409,60}
	tbl[7]={8091409,60}
	tbl[8]={8100609,60}
	tbl[9]={8111309,55}
	tbl[10]={8122309,55}
	tbl[11]={8130409,55}
	tbl[12]={8038003,30}
	tbl[13]={8048003,30}
	tbl[14]={8051903,40}
	tbl[15]={8061903,40}
	tbl[16]={8071903,40}
	tbl[17]={8081903,40}
	tbl[18]={8091903,40}
	tbl[19]={8101306,40}
	tbl[20]={8111903,35}
	tbl[21]={8122903,35}
	tbl[22]={8130906,35}

	for i = 1,22 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830061, 1, "I4830061_GiftBag")
function I4830053_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(100)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8013010,3}
	tbl[2]={8023010,3}
	tbl[3]={8050310,13}
	tbl[4]={8060310,13}
	tbl[5]={8070310,13}
	tbl[6]={8080310,13}
	tbl[7]={8090310,13}
	tbl[8]={8100310,7}
	tbl[9]={8110310,8}
	tbl[10]={8121310,7}
	tbl[11]={8130310,7}

	for i = 1,11 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830053, 1, "I4830053_GiftBag")
function I4830056_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(100)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8023010,3}
	tbl[2]={8013010,3}
	tbl[3]={8050410,13}
	tbl[4]={8060410,13}
	tbl[5]={8070410,13}
	tbl[6]={8080410,13}
	tbl[7]={8090410,13}
	tbl[8]={8100410,7}
	tbl[9]={8110310,8}
	tbl[10]={8121310,7}
	tbl[11]={8130310,7}
	for i = 1,11 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830056, 1, "I4830056_GiftBag")
function I4830059_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(100)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8043010,3}
	tbl[2]={8033010,3}
	tbl[3]={8051310,13}
	tbl[4]={8061310,13}
	tbl[5]={8071310,13}
	tbl[6]={8081310,13}
	tbl[7]={8091310,13}
	tbl[8]={8100510,7}
	tbl[9]={8111310,8}
	tbl[10]={8122310,7}
	tbl[11]={8130410,7}

	for i = 1,11 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830059, 1, "I4830059_GiftBag")
function I4830062_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID, 生成物品TypeID,数量,品质,生成模式,log用网络消息枚举
	local r = math.random(100)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8033010,3}
	tbl[2]={8043010,3}
	tbl[3]={8051410,13}
	tbl[4]={8061410,13}
	tbl[5]={8071410,13}
	tbl[6]={8081410,13}
	tbl[7]={8091410,13}
	tbl[8]={8100610,7}
	tbl[9]={8111310,8}
	tbl[10]={8122310,7}
	tbl[11]={8130410,7}

	for i = 1,11 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4830062, 1, "I4830062_GiftBag")

