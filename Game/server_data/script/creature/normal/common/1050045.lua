--怪物喊话脚本

-- 怪物进入战斗喊话
function c1050045_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20147)
       end
end

aux.RegisterCreatureEvent(10500245, 2, "c1050045_OnEnterCombat")