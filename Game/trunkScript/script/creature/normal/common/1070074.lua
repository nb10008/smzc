--怪物喊话脚本

-- 怪物进入战斗喊话
function c1070074_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20236)
       end
end

aux.RegisterCreatureEvent(1070074, 2, "c1070074_OnEnterCombat")