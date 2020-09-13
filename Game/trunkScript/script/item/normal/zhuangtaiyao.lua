
--续航药水
function I3010043_onUse(MapID, InstanceID, TypeID, TargetID)
--得到玩家等级
	local n = role.GetRoleLevel( MapID , InstanceID , TargetID)
	if n > 100 then
		n = 100
	end
--为玩家添加对应buff
	unit.AddBuff( MapID , InstanceID , TargetID , 2004101+100*(n-1) , TargetID )
end

aux.RegisterItemEvent(3010043, 1 , "I3010043_onUse")


--物理攻击药水
function I3010044_onUse(MapID, InstanceID, TypeID, TargetID)
--得到玩家等级
	local n = role.GetRoleLevel( MapID , InstanceID , TargetID)
	if n > 100 then
		n = 100
	end
--为玩家添加对应buff
	unit.AddBuff( MapID , InstanceID , TargetID , 2014101+100*(n-1) , TargetID )
end

aux.RegisterItemEvent(3010044, 1 , "I3010044_onUse")


--法术攻击药水
function I3010045_onUse(MapID, InstanceID, TypeID, TargetID)
--得到玩家等级
	local n = role.GetRoleLevel( MapID , InstanceID , TargetID)
	if n > 100 then
		n = 100
	end
--为玩家添加对应buff
	unit.AddBuff( MapID , InstanceID , TargetID , 2024101+100*(n-1) , TargetID )
end

aux.RegisterItemEvent(3010045, 1 , "I3010045_onUse")


--法术攻击药水
function I3010046_onUse(MapID, InstanceID, TypeID, TargetID)
--得到玩家等级
	local n = role.GetRoleLevel( MapID , InstanceID , TargetID)
	if n > 100 then
		n = 100
	end
--为玩家添加对应buff
	unit.AddBuff( MapID , InstanceID , TargetID , 2034101+100*(n-1) , TargetID )
end

aux.RegisterItemEvent(3010046, 1 , "I3010046_onUse")
