--判断如果在S02不可使用
function tianzhen_canuse(MapID, InstanceID, SkillID, OwnerID, TargetID)
	local x,y,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	local a = 0
	if MapID == 2216111541 then
		if 826-20 <= x and x <= 826+20 and 832-20 <= z and z<= 832+20 then
			a = 1
		end
		if 509-20 <= x and x <= 509+20 and 528-20 <= z and z<= 528+20 then
			a = 1
		end
		if 382-20 <= x and x <= 382+20 and 162-20 <= z and z<= 162+20 then
			a = 1
		end
	end
	if MapID == 3424073512 or a == 1 or MapID == 3373741864 or MapID == 3390519080 or MapID == 3390518312 or MapID == 3390517544 then
		return 23
	end
	return 0
end
--注册是否可使用
aux.RegisterSkillEvent(1042501, 0, "tianzhen_canuse")
aux.RegisterSkillEvent(1042502, 0, "tianzhen_canuse")
aux.RegisterSkillEvent(1042503, 0, "tianzhen_canuse")
aux.RegisterSkillEvent(1042504, 0, "tianzhen_canuse")



--10秒天阵
function testAI_Cast1(MapID, InstanceID, SkillID, OwnerID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	local unvisiblecre = map.MapCreateCreature(MapID, InstanceID, 6010062, x, y, z)

	unit.AddBuff(MapID, InstanceID,unvisiblecre,1042201,unvisiblecre)


end

--15秒天阵
function testAI_Cast2(MapID, InstanceID, SkillID, OwnerID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	local unvisiblecre = map.MapCreateCreature(MapID, InstanceID, 6010063, x, y, z)

	unit.AddBuff(MapID, InstanceID,unvisiblecre,1042202,unvisiblecre)


end

--20秒天阵
function testAI_Cast3(MapID, InstanceID, SkillID, OwnerID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	local unvisiblecre = map.MapCreateCreature(MapID, InstanceID, 6010064, x, y, z)

	unit.AddBuff(MapID, InstanceID,unvisiblecre,1042203,unvisiblecre)


end

--25秒天阵
function testAI_Cast4(MapID, InstanceID, SkillID, OwnerID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	local unvisiblecre = map.MapCreateCreature(MapID, InstanceID, 6010065, x, y, z)

	unit.AddBuff(MapID, InstanceID,unvisiblecre,1042204,unvisiblecre)


end

aux.RegisterSkillEvent(1042501, 1, "testAI_Cast1")
aux.RegisterSkillEvent(1042502, 1, "testAI_Cast2")
aux.RegisterSkillEvent(1042503, 1, "testAI_Cast3")
aux.RegisterSkillEvent(1042504, 1, "testAI_Cast4")

