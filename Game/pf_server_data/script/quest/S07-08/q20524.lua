--神之分身：20524
--需求1：调查搜神星盘6010027，刷出怪物幻神的分身1060058和火神的分身1060059。


function x20524_OnInvest(MapID, InstanceID, questID, RoleID,CreatureTypeID)

	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	   if m == nil then
		m = 0
	   end
	if m < 4 then
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1060058, x+2, y, z+2)
	map.SetMapScriptData(MapID,InstanceID,1,1,m+1)
	end
	if map.GetMapScriptData(MapID,InstanceID,1,1) >= 4 then
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 148)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	bRet = 32
	end
      
        local n = map.GetMapScriptData(MapID,InstanceID,1,1)
	   if n == nil then
		n = 0
	   end
	if n < 4 then
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1060059, x-2, y, z-2)
	map.SetMapScriptData(MapID,InstanceID,1,1,n+1)
	end
	if map.GetMapScriptData(MapID,InstanceID,1,1) >= 4 then
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 148)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	bRet = 32
	end

end


function c1060058_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,a-1)

end

function c1060059_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,a-1)

end

aux.RegisterQuestEvent(20524, 9, "x20524_OnInvest")
aux.RegisterCreatureEvent(1060058,4,"c1060058_OnDie")
aux.RegisterCreatureEvent(1060059,4,"c1060059_OnDie")


