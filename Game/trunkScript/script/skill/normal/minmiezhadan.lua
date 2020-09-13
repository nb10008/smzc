--ãýÃðÕ¨µ¯
function s2101601_zhaDanCast(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(100)
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	if r < 95 then
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099022, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
		cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 1, 1)
	else
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099023, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
	end

end
aux.RegisterSkillEvent(2101601, 1, "s2101601_zhaDanCast")

function s2101602_zhaDanCast(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(100)
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	if r < 95 then
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099022, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
		cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 1, 2)
	else
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099023, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
	end

end
aux.RegisterSkillEvent(2101602, 1, "s2101602_zhaDanCast")

function s2101603_zhaDanCast(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(100)
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	if r < 95 then
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099022, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
		cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 1, 3)
	else
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099023, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
	end

end
aux.RegisterSkillEvent(2101603, 1, "s2101603_zhaDanCast")

function s2101604_zhaDanCast(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(100)
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	if r < 95 then
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099022, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
		cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 1, 4)
	else
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099023, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
	end

end
aux.RegisterSkillEvent(2101604, 1, "s2101604_zhaDanCast")

function s2101605_zhaDanCast(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(100)
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	if r < 95 then
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099022, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
		cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 1, 5)
	else
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099023, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
	end

end
aux.RegisterSkillEvent(2101605, 1, "s2101605_zhaDanCast")

function s2101606_zhaDanCast(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(100)
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	if r < 95 then
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099022, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
		cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 1, 6)
	else
		local cid = map.MapCreateColCreature(MapID, InstanceID, 1099023, x, y, z, 1, "")
		unit.AddBuff(MapID, InstanceID, cid, 3506001, cid)
	end

end
aux.RegisterSkillEvent(2101606, 1, "s2101606_zhaDanCast")


function c1099022_OnDie(MapID, InstanceID, TargetID, TypeID, KillerID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local bid = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1) + 3506100
	local cid = map.MapCreateColCreature(MapID, InstanceID, 1099024, x, y, z, 1, "")
	unit.SetState(MapID, InstanceID, TargetID, 0)
	unit.AddBuff(MapID, InstanceID, cid, bid, cid)

end
aux.RegisterCreatureEvent(1099022, 4, "c1099022_OnDie")

function c1099023_OnDie(MapID, InstanceID, TargetID, TypeID, KillerID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local cid = map.MapCreateColCreature(MapID, InstanceID, 1099024, x, y, z, 1, "")
	unit.SetState(MapID, InstanceID, TargetID, 0)
	unit.AddBuff(MapID, InstanceID, cid, 3506301, cid)

end
aux.RegisterCreatureEvent(1099023, 4, "c1099023_OnDie")
