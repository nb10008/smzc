---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死
---mapscriptdata[11] --记录冰龙是否死亡
---mapscriptdata[12] --记录火龙是否死亡
---mapscriptdata[13] --记录天界boss是否死亡
---mapscriptdata[14] --记录冥界boss是否死亡
function c3040250_OnEnterCombat(MapID, InstanceID, CreatureID)


	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040250, 2, "c3040250_OnEnterCombat")

function c3040250_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	local temp = map.GetMapScriptData(MapID,InstanceID,1,14)
	if(temp == 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100175)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		local baozang = map.MapCreateCreature(MapID, InstanceID, 6011031, 399, 1151, 903)
		map.MapCreateCreature(MapID, InstanceID, 6011032, 399, 1151, 894)
		map.OpenCloseDoor(MapID, InstanceID, 889, 1)
		map.OpenCloseDoor(MapID, InstanceID, 890, 1)
		table.insert(tianjiebaozang, baozang)
	else
		local mingjieBOss = map.GetMapScriptData(MapID,InstanceID,1,24)
		unit.CancelBuff(MapID, InstanceID, mingjieBOss, 5942601)
		unit.AddBuff(MapID, InstanceID, mingjieBOss,5942301,mingjieBOss)
	end
	map.SetMapScriptData(MapID,InstanceID,1,13,1)
	map.SetMapScriptData(MapID,InstanceID,1,23,0)




end
aux.RegisterCreatureEvent(3040250, 4, "c3040250_OnDie")


function c3040250_On_Leave_Combat(MapID, InstanceID, CreatureID)
	local temp = map.GetMapScriptData(MapID,InstanceID,1,14)
	if(temp == 1) then
		local mingjieBOss = map.MapCreateCreature(MapID, InstanceID, 3040251, 330, 1151, 855)
		map.SetMapScriptData(MapID,InstanceID,1,24,mingjieBOss)
		map.SetMapScriptData(MapID,InstanceID,1,14,0)
	end
	local tianjieBOss = map.GetMapScriptData(MapID,InstanceID,1,23)
	unit.AddBuff(MapID, InstanceID, tianjieBOss,5942501,tianjieBOss)
	unit.CancelBuff(MapID, InstanceID, tianjieBOss, 5942301)
end

aux.RegisterCreatureEvent(3040250, 3, "c3040250_On_Leave_Combat")
