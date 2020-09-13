----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040306是否死亡 0未死亡 1 死亡
----scriptdata[2]标记3040307是否死亡 0未死亡 1 死亡
----scriptdata[3]标记3040332是否存在 0不存在 1 存在


function c3040332_OnDisappear(MapID, InstanceID, CreatureID,CreatureTypeID)
	if CreatureTypeID == 3040332 then
		map.SetMapScriptData(MapID,InstanceID,1,3,0)
		local yanshiisdead = map.GetMapScriptData(MapID,InstanceID,1,1)
		local yanmoisdead = map.GetMapScriptData(MapID,InstanceID,1,2)
		if yanshiisdead == 1 and yanmoisdead == 0 then
			map.MapCreateCreature(MapID, InstanceID, 3040306, 326, 50, 56)
			map.SetMapScriptData(MapID,InstanceID,1,1,0)
			--复活3040306

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100138)
			
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

			
		elseif yanshiisdead == 0 and yanmoisdead == 1 then
			map.MapCreateCreature(MapID, InstanceID, 3040307, 342, 50, 66)
			map.SetMapScriptData(MapID,InstanceID,1,2,0)
			--复活3040307

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100137)
			--速度杀死另一个傀儡
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		end
	end
end

aux.RegisterMapEvent("y01", 18, "c3040332_OnDisappear")