---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死

function c3040238_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10117)
	--人马的命运注定是个悲剧

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040238, 2, "c3040238_OnEnterCombat")

function c3040238_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100128)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	
	map.MapCreateCreature(MapID, InstanceID, 3040247, 439, 1327, 370)
	
	

	
	

end
aux.RegisterCreatureEvent(3040238, 4, "c3040238_OnDie")
