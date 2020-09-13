
shanggusuipian = {}
shanggusuipian[1]={lastDate = 0,num = 0}  --记录每天上古神装碎片的产出
shanggusuipian[2]={lastDate = 0,num = 0}  --记录每天上古神饰碎片的产出
--暗淡的魔铁宝盒
function I4800172_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local temp = TypeID - 4800172
	if(temp == 0) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, math.random(3,4), -1, 8, 420)
	elseif(temp == 5) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, math.random(1,3), -1, 8, 420)
	elseif(temp == 10) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, math.random(1,3), -1, 8, 420)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, math.random(5,10), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820044, math.random(20,25), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800246, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3510002, 1, -1, 8, 420)
end

function I4800172_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800172, 1, "I4800172_GiftBag")
aux.RegisterItemEvent(4800172, 0, "I4800172_CanUseGiftBag")
aux.RegisterItemEvent(4800177, 1, "I4800172_GiftBag")
aux.RegisterItemEvent(4800177, 0, "I4800172_CanUseGiftBag")
aux.RegisterItemEvent(4800182, 1, "I4800172_GiftBag")
aux.RegisterItemEvent(4800182, 0, "I4800172_CanUseGiftBag")


--魔铁宝盒
function I4800173_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local totalNum = 3
	local FreeSize = role.GetBagFreeSize(TargetID)
	local tbal = {}
	tbal[1] = {}
	tbal[2] = {}
	tbal[3] = {}
	tbal[1][1] = {8010014,8020014,8030014,8040014,8050014,8051014,8060014,8061014,8070014,8071014,8080014,8081014,8090014,8091014,8100014,8110014,8111014,8121014,8122014,8130014}
	tbal[1][2] = {8012007,8022007,8032007,8042007,8050207,8051207,8060207,8061207,8070207,8071207,8080207,8081207,8090207,8091207,8100207,8110207,8111207,8121207,8122207,8130207}
    tbal[2][1] = {8010015,8020015,8030015,8040015,8050015,8051015,8060015,8061015,8070015,8071015,8080015,8081015,8090015,8091015,8100015,8110015,8111015,8121015,8122015,8130015}
	tbal[2][2] = {8012008,8022008,8032008,8042008,8050208,8051208,8060208,8061208,8070208,8071208,8080208,8081208,8090208,8091208,8100208,8110208,8111208,8121208,8122208,8130208}
    tbal[3][1] = {8010016,8020016,8030016,8040016,8050016,8051016,8060016,8061016,8070016,8071016,8080016,8081016,8090016,8091016,8100016,8110016,8111016,8121016,8122016,8130016}
    tbal[3][2] = {8010017,8020017,8030017,8040017,8050017,8051017,8060017,8061017,8070017,8071017,8080017,8081017,8090017,8091017,8100017,8110017,8111017,8121017,8122017,8130017}
	tbal[3][3] = {8012009,8022009,8032009,8042009,8050209,8051209,8060209,8061209,8070209,8071209,8080209,8081209,8090209,8091209,8100209,8110209,8111209,8121209,8122209,8130209}
	local theDays = tonumber(os.date("%j"))
	local temp = TypeID - 4800173
	if(temp == 0) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, math.random(4,5), -1, 8, 420)
	elseif(temp == 5) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, math.random(4,5), -1, 8, 420)
	elseif(temp == 10) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, math.random(4,5), -1, 8, 420)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, math.random(10,15), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820044, math.random(25,30), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800246, math.random(2), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3510002, 1, -1, 8, 420)
	local chance = {}
	for i = 1,15 do
		chance[i] = math.random(100)
	end
	if(FreeSize > 1 and totalNum < 12 and chance[1] <= 1) then
		if(theDays ~= shanggusuipian[1].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].lastDate = theDays
			shanggusuipian[1].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[1].lastDate and shanggusuipian[1].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].num = shanggusuipian[1].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[2] <= 1) then
		if(theDays ~= shanggusuipian[2].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].lastDate = theDays
			shanggusuipian[2].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[2].lastDate and shanggusuipian[2].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].num = shanggusuipian[2].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[3] <= 10) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820175, 1, -1, 8, 420)  --星辰之力
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[4] <= 50) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3010038+temp*0.2, 1, -1, 8, 420)  --活力药水
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if( FreeSize > 1 and totalNum < 12 and chance[5] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12  and chance[6] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[7] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(temp == 10 and FreeSize > 1 and totalNum < 12 and chance[8] <= 4 ) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[3][3])], 1, 4, 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
end

