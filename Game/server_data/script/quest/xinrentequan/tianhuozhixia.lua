

--天火之匣
function I4400501_GiftBag(MapID, InstanceID, TypeID, TargetID)

	local TypeID = {5,4010001,4010002,4010003,4010004,4010005,4010006,4010007,4010008,4010009,4010010}

	local bool_duihuan = false
	local allnum = 0
	for k,v in pairs(TypeID) do
		if k > 1 then
			local i = role.GetRoleItemNum(TargetID, v)
			allnum = i + allnum
		end
		if TypeID[1] <= allnum then
			bool_duihuan = true
			break
		end
	end
	local shengyu = TypeID[1]
	if bool_duihuan then
		for k,v in pairs(TypeID) do
			if k > 1 then
				local i = role.GetRoleItemNum(TargetID, v)
				if i < shengyu then
					role.RemoveFromRole(MapID, InstanceID, TargetID, v, i, 420)
					shengyu = shengyu - i
				else
					role.RemoveFromRole(MapID, InstanceID, TargetID, v, shengyu, 420)
					break
				end
			end
		end
		role.AddRoleItem(MapID, InstanceID, TargetID, 4400502, 1, -1, 8, 420)
	    local item_level = {}
		item_level[1] = {Data = "Tianhuozhixia10_34",Pos = 1,ItemID = {3070024},Num = {1},NormalID = 3070056}      --10-14级
		item_level[2] = {Data = "Tianhuozhixia10_34",Pos = 10,ItemID = {3060001,3050006},Num = {3,2},NormalID = 3070056}    --15-19级
		item_level[3] = {Data = "Tianhuozhixia10_34",Pos = 100,ItemID = {3020003,3020009,3050006},Num = {2,2,2},NormalID = 3070056}    --20-24级
		item_level[4] = {Data = "Tianhuozhixia10_34",Pos = 1000,ItemID = {3050007,3050007,3020004,3020004},Num = {2,2,2,2},NormalID = 3070056}    --25-29级
		item_level[5] = {Data = "Tianhuozhixia10_34",Pos = 10000,ItemID = {3060002,3020014,3100043,3020014,3060002},Num = {1,2,1,1,2},NormalID = 3070056}    --30-34级
		item_level[6] = {Data = "Tianhuozhixia35_59",Pos = 1,ItemID = {3020004,3050012,3050012,3060002,3020004,3060002},Num = {1,2,2,1,3,2},NormalID = 3070056}    --35-39级
		item_level[7] = {Data = "Tianhuozhixia35_59",Pos = 10,ItemID = {3020022,3020009,4090007,3020009,3060002,4090007,3060002},Num = {1,2,1,2,1,1,1},NormalID = 3070056}    --40-44级
		item_level[8] = {Data = "Tianhuozhixia35_59",Pos = 100,ItemID = {3050012,3020014,3050024,4090007,3050024,3050012,3020014,3050012},Num = {1,2,3,2,1,2,2,1},NormalID = 3070056}    --45-49级
		item_level[9] = {Data = "Tianhuozhixia35_59",Pos = 1000,ItemID = {3020005,3050024,3050012,3100044,3060003,3020005,3020005,3060003,3050024},Num = {2,3,2,1,3,5,3,4,2},NormalID = 3070056}    --50-54级
		item_level[10] = {Data = "Tianhuozhixia35_59",Pos = 10000,ItemID = {4090002,3050024,3020015,3060003,3020005,3050025,4090002,4090008,3020005},Num = {3,4,1,4,7,10,1,1,3},NormalID = 3070056}    --55-59级

		local index = math.floor((role.GetRoleLevel(MapID, InstanceID, TargetID) - 10)/5) + 1
		if index > 10 then
		    index = 10
		elseif index < 1 then
		    index = 1
		end
		if math.random(100) > 31 then
			local VAL = role.GetRoleScriptData(TargetID, 1, RoleDataType[item_level[index].Data])

		    local datapos = math.floor((VAL % (item_level[index].Pos * 10)) / item_level[index].Pos)
			if datapos >= 0 and datapos < #item_level[index].ItemID then
			    role.SetRoleScriptData(TargetID, 1, RoleDataType[item_level[index].Data],VAL+item_level[index].Pos)
				datapos = datapos + 1
				role.AddRoleItem(MapID, InstanceID, TargetID, item_level[index].ItemID[datapos], item_level[index].Num[datapos], -1, 8, 420)
			else
				role.AddRoleItem(MapID, InstanceID, TargetID, item_level[index].NormalID, 1, -1, 8, 420)
			end
		else
		    role.AddRoleItem(MapID, InstanceID, TargetID, item_level[index].NormalID, 1, -1, 8, 420)
		end

		local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
		if level > 59 then
		    level = 59
		elseif level < 10 then
		    level = 10
		end

		local roleexp = {}
		roleexp[10] = 370
		roleexp[11] = 440
		roleexp[12] = 510
		roleexp[13] = 590
		roleexp[14] = 680
		roleexp[15] = 770
		roleexp[16] = 870
		roleexp[17] = 980
		roleexp[18] = 1090
		roleexp[19] = 1200
		roleexp[20] = 1330
		roleexp[21] = 1460
		roleexp[22] = 1590
		roleexp[23] = 1730
		roleexp[24] = 1880
		roleexp[25] = 2030
		roleexp[26] = 2190
		roleexp[27] = 2360
		roleexp[28] = 2530
		roleexp[29] = 2700
		roleexp[30] = 3580
		roleexp[31] = 3860
		roleexp[32] = 4280
		roleexp[33] = 4770
		roleexp[34] = 5270
		roleexp[35] = 5990
		roleexp[36] = 6830
		roleexp[37] = 8040
		roleexp[38] = 9370
		roleexp[39] = 11090
		roleexp[40] = 12530
		roleexp[41] = 13960
		roleexp[42] = 15380
		roleexp[43] = 16830
		roleexp[44] = 18260
		roleexp[45] = 20000
		roleexp[46] = 22220
		roleexp[47] = 24520
		roleexp[48] = 26930
		roleexp[49] = 31230
		roleexp[50] = 35410
		roleexp[51] = 39440
		roleexp[52] = 43260
		roleexp[53] = 46990
		roleexp[54] = 50360
		roleexp[55] = 56980
		roleexp[56] = 62250
		roleexp[57] = 69920
		roleexp[58] = 77570
		roleexp[59] = 84120



		if role.GetRoleLevel(MapID, InstanceID, TargetID) >= 60 then
		    if role.RemoveFromRole(MapID, InstanceID, TargetID, 4400501, 1, 420) == 0 then
				role.AddRoleExp(MapID, InstanceID, TargetID, 3000000)
			end
			return 1
		else
		    role.AddRoleExp(MapID, InstanceID, TargetID, roleexp[level])
		end
	end
end


function I4400501_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)	-- 请保证背包内有一个空位再来合成
		msg.AddMsgEvent(MsgID, 9, 2)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 40
	else

		local TypeID = {5,4010001,4010002,4010003,4010004,4010005,4010006,4010007,4010008,4010009,4010010}

		local bool_duihuan = false
		local allnum = 0
		for k,v in pairs(TypeID) do
			if k > 1 then
				local i = role.GetRoleItemNum(TargetID, v)
				allnum = i + allnum
			end
			if TypeID[1] <= allnum then
				bool_duihuan = true
				break
			end
		end
		if not bool_duihuan then
			bRet = 32
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 206429)	-- 背包内的灵石数量不足
			msg.DispatchRoleMsgEvent(TargetID, MsgID)
		end
	end

	return bRet, bIgnore
end

aux.RegisterItemEvent(4400501, 1, "I4400501_GiftBag")
aux.RegisterItemEvent(4400501, 0, "I4400501_CanUseGiftBag")


