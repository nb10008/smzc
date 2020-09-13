--怪物喊话脚本

-- 怪物进入战斗喊话
function c1030019_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20081)
       end
end

aux.RegisterCreatureEvent(1030019, 2, "c1030019_OnEnterCombat")

