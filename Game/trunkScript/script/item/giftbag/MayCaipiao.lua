function I4700303_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100000)
	
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={3030118,1,-1,200}
	tbl[2]={4820035,1,-1,2000}
	tbl[3]={3030095,1,-1,4000}
	tbl[4]={5600008,1,-1,1350}
	tbl[5]={4820229,5,-1,5400}
	tbl[6]={4080071,6,-1,9000}
	tbl[7]={3070021,2,-1,3400}
	tbl[8]={3070019,2,-1,3400}
	tbl[9]={3040060,1,-1,600}
	tbl[10]={3040025,1,-1,700}
	tbl[11]={3040059,1,-1,3000}
	tbl[12]={3040024,1,-1,3000}
	tbl[13]={3500002,5,-1,8000}
	tbl[14]={3500011,5,-1,8000}
	tbl[15]={3500012,1,-1,6000}
	tbl[16]={3510002,5,-1,6000}
	tbl[17]={3100212,1,-1,2800}
	tbl[18]={3100206,1,-1,2700}
	tbl[19]={3100200,1,-1,3400}
	tbl[20]={3060005,100,-1,3400}
	tbl[21]={4700301,1,-1,2000}
	tbl[22]={4700302,1,-1,5000}
	tbl[23]={4090019,2,-1,4000}
	tbl[24]={4090020,2,-1,1350}
	tbl[25]={4090021,2,-1,4000}
	tbl[26]={3060060,1,-1,1800}
	tbl[27]={3060059,1,-1,2700}
	tbl[28]={4999005,1,-1,1080}
	tbl[29]={4820108,20,-1,200}
	tbl[30]={4700226,1,-1,70}
	tbl[31]={4800254,1,-1,70}--后面开始喊话
	tbl[32]={4820179,1,-1,300}
	tbl[33]={3030055,1,-1,110}
	tbl[34]={3030049,1,-1,110}
	tbl[35]={3030091,1,-1,110}
	tbl[36]={3030050,1,-1,20}
	tbl[37]={3030092,1,-1,20}
	tbl[38]={3030056,1,-1,20}
	tbl[39]={3030119,1,-1,100}
	tbl[40]={4222501,1,-1,30}
	tbl[41]={3020102,1,-1,1}
	tbl[42]={3020092,1,-1,49}
	tbl[43]={4820180,1,-1,30}
	tbl[44]={3020035,1,-1,100}
	tbl[45]={3020088,1,-1,50}
	tbl[46]={4700300,1,-1,10}
	tbl[47]={3060061,1,-1,10}
	tbl[48]={3060058,1,-1,300}
	tbl[49]={4700231,1,-1,10}

	for i = 1,49 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end
	if index >= 32 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700303_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700303, 1, "I4700303_GiftBag")
