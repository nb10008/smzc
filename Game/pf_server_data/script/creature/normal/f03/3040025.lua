----scriptdata[1]记录3040018是否死亡
----scriptdata[2]记录3040019是否死亡
----scriptdata[3]记录是否刷过4个幻象
----scriptdata[4]记录3040020是否死亡，死亡几次
----scriptdata[5]记录3040021是否死亡，死亡几次
----scriptdata[6]记录3040024是否死亡
----scriptdata[7]记录3040025是否死亡
----scriptdata[8]标记宝箱
----scriptdata[9]标记进副本是否刷怪


function s5054501_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371918 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10037)
		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		map.MapCreateCreature(MapID, InstanceID, 3040027, x+10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040027, x-10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040028, x+10, y, z-10)
		map.MapCreateCreature(MapID, InstanceID, 3040028, x-10, y, z-10)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--同步仇恨给范围为40个格子，高度为20的有方怪物
   		 g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 100, 20)
	end
end
aux.RegisterSkillEvent(5054501, 1, "s5054501_Cast")




function c3040025_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10031)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040025, 2, "c3040025_OnEnterCombat")

function c3040025_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	cre.MonsterSay(MapID, InstanceID, TargetID, 10032)
	map.SetMapScriptData(MapID, InstanceID, 1, 7, 1)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 6)
	if a==1 then
		-------------------------addmsg刷出3040023
		map.MapCreateCreature(MapID, InstanceID, 3040023, 853, 1381, 292)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 843, 1381, 282)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 843, 1381, 302)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 863, 1381, 282)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 863, 1381, 302)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100049)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end	
	
end

aux.RegisterCreatureEvent(3040025, 4, "c3040025_OnDie")