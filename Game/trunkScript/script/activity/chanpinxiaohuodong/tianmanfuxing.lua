function I4700313_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={4080098,2,1000}
	tbl[2]={3070023,1,220}
	tbl[3]={4400031,1,170}
	tbl[4]={4500002,1,450}
	tbl[5]={3070051,1,550}
	tbl[6]={3070055,1,550}
	tbl[7]={3070028,1,350}
	tbl[8]={3060005,3,400}
	tbl[9]={4820047,1,670}
	tbl[10]={4820046,1,670}
	tbl[11]={4820061,1,450}
	tbl[12]={4820045,1,650}
	tbl[13]={4700229,1,350}
	tbl[14]={4700230,1,150}
	tbl[15]={4820087,1,270}
	tbl[16]={4820080,30,700}
	tbl[17]={4820040,30,700}
	tbl[18]={4530109,1,505}
	tbl[19]={4090010,1,365}
	tbl[20]={4090011,1,365}
	tbl[21]={4820228,1,200}
	tbl[22]={3020026,1,50}
	tbl[23]={4630101,1,5}
	tbl[24]={4630201,1,5}
	tbl[25]={4630001,1,5}
	tbl[26]={3020052,1,200}




	for i = 1,26 do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end
	if index >= 21 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)

end

function I4700313_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700313, 1, "I4700313_GiftBag")
aux.RegisterItemEvent(4700313, 0, "I4700313_CanUseGiftBag")
