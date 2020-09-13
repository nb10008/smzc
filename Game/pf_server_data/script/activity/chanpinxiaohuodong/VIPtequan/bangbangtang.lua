function bangbangtang_OnUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)

		map.MapCreateCreature(MapID, -1, 1092010, x,y,z)
		map.MapCreateCreature(MapID, -1, 1092009, x+5,y,z)
        map.MapCreateCreature(MapID, -1, 1092008, x,y,z+5)
		map.MapCreateCreature(MapID, -1, 1092021, x+10,y,z)
        map.MapCreateCreature(MapID, -1, 1092005, x+10,y,z+5)
		map.MapCreateCreature(MapID, -1, 1092004, x+10,y,z+10)
        map.MapCreateCreature(MapID, -1, 1092003, x-5,y,z)
		map.MapCreateCreature(MapID, -1, 1092020, x-5,y,z-5)
        map.MapCreateCreature(MapID, -1, 1092006, x-10,y,z-10)
end
aux.RegisterItemEvent(4810032, 1, "bangbangtang_OnUse")

function bangbangtang_canUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--
	local map = {3424073256,3424071976,3424071720,3424072488,3424072232,3424075048}
	local temp = false
	for i = 1,#map do
		if MapID == map[i] then
			temp = true
		end
	end
	if(temp == false) then
		--提示场景使用错误
		bRet = 47
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4810032, 0, "bangbangtang_canUse")


