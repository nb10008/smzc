
function zuobiaodingwei_Use(MapID, InstanceID, TypeID, TargetID)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
    msg.AddMsgEvent(MsgID, 1, 304003)
	msg.AddMsgEvent(MsgID, 9, x)
	msg.AddMsgEvent(MsgID, 9, y)
	msg.AddMsgEvent(MsgID, 9, z)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)
end
aux.RegisterItemEvent(4500044, 1, "zuobiaodingwei_Use")

