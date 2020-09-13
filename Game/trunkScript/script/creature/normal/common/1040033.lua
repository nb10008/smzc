--怪物喊话脚本

-- 怪物进入战斗喊话
function c1040033_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=30 then
		--30%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20103)
       end
end

aux.RegisterCreatureEvent(1040033, 2, "c1040033_OnEnterCombat")