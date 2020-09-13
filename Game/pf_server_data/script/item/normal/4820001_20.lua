
--4820001
function i4820001CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 3020027) < 5 or role.GetRoleItemNum(TargetID, 4820022) < 20 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820001, 0, "i4820001CanUse")

function i4820001Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630401, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3020027, 5, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 20, 300)
end
aux.RegisterItemEvent(4820001, 1, "i4820001Use")

--4820002
function i4820002CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 3020029) < 5 or role.GetRoleItemNum(TargetID, 4820022) < 60 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820002, 0, "i4820002CanUse")

function i4820002Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630402, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3020029, 5, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 60, 300)
end
aux.RegisterItemEvent(4820002, 1, "i4820002Use")


--4820004
function i4820004CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 3020022) < 5 or role.GetRoleItemNum(TargetID, 4820022) < 20 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820004, 0, "i4820004CanUse")

function i4820004Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630501, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3020022, 5, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 20, 300)
end
aux.RegisterItemEvent(4820004, 1, "i4820004Use")


--4820005
function i4820005CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 3020025) < 5 or role.GetRoleItemNum(TargetID, 4820022) < 60 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820005, 0, "i4820005CanUse")

function i4820005Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630502, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3020025, 5, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 60, 300)
end
aux.RegisterItemEvent(4820005, 1, "i4820005Use")

--4820007
function i4820007CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 3020027) < 2 or role.GetRoleItemNum(TargetID, 4820022) < 30 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820007, 0, "i4820007CanUse")

function i4820007Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630601, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3020027, 2, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 30, 300)
end
aux.RegisterItemEvent(4820007, 1, "i4820007Use")


--4820008
function i4820008CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 3020032) < 1 or role.GetRoleItemNum(TargetID, 4820022) < 80 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820008, 0, "i4820008CanUse")

function i4820008Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630602, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3020032, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 80, 300)
end
aux.RegisterItemEvent(4820008, 1, "i4820008Use")


--4820010
function i4820010CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	local t1 = role.GetRoleItemNum(TargetID, 3010037)
	if t1 + role.GetRoleItemNum(TargetID, 3010038) < 100 or role.GetRoleItemNum(TargetID, 4820022) < 10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820010, 0, "i4820010CanUse")

function i4820010Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630701, 1, -1, 1, 104)
	local t1 = role.GetRoleItemNum(TargetID, 3010037)
	if t1 < 100 then
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3010037, t1, 300)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3010038, 100-t1, 300)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 10, 300)
	else
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3010037, 100, 300)
        role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 10, 300)
	end
end
aux.RegisterItemEvent(4820010, 1, "i4820010Use")

--4820011
function i4820011CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	local t1 = role.GetRoleItemNum(TargetID, 3010037)
	if t1 + role.GetRoleItemNum(TargetID, 3010038) < 200 or role.GetRoleItemNum(TargetID, 4820022) < 15 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820011, 0, "i4820011CanUse")

function i4820011Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630702, 1, -1, 1, 104)
	local t1 = role.GetRoleItemNum(TargetID, 3010037)
	if t1 < 200 then
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3010037, t1, 300)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3010038, 200-t1, 300)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 15, 300)
	else
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3010037, 200, 300)
        role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 15, 300)
	end
end
aux.RegisterItemEvent(4820011, 1, "i4820011Use")

--4820012
function i4820012CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	local t1 = role.GetRoleItemNum(TargetID, 3010037)
	if t1 + role.GetRoleItemNum(TargetID, 3010038) < 400 or role.GetRoleItemNum(TargetID, 4820022) < 20 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820012, 0, "i4820012CanUse")

function i4820012Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630703, 1, -1, 1, 104)
	local t1 = role.GetRoleItemNum(TargetID, 3010037)
	if t1 < 400 then
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3010037, t1, 300)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3010038, 400-t1, 300)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 20, 300)
	else
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3010037, 400, 300)
        role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 20, 300)
	end
end
aux.RegisterItemEvent(4820012, 1, "i4820012Use")

--4820013
function i4820013CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 4080080) < 10 or role.GetRoleItemNum(TargetID, 4080081) < 10 or role.GetRoleItemNum(TargetID, 4080082) < 10 or role.GetRoleItemNum(TargetID, 4080083) < 10 or role.GetRoleItemNum(TargetID, 4080084) < 10 or role.GetRoleItemNum(TargetID, 4820022) < 8 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820013, 0, "i4820013CanUse")

