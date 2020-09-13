--1092007	堕落妖精贝妮嘉
--5107101	怪物技能	怪物AI技能	兴奋秘术

function s5107101_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a > 50 then
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20258)
	end
end
aux.RegisterSkillEvent(5107101, 1, "s5107101_Cast")


