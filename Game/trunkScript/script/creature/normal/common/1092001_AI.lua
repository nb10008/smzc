--1092001	雪之女王嘉迪卡拉
--5101201	怪物技能	怪物AI技能	冲锋

function s5101201_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a >50 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 20254)
	end
end
aux.RegisterSkillEvent(5101201, 1, "s5101201_Cast")


