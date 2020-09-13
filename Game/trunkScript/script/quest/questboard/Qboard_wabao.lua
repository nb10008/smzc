
--能否使用和宝图使用事件
--4010232	宝物地点：1783，2366
function baotu1_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 1773 or x > 1793 or z < 2356 or z > 2376 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010232, 0, "baotu1_CanUse")
function baotu1_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010233, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010234, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010235, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010236, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010232, 1, "baotu1_OnUse")

--4010233	宝物地点：1010，2106
function baotu2_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if  MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 1000 or x > 1020 or z < 1996 or z > 2116 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010233, 0, "baotu2_CanUse")
function baotu2_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010232, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010234, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010235, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010236, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010233, 1, "baotu2_OnUse")

--4010234	宝物地点：1777，1844
function baotu3_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if  MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 1767 or x > 1787 or z < 1834 or z > 1854 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010234, 0, "baotu3_CanUse")
function baotu3_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010233, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010232, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010235, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010236, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010234, 1, "baotu3_OnUse")
--4010235	宝物地点：2422，1559
function baotu4_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if  MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 2412 or x > 2432 or z < 1549 or z > 1569 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010235, 0, "baotu4_CanUse")
function baotu4_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010233, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010234, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010232, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010236, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010235, 1, "baotu4_OnUse")
--4010236	宝物地点：2469，611
function baotu5_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 2456 or x > 2476 or z < 601 or z > 621 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010236, 0, "baotu5_CanUse")
function baotu5_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010233, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010234, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010235, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010232, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010236, 1, "baotu5_OnUse")

--4010237	宝物地点：1834，903
function baotu6_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 1824 or x > 1844 or z < 893 or z > 913 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010237, 0, "baotu6_CanUse")
function baotu6_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010238, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010239, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010240, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010241, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010237, 1, "baotu6_OnUse")

--4010238	宝物地点：1427，1242
function baotu7_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 1417 or x > 1437 or z < 1232 or z > 1252 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010238, 0, "baotu7_CanUse")
function baotu7_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010237, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010239, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010240, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010241, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010238, 1, "baotu7_OnUse")

--4010239	宝物地点：752，1773
function baotu8_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 742 or x > 762 or z < 1763 or z > 1783 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010239, 0, "baotu8_CanUse")
function baotu8_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010238, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010237, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010240, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010241, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010239, 1, "baotu8_OnUse")

--4010240	宝物地点：1942，961
function baotu9_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 1932 or x > 1952 or z < 951 or z > 971 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010240, 0, "baotu9_CanUse")
function baotu9_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010238, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010239, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010237, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010241, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010240, 1, "baotu9_OnUse")

--4010241	宝物地点：193，1001
function baotu10_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false

	if MapID ~= 3424075048 then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x < 183 or x > 203 or z < 991 or z > 1011 then
			bRet = 43
		else
		--背包小于1时不能使用
			local FreeSize = role.GetBagFreeSize(RoleID)
			if (FreeSize < 1) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 100003)
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 40
				return bRet, bIgnore
			end
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010241, 0, "baotu10_CanUse")
function baotu10_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local a = math.random(1, 4)
	if Rate > 0 and Rate <=25 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4010211,1 , -1, 8, 420)--宝物
	elseif Rate > 25 and Rate <=100 then
		if a == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010238, 1, -1, 8, 420)
		elseif a == 2 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010239, 1, -1, 8, 420)
		elseif a == 3 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010240, 1, -1, 8, 420)
		elseif a == 4 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010237, 1, -1, 8, 420)
		end
	end
	if Rate > 25 and Rate <=75 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, 10000, 420)--1W银

	elseif Rate > 75 and Rate <=90 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3050003, 10, -1, 8, 420)--10个结晶
	elseif Rate > 90 and Rate <=100 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3020001, 10, -1, 8, 420)--15个强化石
	end
end
aux.RegisterItemEvent(4010241, 1, "baotu10_OnUse")

