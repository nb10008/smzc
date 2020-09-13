--怪物喊话脚本

-- 怪物进入战斗喊话
function c1060055_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20183)
       end
end

aux.RegisterCreatureEvent(1060055, 2, "c1060055_OnEnterCombat")