--原神出现：20635
--需求1：调查不寻常的岩石6010025，刷出怪物魔族密探1070092。


function x20635_OnInvest(MapID, InstanceID, questID, RoleID,CreatureTypeID)

	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	if m == nil then
		m = 0
	end
	if m < 4 then
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x+2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x-2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x-2, y, z-2)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x+2, y, z-2)	
	map.MapCreateCreature(MapID, InstanceID, 1070092, x-4, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x+4, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x, y, z-4)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x, y, z+4)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x+6, y, z+6)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x-6, y, z+6)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x-6, y, z-6)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x+6, y, z-6)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x-8, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x+8, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x, y, z-8)
	map.MapCreateCreature(MapID, InstanceID, 1070092, x, y, z+8)
	
	
	map.SetMapScriptData(MapID,InstanceID,1,1,m+16)
	end
	if map.GetMapScriptData(MapID,InstanceID,1,1) >= 16 then
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 148)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	bRet = 32
	end
end


function c1070092_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,a-1)

end

aux.RegisterQuestEvent(20635, 9, "x20635_OnInvest")
aux.RegisterCreatureEvent(1070092,4,"c1070092_OnDie")

