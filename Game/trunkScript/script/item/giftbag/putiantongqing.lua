function I4700220_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {4820029,10,-1,1200}
	tbl[2] = {4820029,30,-1,1100}
	tbl[3] = {4820061,15,-1,1000}
	tbl[4] = {4820061,30,-1,800}
	tbl[5] = {3020082,1,-1,500}
	tbl[6] = {4820177,18,-1,700}
	tbl[7] = {4820178,18,-1,700}
	tbl[8] = {4700221,1,-1,1000}
	tbl[9] = {4500002,8,-1,600}
	tbl[10] = {3070052,8,-1,600}
	tbl[11] = {4820079,1,-1,880}
	tbl[12] = {4820078,90,-1,900}
	tbl[13] = {3020090,1,-1,10}
	tbl[14] = {3020091,1,-1,5}
	tbl[15] = {4221704,1,-1,5}

	for i = 1,#tbl do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

	if index >= 13 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

function I4700220_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700220, 1, "I4700220_GiftBag")
aux.RegisterItemEvent(4700220, 0, "I4700220_CanUseGiftBag")





function I4700221_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local class = role.GetRoleClass(TargetID)
    if class==1 or class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8010017, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8020017, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8040017, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8030017, 1, 4, 8, 420)
    end
end

function I4700221_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700221, 1, "I4700221_GiftBag")
aux.RegisterItemEvent(4700221, 0, "I4700221_CanUseGiftBag")

