--超级粘液炸弹
function i4810040_Usable(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 3390517288 then
		bRet = 43
	end
	return bRet, bIgnore

end
aux.RegisterItemEvent(4810040, 0, "i4810040_Usable")

function i4810040_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	map.MapCreateColCreature(MapID, InstanceID, 6040122, x, y, z, 1, "")

end
aux.RegisterItemEvent(4810040, 1, "i4810040_QuestUsable")


--兴奋炸弹
function G6040121_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID,5945801,RoleID)
end
aux.RegisterCreatureEvent(6040121, 4, "G6040121_OnDie")
--进入下马
function s65_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	
end

aux.RegisterMapEvent("s65", 2, "s53_OnPlayerEnter")
function i3070050_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID == 3390517288 then
		bRet = 32
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(3070050, 0, "i3070050_CanUse")
aux.RegisterItemEvent(3070050, 0, "i3070050_CanUse")