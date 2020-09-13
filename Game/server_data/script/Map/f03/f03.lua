----scriptdata[1]记录3040018是否死亡
----scriptdata[2]记录3040019是否死亡
----scriptdata[3]记录是否刷过4个幻象
----scriptdata[4]记录3040020是否死亡，死亡几次
----scriptdata[5]记录3040021是否死亡，死亡几次
----scriptdata[6]记录3040024是否死亡
----scriptdata[7]记录3040025是否死亡
----scriptdata[8]标记宝箱
----scriptdata[9]标记进副本是否刷怪
function f03_OnPlayerEnter(MapID, InstanceID, RoleID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100052)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local freshmonster = map.GetMapScriptData(MapID, InstanceID, 1,9)
	if freshmonster ==0 then
		map.MapCreateCreature(MapID, InstanceID, 3040014, 685, 2621, 942)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 675, 2621, 942)
		map.MapCreateCreature(MapID, InstanceID, 3040016, 685, 2621, 952)
		map.MapCreateCreature(MapID, InstanceID, 3040016, 675, 2621, 952)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 737, 2698, 738)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 503, 2624, 888)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 525, 2623, 784)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 517, 2623, 783)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 523, 2623, 775)
		map.MapCreateCreature(MapID, InstanceID, 3040016, 514, 2623, 780)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 445, 2624, 838)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 502, 2642, 684)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 429, 2714, 676)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 418, 2614, 583)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 418, 2614, 588)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 413, 2614, 583)
		map.MapCreateCreature(MapID, InstanceID, 3040016, 413, 2595, 592)
		map.SetMapScriptData(MapID, InstanceID, 1,9,1)
	end
	--副本统计
	--instanceData(3, RoleID, 26, 33)
	--玩家统计
	--UpdateRoleData(RoleID, 15, 1)
	--加体验buff
	if experienceTeams ~= nil then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		for k in pairs(experienceTeams) do
			if experienceTeams[k].teamid == TeamID then
				local lev = math.floor(role.GetRoleLevel(MapID, InstanceID, RoleID)/10)
				if lev == 4 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109601, RoleID)
					unit.AddBuff(MapID, InstanceID, RoleID, 6109701, RoleID)
				end
				break
			end
		end
	end
end
--注册
aux.RegisterMapEvent("f03", 2, "f03_OnPlayerEnter")


function f03_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID==3498 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100046)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end
end
aux.RegisterMapEvent("f03", 6, "f03_OnEnterTrigger")
