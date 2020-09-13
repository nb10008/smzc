function I4700185_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1] = {4820038,30,-1,620}
	tbl[2] = {3070052,3,-1,700}
	tbl[3] = {3020082,1,-1,200}
	tbl[4] = {3040004,1,-1,450}
	tbl[5] = {3040009,1,-1,450}
	tbl[6] = {3040014,1,-1,450}
	tbl[7] = {3040019,1,-1,450}
	tbl[8] = {3040024,1,-1,450}
	tbl[9] = {3040029,1,-1,450}
	tbl[10] = {3040034,1,-1,450}
	tbl[11] = {3040039,1,-1,450}
	tbl[12] = {3040044,1,-1,450}
	tbl[13] = {3040049,1,-1,300}
	tbl[14] = {3040054,1,-1,300}
	tbl[15] = {3040059,1,-1,300}
	tbl[16] = {4999004,1,-1,200}
	tbl[17] = {4080097,1,-1,650}
	tbl[18] = {4530108,3,-1,550}
	tbl[19] = {4500002,3,-1,800}
	tbl[20] = {5600004,1,-1,700}
	tbl[21] = {4820039,1,-1,400}
	tbl[22] = {3060060,1,-1,200}
	tbl[23] = {3020091,1,-1,5}
	tbl[24] = {3020090,1,-1,10}
	tbl[25] = {4211203,1,-1,5}
	tbl[26] = {4211303,1,-1,5}
	tbl[27] = {4221004,1,-1,5}

	local tbl1={}   --第一次开启宝箱的物品
	tbl1[1] = {3020082,1,-1}
	tbl1[2] = {4530108,3,-1}
	tbl1[3] = {4999004,1,-1}
	tbl1[4] = {4820039,1,-1}
	tbl1[5] = {3040059,1,-1}
	tbl1[6] = {3040049,1,-1}
	tbl1[7] = {3040054,1,-1}
	tbl1[8] = {3060060,1,-1}
	local tbl5={}   --第五次开启宝箱的物品
	tbl5[1] = {4080097,1,-1}
	tbl5[2] = {3040049,3,-1}
	tbl5[3] = {3040054,1,-1}
	tbl5[4] = {3040059,1,-1}
	tbl5[5] = {3060060,1,-1}
	local times = role.GetRoleScriptData(TargetID, 1, RoleDataType["manyuezhiguang"])
	if(times == 0) then
		local index = math.random(8)
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl1[index][1], tbl1[index][2], tbl1[index][3], 8, 420)

		if index == 8 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, TypeID)
			msg.AddMsgEvent(MsgID, 4, tbl1[index][1])
			msg.DispatchWorldMsgEvent(MsgID)
		end
	elseif(times == 4) then
		local index = math.random(5)
		role.AddRoleItem(MapID, InstanceID, TargetID, tbl5[index][1], tbl5[index][2], tbl5[index][3], 8, 420)
		if index == 5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, TypeID)
			msg.AddMsgEvent(MsgID, 4, tbl5[index][1])
			msg.DispatchWorldMsgEvent(MsgID)
		end
	else
		for i = 1,#tbl do
			temp = temp + tbl[i][4]
			if temp >= r then
				index = i
				break
			end
		end

		role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

		if index >= 22 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, TypeID)
			msg.AddMsgEvent(MsgID, 4, tbl[index][1])
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end
	role.SetRoleScriptData(TargetID, 1, RoleDataType["manyuezhiguang"], times+1)
end

function I4700185_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700185, 1, "I4700185_GiftBag")
aux.RegisterItemEvent(4700185, 0, "I4700185_CanUseGiftBag")




function I4820059_Use(MapID, InstanceID, TypeID, TargetID)
	local tianya = role.GetRoleItemNum(TargetID,4820059)
	local yueluo = role.GetRoleItemNum(TargetID,4820060)
	if (yueluo > 0 and tianya > 0) then
		role.RemoveFromRole(MapID, InstanceID, TargetID, 4820059, 1, 101)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 4820060, 1, 101)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700185, 1, -1, 8, 420)
	end
	return 1
end

function I4820059_CanUse(MapID, InstanceID, TypeID, TargetID)
	local yueluo = role.GetRoleItemNum(TargetID,4820059)
	local tianya = role.GetRoleItemNum(TargetID,4820060)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if (yueluo < 1 or tianya < 1) then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4820059, 1, "I4820059_Use")
aux.RegisterItemEvent(4820059, 0, "I4820059_CanUse")
aux.RegisterItemEvent(4820060, 1, "I4820059_Use")
aux.RegisterItemEvent(4820060, 0, "I4820059_CanUse")
