--怪物喊话脚本

-- 怪物进入战斗喊话
function c1040042_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=50 then
		--50%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20109)
       end
end

aux.RegisterCreatureEvent(1040042, 2, "c1040042_OnEnterCombat")