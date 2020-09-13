
function yewai_OnDisappear(MapID, InstanceID, CreatureID,CreatureTypeID)

	if CreatureTypeID == 1020022 or CreatureTypeID == 1020023 then

		local x,y,z = unit.GetPosition(MapID, InstanceID,CreatureID)
	
		map.MapCreateCreature(MapID, InstanceID, 1020020, x, y, z)
		
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100102)
		msg.AddMsgEvent(MsgID, 9, x)
		
		msg.AddMsgEvent(MsgID, 9, z)
		
		
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end
	

end

--aux.RegisterMapEvent("s04", 18, "yewai_OnDisappear")
aux.RegisterMapEvent("s03", 18, "yewai_OnDisappear")

--[[
function c1020022_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	

		local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	
		map.MapCreateCreature(MapID, InstanceID, 1020020, x, y, z)
		
		msg.AddMsgEvent(MsgID, 26, 201004)
		msg.AddMsgEvent(MsgID, 9, x)
		msg.AddMsgEvent(MsgID, 9, y)
		msg.AddMsgEvent(MsgID, 9, z)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	
	

end


aux.RegisterCreatureEvent(1020022, 13, "c1020022_OnDisappear")
]]--