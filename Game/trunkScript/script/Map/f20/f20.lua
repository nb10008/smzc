function f20_Creature()
	map.CreateInstance(1232817742, 1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 444456) --妖精秘境之门已经出现在北洲天雪的【&lt;p1&gt;，&lt;p2&gt;】,各种妖精在里面等着你?
	msg.AddMsgEvent(MsgID, 9, 2642)
	msg.AddMsgEvent(MsgID, 9, 2557)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end



function f20_delete()
	map.DeleteInstance(1232817742, 1)
end

function f20_MonsterCreate(curhour,curmin)
	map.MapCreateColCreature(1232817742, 1, 3040367,1036,2297,1145, 1)
	map.MapCreateColCreature(1232817742, 1, 3040368,1036,2297,1275, 1)
	map.MapCreateColCreature(1232817742, 1, 3040369,1036,2297,970, 1)
end



--玩家在活动中掉线后上线
function f20_CanEnterWhenOnline(MapID, RoleID)
	--修正玩家的上线地图和坐标todo
	return 3424073512,610,23094,669
end

aux.RegisterMapEvent("f20",  12, "f20_CanEnterWhenOnline")

-------------------------------------------------------------------------------------------------------
--找到地图的出口和坐标
function f20_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512,610,23094,669
end

aux.RegisterMapEvent("f20",  13, "f20_GetExporMapAndCoord")


--找到最合适的副本实例
function f20_GetOnePerfectMap(MapID, InstanceID, RoleID)
	--local curhour = tonumber(os.date("%H"))--小时

	return 1

end
aux.RegisterMapEvent("f20",  14, "f20_GetOnePerfectMap")



--检测玩家是否能够进入活动场景
function f20_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("f20",  15, "f20_CanEnter")


