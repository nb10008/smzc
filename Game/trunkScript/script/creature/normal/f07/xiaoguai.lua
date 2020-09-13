----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记召唤术到第几级了
----scriptdata[2]标记3040141是否死亡
----scriptdata[3]标记3040143是否死亡
----scriptdata[4]标记3040150是否死亡
----scriptdata[5]标记3040151是否死亡
----scriptdata[6]记录3040152的ID
----scriptdata[7]标记3040164是否死亡
----scriptdata[8]标记3040165是否死亡
----scriptdata[9]标记3040166是否死亡
----scriptdata[10]标记是否刷过宝箱


function f07huayao_OnEnterCombat(MapID, InstanceID, CreatureID)

	local p = math.random(1,100)
	if p<15 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10083)
	elseif p<30 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10084)
	elseif p<45 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10085)
	end
	
end

aux.RegisterCreatureEvent(3040130, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040131, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040132, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040133, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040134, 2, "f07huayao_OnEnterCombat")
aux.RegisterCreatureEvent(3040135, 2, "f07huayao_OnEnterCombat")


function f07guyongbing_OnEnterCombat(MapID, InstanceID, CreatureID)

	local p = math.random(1,100)
	if p<15 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10086)
	elseif p<30 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10087)
	
	end
	
end

aux.RegisterCreatureEvent(3040136, 2, "f07guyongbing_OnEnterCombat")
aux.RegisterCreatureEvent(3040137, 2, "f07guyongbing_OnEnterCombat")
aux.RegisterCreatureEvent(3040138, 2, "f07guyongbing_OnEnterCombat")
aux.RegisterCreatureEvent(3040139, 2, "f07guyongbing_OnEnterCombat")

function f07weibing_OnEnterCombat(MapID, InstanceID, CreatureID)

	local p = math.random(1,100)
	if p<15 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10097)
	elseif p<30 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10098)
	elseif p<45 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10099)
	
	end
	
end

aux.RegisterCreatureEvent(3040144, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040145, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040146, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040147, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040148, 2, "f07weibing_OnEnterCombat")
aux.RegisterCreatureEvent(3040149, 2, "f07weibing_OnEnterCombat")

