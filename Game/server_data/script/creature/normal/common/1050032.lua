--怪物喊话脚本

-- 怪物进入战斗喊话
function c1050032_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20137)
       end
end

aux.RegisterCreatureEvent(10500232, 2, "c1050032_OnEnterCombat")