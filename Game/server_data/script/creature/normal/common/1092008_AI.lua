--1092008	幻妖姬阿嘉莎
--5108201	怪物技能	怪物AI技能	迷幻气

function s5108201_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a > 50 then
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20259)
	end
end
aux.RegisterSkillEvent(5108201, 1, "s5108201_Cast")


