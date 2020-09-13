--红色BOSS坐标524，7575，846
---事件1中心  3378   6348   47588

--[[
<color=(30,136,255)>//gg现在的坐标是：602，3714，356
<color=(30,136,255)>//gg现在的坐标是：483，4041，365
<color=(30,136,255)>//gg现在的坐标是：378，4420，245
<color=(30,136,255)>//gg现在的坐标是：373，4333，150
<color=(30,136,255)>//gg现在的坐标是：174，4470，212
<color=(30,136,255)>//gg现在的坐标是：209，4752，304
<color=(30,136,255)>//gg现在的坐标是：426，4137，349
<color=(30,136,255)>//gg现在的坐标是：480，4469，184
]]

--[[
9f4--区域2548
小房间中心点 67,6348,951
玩家传入点 106,6348,882
]]
function shijian1_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local Role = {}
	Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		for i=1, 6 do
			role.RoleGotoNewMap(MapID, InstanceID, Role[i], MapID,106, 6348, 882)
		end
	elseif TeamID ~= nil then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 106, 6348, 882)--事件1坐标4800   6300  47400进入区域
	end
	map.MapCreateColCreature(MapID, InstanceID,4020137, 106,6348,882, 1)
end
aux.RegisterCreatureEvent(4020180, 4, "shijian1_OnDie")

function shijian1BOSS_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local Role = {}
	Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		for i=1, 6 do
			role.RoleGotoNewMap(MapID, InstanceID, Role[i], MapID, x, y, z)
		end
	elseif TeamID ~= nil then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 106, 6348, 882)--事件1坐标4800   6300  47400进入区域
	end
end
aux.RegisterCreatureEvent(4020137, 4, "shijian1BOSS_OnDie")
function SHquyu_OnEnterArea(MapID, InstanceID, RoleID, objID)
	
	if objID == 2548  then
		map.SetMonsterGenState(MapID, InstanceID, 3)
	end
end
aux.RegisterMapEvent( "dmap1", 7, "SHquyu_OnEnterArea")
function QiPao_OnEnterArea(MapID, InstanceID, RoleID, objID)
	
	if objID == 2548  then
		map.SetMonsterGenState(MapID, InstanceID, 4)
	end
end
aux.RegisterMapEvent( "dmap1", 23, "QiPao_OnEnterArea")