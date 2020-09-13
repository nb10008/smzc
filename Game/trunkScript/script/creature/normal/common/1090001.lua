--怪物喊话脚本

-- 怪物进入战斗喊话
function c1090001_OnEnterCombat(MapID, InstanceID, CreatureID)
           cre.MonsterSay(MapID, InstanceID, CreatureID, 30001)
end

aux.RegisterCreatureEvent(1090001, 2, "c1090001_OnEnterCombat")