aux.RegisterItemEvent(4700303, 0, "I4700303_CanUseGiftBag")
aux.RegisterItemEvent(4700297, 0, "I4700303_CanUseGiftBag")
aux.RegisterItemEvent(4700298, 0, "I4700303_CanUseGiftBag")
aux.RegisterItemEvent(4700299, 0, "I4700303_CanUseGiftBag")
aux.RegisterItemEvent(4700300, 0, "I4700303_CanUseGiftBag")
aux.RegisterItemEvent(4700301, 0, "I4700303_CanUseGiftBag")
aux.RegisterItemEvent(4700302, 0, "I4700303_CanUseGiftBag")
--粉妖包
function I4700300_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(29)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8161011, 1}
	tbl[2]={8161012, 1}
	tbl[3]={8161013, 1}
	tbl[4]={8161014, 1}
	tbl[5]={8161015, 1}
	tbl[6]={8160032, 1}
	tbl[7]={8160033, 1}
	tbl[8]={8171011, 1}
	tbl[9]={8171012, 1}
	tbl[10]={8171013, 1}
	tbl[11]={8171014, 1}
	tbl[12]={8171015, 1}
	tbl[13]={8170032, 1}
	tbl[14]={8170033, 1}
	tbl[15]={8170138, 1}
	tbl[16]={8170139, 1}
	tbl[17]={8170140, 1}
	tbl[18]={8170141, 1}
	tbl[19]={8170142, 1}
	tbl[20]={8170143, 1}
	tbl[21]={8170145, 1}
	tbl[22]={8170146, 1}
	tbl[23]={8170147, 1}
	tbl[24]={8160053, 1}
	tbl[25]={8160054, 1}
	tbl[26]={8160055, 1}
	tbl[27]={8160056, 1}
	tbl[28]={8160057, 1}
	tbl[29]={8160058, 1}
	
	for i = 1,29 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end
	
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4700300, 1, "I4700300_GiftBag")
--紫妖包
function I4700301_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(40)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8160004,1}
	tbl[2]={8160008,1}
	tbl[3]={8160012,1}
	tbl[4]={8160016,1}
	tbl[5]={8160020,1}
	tbl[6]={8170004,1}
	tbl[7]={8170008,1}
	tbl[8]={8170012,1}
	tbl[9]={8170016,1}
	tbl[10]={8170020,1}
	tbl[11]={8170028,1}
	tbl[12]={8170029,1}
	tbl[13]={8160039,1}
	tbl[14]={8160040,1}
	tbl[15]={8160041,1}
	tbl[16]={8160042,1}
	tbl[17]={8160043,1}
	tbl[18]={8160044,1}
	tbl[19]={8160045,1}
	tbl[20]={8160046,1}
	tbl[21]={8160047,1}
	tbl[22]={8160048,1}
	tbl[23]={8160049,1}
	tbl[24]={8160050,1}
	tbl[25]={8160051,1}
	tbl[26]={8160052,1}
	tbl[27]={8170124,1}
	tbl[28]={8170125,1}
	tbl[29]={8170126,1}
	tbl[30]={8170127,1}
	tbl[31]={8170128,1}
	tbl[32]={8170129,1}
	tbl[33]={8170130,1}
	tbl[34]={8170131,1}
	tbl[35]={8170132,1}
	tbl[36]={8170133,1}
	tbl[37]={8170134,1}
	tbl[38]={8170135,1}
	tbl[39]={8170136,1}
	tbl[40]={8170137,1}
	for i = 1,40 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end
	
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4700301, 1, "I4700301_GiftBag")
--橙妖包
function I4700302_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(26)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={8160002,1}
	tbl[2]={8160003,1}
	tbl[3]={8160006,1}
	tbl[4]={8160007,1}
	tbl[5]={8160010,1}
	tbl[6]={8160011,1}
	tbl[7]={8160014,1}
	tbl[8]={8160015,1}
	tbl[9]={8160018,1}
	tbl[10]={8160019,1}
	tbl[11]={8160036,1}
	tbl[12]={8160037,1}
	tbl[13]={8160038,1}
	tbl[14]={8170002,1}
	tbl[15]={8170003,1}
	tbl[16]={8170006,1}
	tbl[17]={8170007,1}
	tbl[18]={8170010,1}
	tbl[19]={8170011,1}
	tbl[20]={8170014,1}
	tbl[21]={8170015,1}
	tbl[22]={8170018,1}
	tbl[23]={8170019,1}
	tbl[24]={8170121,1}
	tbl[25]={8170122,1}
	tbl[26]={8170123,1}
	for i = 1,26 do
	    temp = temp + tbl[i][2]
		if temp >= r then
		    index = i
			break
		end
	end
	
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4700302, 1, "I4700302_GiftBag")
--4700297	魂晶包80-80
function I4700297_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(2)
	if r == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8220521, 1, -1, 8, 420)
	elseif r == 2 then 
		role.AddRoleItem(MapID, InstanceID, TargetID, 8220621, 1, -1, 8, 420)
	end

end
aux.RegisterItemEvent(4700297, 1, "4700297_GiftBag")

--4700298	魂晶包80-75
function I4700298_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(2)
	if r == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8220522, 1, -1, 8, 420)
	elseif r == 2 then 
		role.AddRoleItem(MapID, InstanceID, TargetID, 8220622, 1, -1, 8, 420)
	end

end
aux.RegisterItemEvent(4700298, 1, "4700298_GiftBag")
--4700299	魂晶包75-70
function I4700299_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(2)
	if r == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8220523, 1, -1, 8, 420)
	elseif r == 2 then 
		role.AddRoleItem(MapID, InstanceID, TargetID, 8220623, 1, -1, 8, 420)
	end

end
aux.RegisterItemEvent(4700299, 1, "4700299_GiftBag")


