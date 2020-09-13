--怪物喊话脚本

-- 怪物进入战斗喊话
function c1050026_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20133)
       end
end

aux.RegisterCreatureEvent(10500226, 2, "c1050026_OnEnterCombat")