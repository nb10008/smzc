----scriptdata[1]记录3040018是否死亡
----scriptdata[2]记录3040019是否死亡
----scriptdata[3]记录是否刷过4个幻象
----scriptdata[4]记录3040020是否死亡，死亡几次
----scriptdata[5]记录3040021是否死亡，死亡几次
----scriptdata[6]记录3040024是否死亡
----scriptdata[7]记录3040025是否死亡
----scriptdata[8]标记宝箱
----scriptdata[9]标记进副本是否刷怪


function s5054502_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371918 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10038)
		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		map.MapCreateCreature(MapID, InstanceID, 3040029, x+10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040029, x-10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040030, x+10, y, z-10)
		map.MapCreateCreature(MapID, InstanceID, 3040030, x-10, y, z-10)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--同步仇恨给范围为40个格子，高度为20的有方怪物
   		g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 100, 20)
	end
end
aux.RegisterSkillEvent(5054502, 1, "s5054502_Cast")


function c3040021_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 5)
	if a==0 then
		-------------------------addmsg还有一个幻象要杀
		map.SetMapScriptData(MapID, InstanceID, 1, 5, 1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100051)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		
	elseif a==1 then
		-------------------------幻象被杀死2次从而刷出真身
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100048)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.SetMapScriptData(MapID, InstanceID, 1, 5, 2)
		map.MapCreateCreature(MapID, InstanceID, 3040025, 955, 1889, 372)
	end	
end

aux.RegisterCreatureEvent(3040021, 4, "c3040021_OnDie")