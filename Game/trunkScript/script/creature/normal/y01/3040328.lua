----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040306是否死亡 0未死亡 1 死亡
----scriptdata[2]标记3040307是否死亡 0未死亡 1 死亡
----scriptdata[3]标记3040332是否存在 0不存在 1 存在

----scriptdata[4]标记3040317是否死亡 0未死亡 1 死亡
----scriptdata[5]标记3040318是否死亡 0未死亡 1 死亡
----scriptdata[6]标记3040319是否死亡 0未死亡 1 死亡
----scriptdata[7]记录3040329的id






function s5936601_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==3870157173 then
		
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100144)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


		
		
		local bossid = map.GetMapScriptData(MapID,InstanceID,1,7)

	 	unit.CancelBuff(MapID, InstanceID, bossid, 5938001)
	end

end
aux.RegisterSkillEvent(5936601, 1, "s5936601_Cast")



function s5936701_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==3870157173 then
		

		local x,y,z = unit.GetPosition(MapID, InstanceID,OwnerID)

		map.MapCreateCreature(MapID, InstanceID, 3040327, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040327, x-5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040327, x+5, y, z-5)
		map.MapCreateCreature(MapID, InstanceID, 3040327, x-5, y, z-5)
		
	end

end
aux.RegisterSkillEvent(5936701, 1, "s5936701_Cast")



--[[
function c3040328_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10054)
	

end

aux.RegisterCreatureEvent(3040328, 2, "c3040328_OnEnterCombat")



function c3040328_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10055)

end

aux.RegisterCreatureEvent(3040328, 4, "c3040328_OnDie")

]]--

