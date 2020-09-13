function qingliangyixia_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
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
aux.RegisterItemEvent(4830081, 0, "qingliangyixia_CanUseGiftBag")

function I4830081_OnUseGiftBag(MapID, InstanceID, TypeID, TargetID)
		local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}
	tbl[1]={3070001,1,600}
	tbl[2]={3060005,15,800}
	tbl[3]={3070018,1,400}
	tbl[4]={3070020,1,600}
	tbl[5]={3070028,1,900}
	tbl[6]={3070034,1,700}
	tbl[7]={3070040,1,750}
	tbl[8]={3070051,1,900}
	tbl[9]={4500002,2,900}
	tbl[10]={3070078,1,30}
	tbl[11]={3020007,1,700}
	tbl[12]={3020010,1,700}
	tbl[13]={3010028,1,5}
	tbl[14]={3010039,1,5}
	tbl[15]={3010049,1,5}
	tbl[16]={3010048,1,5}
	tbl[17]={3020011,1,300}
	tbl[18]={3020012,1,300}
	tbl[19]={3070010,1,200}
	tbl[20]={4800131,1,300}
	tbl[21]={4700302,1,300}
	tbl[22]={4530100,1,200}
	tbl[23]={4530103,1,200}
	tbl[24]={3500013,1,200}

	for i = 1,24 do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)
end
aux.RegisterItemEvent(4830081, 1, "I4830081_OnUseGiftBag")


function I4830085_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0
	--判断背包空闲空间是否足够
	local num = role.GetRoleItemNum(TargetID, 4830081)
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	else
		if num < 1 then
			bRet = 32
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 200006)--缺少清凉一夏，无法合成
			msg.DispatchRoleMsgEvent(TargetID, QuestID1)
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830085, 0, "I4830085_CanUse")
aux.RegisterItemEvent(4830086, 0, "I4830085_CanUse")
aux.RegisterItemEvent(4830087, 0, "I4830085_CanUse")
function I4830085_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4830081, 1, 101)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830082, 1, -1, 8, 420)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102,300102)      --XX通过使用XX获得XX
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 4, TypeID)
	msg.AddMsgEvent(MsgID, 4, 4830082)
	msg.DispatchWorldMsgEvent(MsgID)
end
aux.RegisterItemEvent(4830085, 1, "I4830085_OnUse")
function I4830086_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4830081, 1, 101)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830083, 1, -1, 8, 420)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102,300102)      --XX通过使用XX获得XX
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 4, TypeID)
	msg.AddMsgEvent(MsgID, 4, 4830083)
	msg.DispatchWorldMsgEvent(MsgID)
end
aux.RegisterItemEvent(4830086, 1, "I4830086_OnUse")
function I4830087_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4830081, 1, 101)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830084, 1, -1, 8, 420)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102,300102)      --XX通过使用XX获得XX
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 4, TypeID)
	msg.AddMsgEvent(MsgID, 4, 4830084)
	msg.DispatchWorldMsgEvent(MsgID)
end
aux.RegisterItemEvent(4830087, 1, "I4830087_OnUse")