function xiazhiliange_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830093, 0, "xiazhiliange_CanUseGiftBag")
aux.RegisterItemEvent(4830094, 0, "xiazhiliange_CanUseGiftBag")
aux.RegisterItemEvent(4830091, 0, "xiazhiliange_CanUseGiftBag")
aux.RegisterItemEvent(4830092, 0, "xiazhiliange_CanUseGiftBag")

function I4830093_OnUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={4820047,1,1000}
	tbl[2]={4820046,1,1000}
	tbl[3]={4530109,4,820}
	tbl[4]={4820039,1,820}
	tbl[5]={4820079,1,820}
	tbl[6]={4820172,1,520}
	tbl[7]={4820061,6,570}
	tbl[8]={3070023,1,770}
	tbl[9]={4500002,6,955}
	tbl[10]={4830048,1,350}
	tbl[11]={4830049,1,350}
	tbl[12]={4700231,1,150}
	tbl[13]={4820256,1,450}
	tbl[14]={4999004,1,470}
	tbl[15]={4820087,2,650}
	tbl[16]={4820262,3000,5}
	tbl[17]={4700226,1,10}
	tbl[18]={4800254,1,10}
	tbl[19]={3030076,1,40}
	tbl[20]={3030084,1,40}
	tbl[21]={3030041,1,40}
	tbl[22]={3030033,1,40}
	tbl[23]={3030013,1,40}
	tbl[24]={3030019,1,40}
	tbl[25]={3030005,1,40}

	for i = 1,25 do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end
	if index >= 16 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)
end
aux.RegisterItemEvent(4830093, 1, "I4830093_OnUseGiftBag")
function I4830094_OnUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={3500203,1,1050}
	tbl[2]={3500204,1,420}
	tbl[3]={3500205,1,100}
	tbl[4]={4530102,1,550}
	tbl[5]={4530105,1,550}
	tbl[6]={3500014,1,700}
	tbl[7]={3500015,1,1050}
	tbl[8]={3500013,1,1050}
	tbl[9]={3500021,1,900}
	tbl[10]={3500022,1,400}
	tbl[11]={3500004,1,600}
	tbl[12]={3500005,1,600}
	tbl[13]={4830091,1,150}
	tbl[14]={3060060,1,800}
	tbl[15]={4530109,4,950}
	tbl[16]={3500206,1,50}
	tbl[17]={3500207,1,35}
	tbl[18]={4830092,1,15}
	tbl[19]={4820260,1,10}
	tbl[20]={4530097,1,20}

	for i = 1,20  do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end
	if index >= 16 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)
end
aux.RegisterItemEvent(4830094, 1, "I4830094_OnUseGiftBag")
function I4830091_OnUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={9850101,1,850}
	tbl[2]={9850102,1,1000}
	tbl[3]={9850103,1,1820}
	tbl[4]={9850104,1,1820}
	tbl[5]={9850105,1,900}
	tbl[6]={9850106,1,1810}
	tbl[7]={9850107,1,900}
	tbl[8]={9850108,1,900}
	for i = 1,8  do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)
end
aux.RegisterItemEvent(4830091, 1, "I4830091_OnUseGiftBag")
function I4830092_OnUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={9850201,1,850}
	tbl[2]={9850202,1,1000}
	tbl[3]={9850203,1,1820}
	tbl[4]={9850204,1,1820}
	tbl[5]={9850205,1,900}
	tbl[6]={9850206,1,1810}
	tbl[7]={9850207,1,900}
	tbl[8]={9850208,1,900}

	for i = 1,8  do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)
end
aux.RegisterItemEvent(4830092, 1, "I4830092_OnUseGiftBag")