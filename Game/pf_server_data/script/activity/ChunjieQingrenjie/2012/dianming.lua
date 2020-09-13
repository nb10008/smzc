GetGiftRoleID = {}
dianmingNormal = {}
dianmingSpecial = {}
function dianmingIni()
	dianmingNormal={1,1,1,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4}
	dianmingSpecial = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,4,4,5,5,5,5,5,6,6,6,6,6,7,7,8,8,9,9,9,9,9,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11}	--特殊奖励
	GetGiftRoleID = {}
end
function dianmingNor(hour,min)
	local giftNormal={1,1,1,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4}    --平常奖励
	local NormalId = {4800288,4800288,4800288,4800288}
    local length=0
    for k,v in pairs(dianmingNormal) do
       length = length + 1
    end
    if length == 0 then
       dianmingIni()
    end
	local temp = dianmingNormal[math.random(#dianmingNormal)]
	local RoleID = chooseRole()
	local giftID
	if(hour == 22 and min == 0) then
		giftID = 4800289
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,209006)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, giftID)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	else
		giftID = NormalId[temp]
		table.remove(dianmingNormal,temp)
	end
	local RoleNums=0
    for k,v in pairs(GetGiftRoleID) do
       RoleNums = RoleNums + 1
    end
	GetGiftRoleID[RoleNums+1] = {RoleID,giftID,1}
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 71, 209011)
	msg.AddMsgEvent(MsgID, 4, giftID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
function dianmingSpe(hour,min)
	local giftSpecial={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,4,4,5,5,5,5,5,6,6,6,6,6,7,7,8,8,9,9,9,9,9,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11}	--特殊奖励
	local SpecialId = {3020120,3020121,3020122,4800234,3030143,3030144,3030145,3030146,4800233,4800260,4800232}
    local length=0
    for k,v in pairs(dianmingSpecial) do
       length = length + 1
    end
    if length == 0 then
       dianmingIni()
    end
	local temp = dianmingSpecial[math.random(#dianmingSpecial)]
	local RoleID = chooseRole()
	local giftID = 0
	if(hour == 22 and min == 0) then
		giftID = 3020035
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 209006)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, giftID)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif(hour == 23 and min == 0) then
		giftID = 3020036
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 209006)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, giftID)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif(hour == 23 and min == 59) then
		giftID = 3020037
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 209006)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, giftID)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)

	else
		giftID = SpecialId[temp]
		table.remove(dianmingSpecial,temp)
	end
	local RoleNums=0
    for k,v in pairs(GetGiftRoleID) do
       RoleNums = RoleNums + 1
    end
	GetGiftRoleID[RoleNums+1] = {RoleID,giftID,1}
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 71, 209011)
	msg.AddMsgEvent(MsgID, 4, giftID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

function chooseRole()   --选择幸运玩家
	local RoleTblIni = map.GetMapAllRoleID(3424073512, -1)
	local RoleTbl = {}
	local length=0
	local luckyRoleId = 0
    for k,v in pairs(RoleTblIni) do
       length = length + 1
    end
	local RoleNums=0
    for k,v in pairs(GetGiftRoleID) do
       RoleNums = RoleNums + 1
    end
	for i = 1,length do
		RoleTbl[i] = RoleTblIni[i]
	end
	for i = 1,length do
		for r= 1,RoleNums do
			if RoleTblIni[i] == GetGiftRoleID[r] then
				table.remove(RoleTbl,RoleTblIni[i])
			end
		end
	end
	local RoleTotalNum=0
    for k,v in pairs(RoleTbl) do
       RoleTotalNum = RoleTotalNum + 1
    end
	if(RoleTotalNum >= 1) then
		local temp = math.random(RoleTotalNum)
		for r=1,RoleTotalNum do
			luckyRoleId = RoleTbl[temp]
			for i = 1,RoleNums do
				if luckyRoleId == GetGiftRoleID[i][1] then
					break
				else
					return luckyRoleId
				end
			end
		end
	else
		if(length >= 1) then
			local temp = math.random(length)
			local luckyRoleId = RoleTblIni[temp]
		end
	end
	return luckyRoleId
end





function xingyunzhishen_OnIalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 20, 209007)   --
	    msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
	    msg.AddMsgEvent(MsgID, 21,	4)			--确定按钮
	    msg.AddMsgEvent(MsgID, 1,209010)			--“确定”
	    msg.AddMsgEvent(MsgID, 21,	20)			--取消按钮
	    msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local num=0
		local i = 0
		local bool = true
		for k,v in pairs(GetGiftRoleID) do
			num = num + 1
		end
		local temp = false
		local inGift = false
		if(num == nil or num ==0 ) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 209009)   --
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	20)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			temp = true
		end
		while bool == true do
			if i < num then
				i = i + 1
			else
				bool = false
			end
			if(num >= 1) then
				if RoleID == GetGiftRoleID[i][1] then
					if GetGiftRoleID[i][3] == 1 then
						inGift = false
						if role.GetBagFreeSize(RoleID) < 1 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
							msg.AddMsgEvent(MsgID, 9, 1)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
							return 32,false
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, GetGiftRoleID[i][2], 1, -1, 8, 35)
							GetGiftRoleID[i][3] = 0
							temp =true
							bool = false
						end
					else
						inGift = true
					end
				end
			end
		end
		if inGift == false and temp == false then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 209009)   --
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	20)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif inGift == true then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 209008)   --
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	20)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100002)			--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		end

	end
end
aux.RegisterCreatureEvent(5610098, 7, "xingyunzhishen_OnIalk")

