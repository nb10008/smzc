--能否使用和宝图使用事件
function qingrenjiebaotu_CanUse(MapID, InstanceID, TypeID, RoleID)
	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false
	local tbl = {}
	tbl[1] = {x=416,z=426}
	tbl[2] = {x=1279,z=744}
	tbl[3] = {x=1106,z=1157}
	tbl[4] = {x=334,z=960}
	tbl[5] = {x=717,z=552}

	tbl[6] = {x=305,z=1177}
	tbl[7] = {x=1337,z=1269}
	tbl[8] = {x=1293,z=720}
	tbl[9] = {x=1171,z=302}
	tbl[10] = {x=306,z=328}
	local temp = TypeID -4800268
	local map = 0
	if temp <= 5 then
		map = 3424073256
	else
		map = 3424071976
	end
	if MapID ~= map then
		bRet = 43
	else--如果不在这个坐标范围，则不能使用
		if x - tbl[temp].x> 10 or tbl[temp].x - x > 10 or z - tbl[temp].z> 10 or tbl[temp].z - z > 10 then
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
aux.RegisterItemEvent(4800269, 0, "qingrenjiebaotu_CanUse")
aux.RegisterItemEvent(4800270, 0, "qingrenjiebaotu_CanUse")
aux.RegisterItemEvent(4800271, 0, "qingrenjiebaotu_CanUse")
aux.RegisterItemEvent(4800272, 0, "qingrenjiebaotu_CanUse")
aux.RegisterItemEvent(4800273, 0, "qingrenjiebaotu_CanUse")
aux.RegisterItemEvent(4800274, 0, "qingrenjiebaotu_CanUse")
aux.RegisterItemEvent(4800275, 0, "qingrenjiebaotu_CanUse")
aux.RegisterItemEvent(4800276, 0, "qingrenjiebaotu_CanUse")
aux.RegisterItemEvent(4800277, 0, "qingrenjiebaotu_CanUse")
aux.RegisterItemEvent(4800278, 0, "qingrenjiebaotu_CanUse")

function qingrenjiebaotu_OnUse(MapID, InstanceID, TypeID, RoleID)
	local Rate = math.random(1, 100)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	if Rate > 0 and Rate <=50 then
		role.AddRoleSilver(MapID, InstanceID, RoleID, level*1000)
	elseif Rate > 50 and Rate <=70 then
		map.MapCreateCreature(MapID, -1, 6011043, x,y,z)
	elseif Rate > 70 and Rate <=90 then
		map.MapCreateCreature(MapID, -1, 3041001, x,y,z)
	else
		if level < 60 then
			role.AddRoleSilver(MapID, InstanceID, RoleID, level*1000)
		elseif level <= 70 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4800176, 1, -1, 8, 420)
		elseif level <= 85 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4800181, 1, -1, 8, 420)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4800186, 1, -1, 8, 420)
		end
	end
end
aux.RegisterItemEvent(4800269, 1, "qingrenjiebaotu_OnUse")
aux.RegisterItemEvent(4800270, 1, "qingrenjiebaotu_OnUse")
aux.RegisterItemEvent(4800271, 1, "qingrenjiebaotu_OnUse")
aux.RegisterItemEvent(4800272, 1, "qingrenjiebaotu_OnUse")
aux.RegisterItemEvent(4800273, 1, "qingrenjiebaotu_OnUse")
aux.RegisterItemEvent(4800274, 1, "qingrenjiebaotu_OnUse")
aux.RegisterItemEvent(4800275, 1, "qingrenjiebaotu_OnUse")
aux.RegisterItemEvent(4800276, 1, "qingrenjiebaotu_OnUse")
aux.RegisterItemEvent(4800277, 1, "qingrenjiebaotu_OnUse")
aux.RegisterItemEvent(4800278, 1, "qingrenjiebaotu_OnUse")
