--世界BOSS死亡，清空全局变量
function WorldBOSS_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	if TargetTypeID == 3040333 then
	    World_Boss_1 = 0
	elseif TargetTypeID == 3040334 then
	    World_Boss_4 = 0
	elseif TargetTypeID == 3040335 then
	    World_Boss_3 = 0
	elseif TargetTypeID == 3040336 then
	    World_Boss_2 = 0
	end
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 3040338, x, y, z)

	if tianxiafuliren ~= nil then
	    map.MapDeleteCreature(3424073512, -1, tianxiafuliren)
	end
	tianxiafuliren = map.MapCreateCreature(3424073512, -1, 5610054, 612, 23510, 588)--主城
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101,206492)      --天下福利人已经出现在斗战天城（612，588）处，快去它哪里领取兑换福利吧  612，23510，588
	msg.DispatchWorldMsgEvent(MsgID)
end

aux.RegisterCreatureEvent(3040333, 4, "WorldBOSS_OnDie")
aux.RegisterCreatureEvent(3040334, 4, "WorldBOSS_OnDie")
aux.RegisterCreatureEvent(3040335, 4, "WorldBOSS_OnDie")
aux.RegisterCreatureEvent(3040336, 4, "WorldBOSS_OnDie")

--福利人消失，清空变量
function c5610054_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    tianxiafuliren = nil
end
aux.RegisterCreatureEvent(5610054, 13, "c5610054_OnDisappear")
