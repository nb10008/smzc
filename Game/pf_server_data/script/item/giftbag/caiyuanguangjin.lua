function I4700261_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}

   tbl[1]={4820046,1,-1,700}
   tbl[2]={3070001,3,-1,500}
   tbl[3]={3070051,3,-1,700}
   tbl[4]={4500002,3,-1,700}
   tbl[5]={4820061,8,-1,600}
   tbl[6]={5600001,1,-1,400}
   tbl[7]={3020010,1,-1,500}
   tbl[8]={3020007,1,-1,500}
   tbl[9]={4090011,2,-1,350}
   tbl[10]={4090010,2,-1,350}
   tbl[11]={4090009,3,-1,450}
   tbl[12]={4820047,1,-1,600}
   tbl[13]={3020119,1,-1,290}
   tbl[14]={3030031,1,-1,450}
   tbl[15]={3030039,1,-1,450}
   tbl[16]={3030011,1,-1,300}
   tbl[17]={3030003,1,-1,300}
   tbl[18]={3030074,1,-1,400}
   tbl[19]={3030088,1,-1,300}
   tbl[20]={3030057,1,-1,300}
   tbl[21]={3030062,1,-1,300}
   tbl[22]={3030067,1,-1,300}
   tbl[23]={3020121,1,-1,150}

   tbl[24]={3020122,1,-1,100}
   tbl[25]={3020124,1,-1,10}



	for i = 1,25 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end
	if index >= 24 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700261_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700261, 1, "I4700261_GiftBag")
aux.RegisterItemEvent(4700261, 0, "I4700261_CanUseGiftBag")
