--
function bianshenjineng(MapID, InstanceID, TypeID, TargetID)

    if role.GetRoleSex(TargetID) == 1 then
		unit.AddBuff(MapID, InstanceID, TargetID, 9004601, TargetID)
	else
		unit.AddBuff(MapID, InstanceID, TargetID, 9004801, TargetID)
	end

	local k = role.GetRoleScriptData(TargetID, 1, RoleDataType["God_Quest"])
	role.SetRoleScriptData(TargetID, 1, RoleDataType["God_Quest"], math.floor(k/100)*100+k%10+10)
    if role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 40060) then
		role.ModSpecialTargetValue(MapID, InstanceID, TargetID, 40060, 2)
	end
end
aux.RegisterSkillEvent(2300001, 1, "bianshenjineng")



function bianshenjineng_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
    if role.GetRoleClergy(MapID, InstanceID, RoleID) ~= nil and role.GetRoleClergy(MapID, InstanceID, RoleID) == 0 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 442051)		--晋升神职后才能使用神化技能
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
        return 999
	else
	    return 0
	end

end
aux.RegisterSkillEvent(2300001, 0, "bianshenjineng_CanUse")
