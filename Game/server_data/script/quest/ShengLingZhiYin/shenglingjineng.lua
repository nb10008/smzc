function I3560001_Use(MapID, InstanceID, TypeID, RoleID)

	local shenglingSkill = role.IsLearnedSkill(RoleID, 59441) 
	if shenglingSkill == nil then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13,0)		--你已经获得圣灵召唤技能，可以用来召唤圣灵协助作战
		msg.AddMsgEvent(MsgID, 1,200008)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		role.AddSkill(RoleID, 5944101)
	end
end
aux.RegisterItemEvent(3560001, 1, "I3560001_Use")