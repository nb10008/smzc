--怪物喊话脚本

-- 怪物进入战斗喊话
function c1050043_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20146)
       end
end

aux.RegisterCreatureEvent(10500243, 2, "c1050043_OnEnterCombat")