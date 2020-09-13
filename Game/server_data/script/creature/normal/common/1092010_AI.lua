--1092010	魔女奥丽菲娅
--5110101	怪物技能	怪物AI技能	黑环
--5110401	怪物技能	怪物AI技能	能量爆发

function s5110101_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a > 50 then
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20260)
	end
end
aux.RegisterSkillEvent(5110101, 1, "s5110101_Cast")


function s5110401_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a > 50 then
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20260)
	end
end
aux.RegisterSkillEvent(5110401, 1, "s5110401_Cast")
