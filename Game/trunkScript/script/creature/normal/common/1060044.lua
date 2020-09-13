--怪物喊话脚本

-- 怪物进入战斗喊话
function c1060044_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20175)
       end
end

aux.RegisterCreatureEvent(1060044, 2, "c1060044_OnEnterCombat")