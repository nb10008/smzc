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


function c3040140_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10089)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040140, 2, "c3040140_OnEnterCombat")


function s5054505_Cast(MapID, InstanceID, SkillID, OwnerID)
	
	cre.MonsterSay(MapID, InstanceID, OwnerID, 10088)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	
	local crelvl = map.GetMapScriptData(MapID,InstanceID,1,1)

	if crelvl == 0 then
		map.MapCreateCreature(MapID, InstanceID, 3040155, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040159, x-5, y, z+5)
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
	elseif crelvl == 1 then
		map.MapCreateCreature(MapID, InstanceID, 3040156, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040160, x-5, y, z+5)
		map.SetMapScriptData(MapID,InstanceID,1,1,2)
	elseif crelvl == 2 then
		map.MapCreateCreature(MapID, InstanceID, 3040157, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040161, x-5, y, z+5)
		map.SetMapScriptData(MapID,InstanceID,1,1,3)
	else 
		map.MapCreateCreature(MapID, InstanceID, 3040158, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040162, x-5, y, z+5)
	end
			
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--同步仇恨给范围为30个格子，高度为20的有方怪物
   	g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 30, 20)
	
end
aux.RegisterSkillEvent(5054505, 1, "s5054505_Cast")



function c3040140_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10090)
	
	map.OpenCloseDoor(MapID, InstanceID, 2216, 1)
	map.MapCreateCreature(MapID, InstanceID, 3040142, 270, 3840, 613)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100111)
	--门打开
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040140, 4, "c3040140_OnDie")
