---mapscriptdata[1] 记录进副本后是否已刷出怪物
----[2]记录3040008是否被杀死
----[3]记录3040009是否被杀死
----[4]记录是否有过包围。
----[5]记录是否有埋伏。
----[6]记录3040010是否被杀死。


function c3040008_OnEnterCombat(MapID, InstanceID, CreatureID)


	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
------------------------
	cre.MonsterSay(MapID, InstanceID, CreatureID, 10006)
	local baowei=map.GetMapScriptData(MapID, InstanceID, 1,4)
 

	if baowei==0 then
		map.MapCreateCreature(MapID, InstanceID, 3040002, x+10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040007, x-10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040002, x+10, y, z-10)
		map.MapCreateCreature(MapID, InstanceID, 3040006, x-10, y, z-10)
		map.SetMapScriptData(MapID,InstanceID,1,4,1)
	end	

	local TargetID2 = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID2, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040008, 2, "c3040008_OnEnterCombat")



function c3040008_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local b = map.GetMapScriptData(MapID, InstanceID, 1, 3)
	map.SetMapScriptData(MapID, InstanceID, 1, 2, 1)
	
	if b==1 then
		-----3040009已死
		cre.MonsterSay(MapID, InstanceID, TargetID, 10008)
		map.MapCreateCreature(MapID, InstanceID, 3040010, 577, 4295, 147)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100041)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		-----3040009未死
		cre.MonsterSay(MapID, InstanceID, TargetID, 10007)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100039)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end
	local x,y,z=784,6703,790
	map.MapCreateCreature(MapID, InstanceID, 3040007, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 3040006, x+7, y, z-5)
	map.MapCreateCreature(MapID, InstanceID, 3040004, x-7, y, z-5)



end

aux.RegisterCreatureEvent(3040008, 4, "c3040008_OnDie")



