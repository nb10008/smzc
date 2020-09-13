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


function c3040142_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10091)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040142, 2, "c3040142_OnEnterCombat")


function s5933201_Cast(MapID, InstanceID, SkillID, OwnerID)
	
	--cre.MonsterSay(MapID, InstanceID, OwnerID, 10038)
		
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
	
	local indexbuffId = math.random(1,4)
	if indexbuffId == 1 then
    		unit.AddBuff(MapID, InstanceID,TargetID,5933201,OwnerID)
	elseif indexbuffId == 2 then
		unit.AddBuff(MapID, InstanceID,TargetID,5933401,OwnerID)
	elseif indexbuffId == 3 then
		unit.AddBuff(MapID, InstanceID,TargetID,5933501,OwnerID)
	else
		unit.AddBuff(MapID, InstanceID,TargetID,5933301,OwnerID)
	end
	
end
aux.RegisterSkillEvent(5933201, 1, "s5933201_Cast")



function c3040142_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10092)
	
	map.OpenCloseDoor(MapID, InstanceID, 2219, 1)

	map.MapCreateCreature(MapID, InstanceID, 3040141, 149, 3840, 897)
	map.MapCreateCreature(MapID, InstanceID, 3040143, 749, 3840, 1116)


	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100112)
	--门打开
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040142, 4, "c3040142_OnDie")
