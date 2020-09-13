---mapscriptdata[1] 记录进副本后是否已刷出怪物
----[2]记录3040008是否被杀死
----[3]记录3040009是否被杀死
----[4]记录是否有过包围。
----[5]记录是否有埋伏。
----[6]记录3040010是否被杀死



function f02_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100038)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,1)
	if monsterhave==0 then
		local x,y,z = 858, 4200, 623
		map.MapCreateCreature(MapID, InstanceID, 3040007, x, y, z)
		map.MapCreateCreature(MapID, InstanceID, 3040006, x+10, y, z-20)
		map.MapCreateCreature(MapID, InstanceID, 3040004, x-10, y, z-20)
		map.MapCreateCreature(MapID, InstanceID, 3040007, 803, 5590, 742)
		map.MapCreateCreature(MapID, InstanceID, 3040007, 817, 4754, 706)
		map.MapCreateCreature(MapID, InstanceID, 3040007, 338, 5378, 798)
		map.MapCreateCreature(MapID, InstanceID, 3040007, 338, 5378, 783)
		map.MapCreateCreature(MapID, InstanceID, 3040006, 612, 4394, 441)
		map.MapCreateCreature(MapID, InstanceID, 3040005, 603, 4394, 447)
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
	end
	-----接取副本任务
	if not (role.IsRoleCompleteQuest(MapID,InstanceID,RoleID,29000) or role.IsRoleHaveQuest(MapID,InstanceID,RoleID,29000)) then
		role.AddQuest(RoleID,29000)
	end
	--副本统计
	--instanceData(2, RoleID, 25, 32)
	--玩家统计
	--UpdateRoleData(RoleID, 15, 1)
end

--注册
aux.RegisterMapEvent("f02", 2, "f02_OnPlayerEnter")
