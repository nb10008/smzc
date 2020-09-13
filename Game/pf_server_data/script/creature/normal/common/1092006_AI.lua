--1092006	死亡契约露艾拉
--5106101	怪物技能	怪物AI技能	死神降临
function s5106101_Cast(MapID, InstanceID, SkillID, OwnerID)
	local a = math.random(1,100)
	if a > 50 then
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20257)
	end
end
aux.RegisterSkillEvent(5106101, 1, "s5106101_Cast")