function i4820013Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630801, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080080, 10, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080081, 10, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080082, 10, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080083, 10, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080084, 10, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 8, 300)
end
aux.RegisterItemEvent(4820013, 1, "i4820013Use")

--4820014
function i4820014CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 4080080) < 50 or role.GetRoleItemNum(TargetID, 4080081) < 50 or role.GetRoleItemNum(TargetID, 4080082) < 50 or role.GetRoleItemNum(TargetID, 4080083) < 50 or role.GetRoleItemNum(TargetID, 4080084) < 50 or role.GetRoleItemNum(TargetID, 4820022) < 10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820014, 0, "i4820014CanUse")

function i4820014Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630802, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080080, 50, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080081, 50, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080082, 50, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080083, 50, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080084, 50, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 10, 300)
end
aux.RegisterItemEvent(4820014, 1, "i4820014Use")

--4820015
function i4820015CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 4080080) < 80 or role.GetRoleItemNum(TargetID, 4080081) < 80 or role.GetRoleItemNum(TargetID, 4080082) < 80 or role.GetRoleItemNum(TargetID, 4080083) < 80 or role.GetRoleItemNum(TargetID, 4080084) < 80 or role.GetRoleItemNum(TargetID, 4820022) < 12 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820015, 0, "i4820015CanUse")

function i4820015Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630803, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080080, 80, 300)
    role.RemoveFromRole(MapID, InstanceID, TargetID, 4080081, 80, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080082, 80, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080083, 80, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080084, 80, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 12, 300)
end
aux.RegisterItemEvent(4820015, 1, "i4820015Use")

--4820016
function i4820016CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 4080080) < 120 or role.GetRoleItemNum(TargetID, 4080081) < 120 or role.GetRoleItemNum(TargetID, 4080082) < 120 or role.GetRoleItemNum(TargetID, 4080083) < 120 or role.GetRoleItemNum(TargetID, 4080084) < 120 or role.GetRoleItemNum(TargetID, 4820022) < 14 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820016, 0, "i4820016CanUse")

function i4820016Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630804, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080080, 120, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080081, 120, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080082, 120, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080083, 120, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080084, 120, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 14, 300)
end
aux.RegisterItemEvent(4820016, 1, "i4820016Use")

--4820017
function i4820017CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 4080080) < 200 or role.GetRoleItemNum(TargetID, 4080081) < 200 or role.GetRoleItemNum(TargetID, 4080082) < 200 or role.GetRoleItemNum(TargetID, 4080083) < 200 or role.GetRoleItemNum(TargetID, 4080084) < 200 or role.GetRoleItemNum(TargetID, 4820022) < 16 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820017, 0, "i4820017CanUse")

function i4820017Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630805, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080080, 200, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080081, 200, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080082, 200, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080083, 200, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4080084, 200, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 16, 300)

end
aux.RegisterItemEvent(4820017, 1, "i4820017Use")

--4820018
function i4820018CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 3030004) < 1 or role.GetRoleItemNum(TargetID, 3030012) < 1 or role.GetRoleItemNum(TargetID, 3030032) < 1 or role.GetRoleItemNum(TargetID, 3030040) < 1 or role.GetRoleItemNum(TargetID, 3030075) < 1 or role.GetRoleItemNum(TargetID, 3030083) < 1 or role.GetRoleItemNum(TargetID, 4820022) < 20 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820018, 0, "i4820018CanUse")

function i4820018Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630901, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030004, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030012, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030032, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030040, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030075, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030083, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 20, 300)
end
aux.RegisterItemEvent(4820018, 1, "i4820018Use")

--4820019
function i4820019CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 3030005) < 1 or role.GetRoleItemNum(TargetID, 3030013) < 1 or role.GetRoleItemNum(TargetID, 3030033) < 1 or role.GetRoleItemNum(TargetID, 3030041) < 1 or role.GetRoleItemNum(TargetID, 3030076) < 1 or role.GetRoleItemNum(TargetID, 3030084) < 1 or role.GetRoleItemNum(TargetID, 4820022) < 60 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4820019, 0, "i4820019CanUse")

function i4820019Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630902, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030005, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030013, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030033, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030041, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030076, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3030084, 1, 300)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820022, 60, 300)
end
aux.RegisterItemEvent(4820019, 1, "i4820019Use")
