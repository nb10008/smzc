---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死

---mapscriptdata[11] --记录冰龙是否死亡
---mapscriptdata[12] --记录火龙是否死亡
---mapscriptdata[13] --记录天界boss是否死亡
---mapscriptdata[14] --记录冥界boss是否死亡
---mapscriptdata[23] --记录天界boss的ID
---mapscriptdata[24] --记录冥界boss的ID
tianjiebaozang = {}  --记录天界宝箱的ID
function f08_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end


	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100121)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	-------------英勇的探险者们终于来到了魔族的王宫，即将面对的是铲除邪恶的最后战役，为了世界和平，战斗吧！

	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,0)
	if monsterhave==0 then


		--map.MapCreateCreature(MapID, InstanceID, 3040200, 893, 124, 238)

		local bossid = map.MapCreateCreature(MapID, InstanceID, 3040208, 857, 63, 326)
		--------第一个boss


		map.SetMapScriptData(MapID,InstanceID,1,0,1)
		map.SetMapScriptData(MapID,InstanceID,1,1,bossid)
		map.SetMapScriptData(MapID,InstanceID,1,11,0)  --记录冰龙是否死亡
		map.SetMapScriptData(MapID,InstanceID,1,12,0)  --记录火龙是否死亡
		map.SetMapScriptData(MapID,InstanceID,1,13,0)  --记录天界boss是否死亡
		map.SetMapScriptData(MapID,InstanceID,1,14,0)  --记录冥界boss是否死亡

	end

end

--注册
aux.RegisterMapEvent("f08", 2, "f08_OnPlayerEnter")




