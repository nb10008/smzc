--[[
function c3040002_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--local y=1000
	local x,y,z=unit.GetPosition(MapID, InstanceID,RoleID)
	--local x=map.GetMapScriptData(MapId,InstanceID,1,2)
	--local y=map.GetMapScriptData(MapId,InstanceID,1,3)
	--local z=map.GetMapScriptData(MapId,InstanceID,1,4)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 201004)
	msg.AddMsgEvent(MsgID, 9, x)
	msg.AddMsgEvent(MsgID, 9, y)
	msg.AddMsgEvent(MsgID, 9, z)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

--	map.MapCreateColCreature(MapID, InstanceID, 3040006, x, y, z,1,'')
	
--	map.MapCreateCreature(MapID, InstanceID, 3040002, 44150, y, 31000)


	
end
aux.RegisterCreatureEvent(3040002, 4, "c3040002_OnDie")
]]--