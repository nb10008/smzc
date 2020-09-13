--怪物喊话脚本

-- 怪物进入战斗喊话
function c1091004_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 30007)
end

aux.RegisterCreatureEvent(1091004, 2, "c1091004_OnEnterCombat")