function I4800173_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800173, 1, "I4800173_GiftBag")
aux.RegisterItemEvent(4800173, 0, "I4800173_CanUseGiftBag")
aux.RegisterItemEvent(4800178, 1, "I4800173_GiftBag")
aux.RegisterItemEvent(4800178, 0, "I4800173_CanUseGiftBag")
aux.RegisterItemEvent(4800183, 1, "I4800173_GiftBag")
aux.RegisterItemEvent(4800183, 0, "I4800173_CanUseGiftBag")



--精致的魔铁宝盒
function I4800174_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local totalNum = 3
	local FreeSize = role.GetBagFreeSize(TargetID)
	local tbal = {}
	tbal[1] = {}
	tbal[2] = {}
	tbal[3] = {}
	tbal[1][1] = {8010014,8020014,8030014,8040014,8050014,8051014,8060014,8061014,8070014,8071014,8080014,8081014,8090014,8091014,8100014,8110014,8111014,8121014,8122014,8130014}
	tbal[1][2] = {8012007,8022007,8032007,8042007,8050207,8051207,8060207,8061207,8070207,8071207,8080207,8081207,8090207,8091207,8100207,8110207,8111207,8121207,8122207,8130207}
    tbal[2][1] = {8010015,8020015,8030015,8040015,8050015,8051015,8060015,8061015,8070015,8071015,8080015,8081015,8090015,8091015,8100015,8110015,8111015,8121015,8122015,8130015}
	tbal[2][2] = {8012008,8022008,8032008,8042008,8050208,8051208,8060208,8061208,8070208,8071208,8080208,8081208,8090208,8091208,8100208,8110208,8111208,8121208,8122208,8130208}
    tbal[3][1] = {8010016,8020016,8030016,8040016,8050016,8051016,8060016,8061016,8070016,8071016,8080016,8081016,8090016,8091016,8100016,8110016,8111016,8121016,8122016,8130016}
    tbal[3][2] = {8010017,8020017,8030017,8040017,8050017,8051017,8060017,8061017,8070017,8071017,8080017,8081017,8090017,8091017,8100017,8110017,8111017,8121017,8122017,8130017}
	tbal[3][3] = {8012009,8022009,8032009,8042009,8050209,8051209,8060209,8061209,8070209,8071209,8080209,8081209,8090209,8091209,8100209,8110209,8111209,8121209,8122209,8130209}
	local theDays = tonumber(os.date("%j"))
	local temp = TypeID - 4800174
	if(temp == 0) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, math.random(5,6), -1, 8, 420)
	elseif(temp == 5) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, math.random(5,6), -1, 8, 420)
	elseif(temp == 10) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, math.random(5,6), -1, 8, 420)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, math.random(15,20), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820044, math.random(30,35), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800246, math.random(2), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3510002, 1, -1, 8, 420)
	local chance = {}
	for i = 1,15 do
		chance[i] = math.random(100)
	end
	if(FreeSize > 1 and totalNum < 12 and chance[1] <= 1) then
		if(theDays ~= shanggusuipian[1].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].lastDate = theDays
			shanggusuipian[1].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[1].lastDate and shanggusuipian[1].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].num = shanggusuipian[1].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[2] <= 1) then
		if(theDays ~= shanggusuipian[2].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].lastDate = theDays
			shanggusuipian[2].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[2].lastDate and shanggusuipian[2].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].num = shanggusuipian[2].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[3] <= 20) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820175, 1, -1, 8, 420)  --星辰之力
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[4] <= 100) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3010038+temp*0.2, 1, -1, 8, 420)  --活力药水
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[5] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if( FreeSize > 1 and totalNum < 12 and chance[6] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[7] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if( FreeSize > 1 and totalNum < 12 and chance[8] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if( FreeSize > 1 and totalNum < 12 and chance[9] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(temp == 10 and FreeSize > 1 and totalNum < 12 and chance[10] <= 4 ) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[3][3][math.random(#tbal[3][3])], 1, 4, 8, 420)  --橙色成长
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
end

function I4800174_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800174, 1, "I4800174_GiftBag")
aux.RegisterItemEvent(4800174, 0, "I4800174_CanUseGiftBag")
aux.RegisterItemEvent(4800179, 1, "I4800174_GiftBag")
aux.RegisterItemEvent(4800179, 0, "I4800174_CanUseGiftBag")
aux.RegisterItemEvent(4800184, 1, "I4800174_GiftBag")
aux.RegisterItemEvent(4800184, 0, "I4800174_CanUseGiftBag")


--华丽的魔铁宝盒
function I4800175_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local totalNum = 3
	local FreeSize = role.GetBagFreeSize(TargetID)
	local tbal = {}
	tbal[1] = {}
	tbal[2] = {}
	tbal[3] = {}
	tbal[1][1] = {8010014,8020014,8030014,8040014,8050014,8051014,8060014,8061014,8070014,8071014,8080014,8081014,8090014,8091014,8100014,8110014,8111014,8121014,8122014,8130014}
	tbal[1][2] = {8012007,8022007,8032007,8042007,8050207,8051207,8060207,8061207,8070207,8071207,8080207,8081207,8090207,8091207,8100207,8110207,8111207,8121207,8122207,8130207}
    tbal[2][1] = {8010015,8020015,8030015,8040015,8050015,8051015,8060015,8061015,8070015,8071015,8080015,8081015,8090015,8091015,8100015,8110015,8111015,8121015,8122015,8130015}
	tbal[2][2] = {8012008,8022008,8032008,8042008,8050208,8051208,8060208,8061208,8070208,8071208,8080208,8081208,8090208,8091208,8100208,8110208,8111208,8121208,8122208,8130208}
    tbal[3][1] = {8010016,8020016,8030016,8040016,8050016,8051016,8060016,8061016,8070016,8071016,8080016,8081016,8090016,8091016,8100016,8110016,8111016,8121016,8122016,8130016}
    tbal[3][2] = {8010017,8020017,8030017,8040017,8050017,8051017,8060017,8061017,8070017,8071017,8080017,8081017,8090017,8091017,8100017,8110017,8111017,8121017,8122017,8130017}
	tbal[3][3] = {8012009,8022009,8032009,8042009,8050209,8051209,8060209,8061209,8070209,8071209,8080209,8081209,8090209,8091209,8100209,8110209,8111209,8121209,8122209,8130209}
	local theDays = tonumber(os.date("%j"))
	local temp = TypeID - 4800175
	if(temp == 0) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, math.random(6,7), -1, 8, 420)
	elseif(temp == 5) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, math.random(6,7), -1, 8, 420)
	elseif(temp == 10) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, math.random(6,7), -1, 8, 420)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, math.random(20,25), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820044, math.random(35,40), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800246, math.random(2), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3510002, 1, -1, 8, 420)
	local chance = {}
	for i = 1,15 do
		chance[i] = math.random(100)
	end
	if(FreeSize > 1 and totalNum < 12 and chance[1] <= 1) then
		if(theDays ~= shanggusuipian[1].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].lastDate = theDays
			shanggusuipian[1].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[1].lastDate and shanggusuipian[1].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].num = shanggusuipian[1].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[2] <= 1) then
		if(theDays ~= shanggusuipian[2].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].lastDate = theDays
			shanggusuipian[2].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[2].lastDate and shanggusuipian[2].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].num = shanggusuipian[2].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[3] <= 50) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820175, 1, -1, 8, 420)  --星辰之力
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[4] <= 100) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3010038+temp*0.2, 1, -1, 8, 420)  --活力药水
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[5] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[6] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[7] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[8] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[9] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[10] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(temp == 10 and FreeSize > 1 and totalNum < 12 and chance[11] <= 4 ) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[3][3][math.random(#tbal[3][3])], 1, 4, 8, 420)  --橙色成长
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
end

function I4800175_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800175, 1, "I4800175_GiftBag")
aux.RegisterItemEvent(4800175, 0, "I4800175_CanUseGiftBag")
aux.RegisterItemEvent(4800180, 1, "I4800175_GiftBag")
aux.RegisterItemEvent(4800180, 0, "I4800175_CanUseGiftBag")
aux.RegisterItemEvent(4800185, 1, "I4800175_GiftBag")
aux.RegisterItemEvent(4800185, 0, "I4800175_CanUseGiftBag")


--闪耀的魔铁宝盒
function I4800176_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local totalNum = 3
	local FreeSize = role.GetBagFreeSize(TargetID)
	local tbal = {}
	tbal[1] = {}
	tbal[2] = {}
	tbal[3] = {}
	tbal[1][1] = {8010014,8020014,8030014,8040014,8050014,8051014,8060014,8061014,8070014,8071014,8080014,8081014,8090014,8091014,8100014,8110014,8111014,8121014,8122014,8130014}
	tbal[1][2] = {8012007,8022007,8032007,8042007,8050207,8051207,8060207,8061207,8070207,8071207,8080207,8081207,8090207,8091207,8100207,8110207,8111207,8121207,8122207,8130207}
    tbal[2][1] = {8010015,8020015,8030015,8040015,8050015,8051015,8060015,8061015,8070015,8071015,8080015,8081015,8090015,8091015,8100015,8110015,8111015,8121015,8122015,8130015}
	tbal[2][2] = {8012008,8022008,8032008,8042008,8050208,8051208,8060208,8061208,8070208,8071208,8080208,8081208,8090208,8091208,8100208,8110208,8111208,8121208,8122208,8130208}
    tbal[3][1] = {8010016,8020016,8030016,8040016,8050016,8051016,8060016,8061016,8070016,8071016,8080016,8081016,8090016,8091016,8100016,8110016,8111016,8121016,8122016,8130016}
    tbal[3][2] = {8010017,8020017,8030017,8040017,8050017,8051017,8060017,8061017,8070017,8071017,8080017,8081017,8090017,8091017,8100017,8110017,8111017,8121017,8122017,8130017}
	tbal[3][3] = {8012009,8022009,8032009,8042009,8050209,8051209,8060209,8061209,8070209,8071209,8080209,8081209,8090209,8091209,8100209,8110209,8111209,8121209,8122209,8130209}
	local theDays = tonumber(os.date("%j"))
	local temp = TypeID - 4800176
	if(temp == 0) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, math.random(7,8), -1, 8, 420)
	elseif(temp == 5) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, math.random(7,8), -1, 8, 420)
	elseif(temp == 10) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, math.random(7,8), -1, 8, 420)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, math.random(25,30), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820044, math.random(40,45), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800246, math.random(2), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3510002, 1, -1, 8, 420)
	local chance = {}
	for i = 1,15 do
		chance[i] = math.random(100)
	end
	if(FreeSize > 1 and totalNum < 12 and chance[1] <= 1) then
		if(theDays ~= shanggusuipian[1].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].lastDate = theDays
			shanggusuipian[1].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[1].lastDate and shanggusuipian[1].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].num = shanggusuipian[1].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[2] <= 1) then
		if(theDays ~= shanggusuipian[2].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].lastDate = theDays
			shanggusuipian[2].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[2].lastDate and shanggusuipian[2].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].num = shanggusuipian[2].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[3] <= 100) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820175, 1, -1, 8, 420)  --星辰之力
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[4] <= 100) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3010038+temp*0.2, 2, -1, 8, 420)  --活力药水
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[5] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[6] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[7] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[8] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[9] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[10] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[11] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(temp == 10 and FreeSize > 1 and totalNum < 12 and chance[12] <= 8 ) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[3][3][math.random(#tbal[3][3])], 1, 4, 8, 420)  --橙色成长
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
end

function I4800176_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800176, 1, "I4800176_GiftBag")
aux.RegisterItemEvent(4800176, 0, "I4800176_CanUseGiftBag")
aux.RegisterItemEvent(4800181, 1, "I4800176_GiftBag")
aux.RegisterItemEvent(4800181, 0, "I4800176_CanUseGiftBag")
aux.RegisterItemEvent(4800186, 1, "I4800176_GiftBag")
aux.RegisterItemEvent(4800186, 0, "I4800176_CanUseGiftBag")




--魔石宝盒
function I4800187_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local totalNum = 3
	local FreeSize = role.GetBagFreeSize(TargetID)
	local tbal = {}
	tbal[1] = {}
	tbal[2] = {}
	tbal[3] = {}
	tbal[1][1] = {8010014,8020014,8030014,8040014,8050014,8051014,8060014,8061014,8070014,8071014,8080014,8081014,8090014,8091014,8100014,8110014,8111014,8121014,8122014,8130014}
	tbal[1][2] = {8012007,8022007,8032007,8042007,8050207,8051207,8060207,8061207,8070207,8071207,8080207,8081207,8090207,8091207,8100207,8110207,8111207,8121207,8122207,8130207}
    tbal[2][1] = {8010015,8020015,8030015,8040015,8050015,8051015,8060015,8061015,8070015,8071015,8080015,8081015,8090015,8091015,8100015,8110015,8111015,8121015,8122015,8130015}
	tbal[2][2] = {8012008,8022008,8032008,8042008,8050208,8051208,8060208,8061208,8070208,8071208,8080208,8081208,8090208,8091208,8100208,8110208,8111208,8121208,8122208,8130208}
    tbal[3][1] = {8010016,8020016,8030016,8040016,8050016,8051016,8060016,8061016,8070016,8071016,8080016,8081016,8090016,8091016,8100016,8110016,8111016,8121016,8122016,8130016}
    tbal[3][2] = {8010017,8020017,8030017,8040017,8050017,8051017,8060017,8061017,8070017,8071017,8080017,8081017,8090017,8091017,8100017,8110017,8111017,8121017,8122017,8130017}
	tbal[3][3] = {8012009,8022009,8032009,8042009,8050209,8051209,8060209,8061209,8070209,8071209,8080209,8081209,8090209,8091209,8100209,8110209,8111209,8121209,8122209,8130209}
	local theDays = tonumber(os.date("%j"))
	local temp = TypeID - 4800187
	if(temp == 0) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, math.random(8,10), -1, 8, 420)
	elseif(temp == 1) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, math.random(8,10), -1, 8, 420)
	elseif(temp == 2) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, math.random(8,10), -1, 8, 420)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, math.random(25,30), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820044, math.random(45,50), -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800246, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3510002, 1, -1, 8, 420)
	local chance = {}
	for i = 1,15 do
		chance[i] = math.random(100)
	end
	if(FreeSize > 1 and totalNum < 12 and chance[1] <= 1) then
		if(theDays ~= shanggusuipian[1].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].lastDate = theDays
			shanggusuipian[1].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[1].lastDate and shanggusuipian[1].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820120, 1, -1, 8, 420)
			shanggusuipian[1].num = shanggusuipian[1].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[2] <= 1) then
		if(theDays ~= shanggusuipian[2].lastDate) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].lastDate = theDays
			shanggusuipian[2].num = 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		elseif(theDays == shanggusuipian[2].lastDate and shanggusuipian[2].num < 3) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820121, 1, -1, 8, 420)
			shanggusuipian[2].num = shanggusuipian[2].num + 1
			totalNum = totalNum + 1
			FreeSize = FreeSize - 1
		end
	end
	if(FreeSize > 1 and totalNum < 12 and chance[3] <= 100) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820175, 1, -1, 8, 420)  --星辰之力
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[4] <= 100) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3010038+temp, 2, -1, 8, 420)  --活力药水
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[5] <= 50) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[6] <= 50) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[7] <= 50) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][1][math.random(#tbal[temp*0.2+1][1])], 1, 3, 8, 420)  --成长装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[8] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[9] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[10] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(FreeSize > 1 and totalNum < 12 and chance[11] <= 30) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[temp*0.2+1][2][math.random(#tbal[temp*0.2+1][2])], 1, math.random(3,4), 8, 420)  --副本装备
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
	if(temp == 10 and FreeSize > 1 and totalNum < 12 and chance[12] <= 12 ) then
		role.AddRoleItem(MapID, InstanceID, TargetID, tbal[3][3][math.random(#tbal[3][3])], 1, 4, 8, 420)  --橙色成长
		totalNum = totalNum + 1
		FreeSize = FreeSize - 1
	end
end

function I4800187_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 7) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800187, 1, "I4800187_GiftBag")
aux.RegisterItemEvent(4800187, 0, "I4800187_CanUseGiftBag")
aux.RegisterItemEvent(4800188, 1, "I4800187_GiftBag")
aux.RegisterItemEvent(4800188, 0, "I4800187_CanUseGiftBag")
aux.RegisterItemEvent(4800189, 1, "I4800187_GiftBag")
aux.RegisterItemEvent(4800189, 0, "I4800187_CanUseGiftBag")
