---mapscriptdata[1] 记录进副本后是否已刷出怪物
----[2]记录3040008是否被杀死
----[3]记录3040009是否被杀死
----[4]记录是否有过包围。
----[5]记录是否有埋伏。
----[6]记录3040010是否被杀死

function s5051001_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266371662 then
		cre.MonsterSay(MapID, InstanceID, TargetID, 10028)
		local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
		map.MapCreateCreature(MapID, InstanceID, 3040031, x+10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040032, x-10, y, z+10)
		map.MapCreateCreature(MapID, InstanceID, 3040033, x+10, y, z-10)
		map.MapCreateCreature(MapID, InstanceID, 3040034, x-10, y, z-10)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    		--同步仇恨给范围为40个格子，高度为20的有方怪物
   		 g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 100, 20)
	end
end
aux.RegisterSkillEvent(5051001, 1, "s5051001_Cast")


function c3040010_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	-----怪物喊话 3040011出现了
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100042)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10016)
	map.MapCreateCreature(MapID, InstanceID, 3040004, 393, 5862, 793)
	map.MapCreateCreature(MapID, InstanceID, 3040011, 419, 5913, 794)
	map.MapCreateCreature(MapID, InstanceID, 3040004, 420, 5862, 767)
	map.MapCreateCreature(MapID, InstanceID, 3040004, 446, 5862, 794)
	map.MapCreateCreature(MapID, InstanceID, 3040004, 419, 5862, 820)
	
	map.MapCreateCreature(MapID, InstanceID, 3040006, 393, 5793, 768)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 443, 5793, 766)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 446, 5793, 820)
	map.MapCreateCreature(MapID, InstanceID, 3040006, 393, 5793, 820)
	map.SetMapScriptData(MapID, InstanceID, 1, 6, 1)

end

aux.RegisterCreatureEvent(3040010, 4, "c3040010_OnDie")