--1092005	女武神泰蕾莎
--5105301	怪物技能	怪物AI技能	战争怒吼
function s5105301_Cast(MapID, InstanceID, SkillID, OwnerID)

	cre.MonsterSay(MapID, InstanceID, OwnerID, 20256)
end
aux.RegisterSkillEvent(5105301, 1, "s5105301_Cast")


