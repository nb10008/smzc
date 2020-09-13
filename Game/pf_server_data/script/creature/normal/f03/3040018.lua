----scriptdata[1]记录3040018是否死亡
----scriptdata[2]记录3040019是否死亡
----scriptdata[3]记录是否刷过4个幻象
----scriptdata[4]记录3040020是否死亡，死亡几次
----scriptdata[5]记录3040021是否死亡，死亡几次
----scriptdata[6]记录3040024是否死亡
----scriptdata[7]记录3040025是否死亡
----scriptdata[8]标记宝箱
----scriptdata[9]标记进副本是否刷怪

function c3040018_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10025)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040018, 2, "c3040018_OnEnterCombat")






function c3040018_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10029)

	map.SetMapScriptData(MapID, InstanceID, 1, 1, 1)	
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 2)	
	if a==1 then
		
		map.OpenCloseDoor(MapID, InstanceID, 3143, 1)
		
		------addmsg:通往雪地的大门敞开 新的挑战迎接你

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100045)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


		local position={}
		position[1]={x=409,y=1513,z=346}
		position[2]={x=469,y=1898,z=368}
		position[3]={x=566,y=1408,z=408}
		position[4]={x=807,y=941,z=383}
		position[5]={x=430,y=1798,z=251}
		position[6]={x=392,y=1408,z=396}
		position[7]={x=639,y=1804,z=252}
		position[8]={x=510,y=1821,z=201}
		
		swap={x=0,y=0,z=0}
		----------洗牌
		for k=1,8  do 
			local num=math.random(1,8)
			-----------------------------------swap=position[num]
			swap.x=position[num].x
			swap.y=position[num].y
			swap.z=position[num].z
			-----------------------------------position[num]=position[k]
			position[num].x=position[k].x
			position[num].y=position[k].y
			position[num].z=position[k].z
			------------------------------------position[k]=swap
			position[k].x=swap.x
			position[k].y=swap.y
			position[k].z=swap.z

		end
		----------随机选择4个位置刷幻象
		local b = map.GetMapScriptData(MapID, InstanceID, 1, 3)	
		if b==0 then

			map.MapCreateCreature(MapID, InstanceID, 3040020, position[1].x, position[1].y, position[1].z)
			map.MapCreateCreature(MapID, InstanceID, 3040020, position[2].x, position[2].y, position[2].z)
			map.MapCreateCreature(MapID, InstanceID, 3040021, position[3].x, position[3].y, position[3].z)
			map.MapCreateCreature(MapID, InstanceID, 3040021, position[4].x, position[4].y, position[4].z)
			--map.MapCreateCreature(MapID, InstanceID, 3040026, position[5].x, position[5].y, position[5].z)
			--map.MapCreateCreature(MapID, InstanceID, 3040026, position[6].x, position[6].y, position[6].z)
			--map.MapCreateCreature(MapID, InstanceID, 3040026, position[7].x, position[7].y, position[7].z)
			--map.MapCreateCreature(MapID, InstanceID, 3040026, position[8].x, position[8].y, position[8].z)
			map.SetMapScriptData(MapID, InstanceID, 1, 3, 1)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100044)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	end


end
aux.RegisterCreatureEvent(3040018, 4, "c3040018_OnDie")
