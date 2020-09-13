--怪物喊话脚本

-- 怪物进入战斗喊话
function c1050040_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20144)
       end
end

aux.RegisterCreatureEvent(10500240, 2, "c1050040_OnEnterCombat")