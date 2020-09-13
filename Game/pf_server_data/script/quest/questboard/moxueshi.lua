--魔血巨石任务

--血魔巨石的地点
moxue_NPC = {}
moxue_NPC[1] = {mapid=3424072488, npcid=1093009, x=896, y=6110, z=1009, monid=0, time=0}--西洲幻林
moxue_NPC[2] = {mapid=3424072488, npcid=1093009, x=1968, y=3100, z=737, monid=0, time=0}--西洲幻林
moxue_NPC[3] = {mapid=3424072232, npcid=1093009, x=700, y=12030, z=2056, monid=0, time=0}--南洲迷沼
moxue_NPC[4] = {mapid=3424072232, npcid=1093009, x=1889, y=3445, z=960, monid=0, time=0}--南洲迷沼
moxue_NPC[5] = {mapid=3424075048, npcid=1093009, x=1684, y=10538, z=2279, monid=0, time=0}--东洲冥山
moxue_NPC[6] = {mapid=3424075048, npcid=1093009,  x=784, y=10080, z=2563, monid=0, time=0}--东洲冥山

--炸弹调查
function zhadan_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local FreeSize = role.GetBagFreeSize(RoleID)
	local bRe = 0
	local i = role.GetRoleItemNum(RoleID, 4010244)--有炸弹钥匙
	if i >= 1 then 
		if(FreeSize < 1) then
			bRet = 40
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4010229, 1, -1, 8, 420)
		end
		return bRet
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 405025)--没有炸弹箱钥匙，不能开启炸弹箱。
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
	end
end

--注册
aux.RegisterCreatureEvent(6011024,6,"zhadan_OnInvest")

--交付任务时候删除物品
function zhadan_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local i = role.GetRoleItemNum(RoleID,4010244)
	if i > 0 then
		role.RemoveFromRole(MapID, InstanceID, RoleID,4010244, 1, 402)
	end

end
aux.RegisterQuestEvent(60102,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60103,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60104,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60105,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60106,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60107,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60108,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60109,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60110,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60111,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60112,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60113,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60114,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60115,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60116,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60117,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60118,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60119,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60120,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60121,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60122,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60123,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60124,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60125,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60126,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60127,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60128,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60129,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60130,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60131,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60132,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60133,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60134,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60135,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60136,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60137,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60138,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60139,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60140,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60141,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60142,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60143,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60144,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60145,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60146,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60147,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60148,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60149,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60150,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60151,11,"zhadan_OnComplete")
aux.RegisterQuestEvent(60152,11,"zhadan_OnComplete")

--炸弹能否使用
function zhadan_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false
	local a = moxue_NPC[1].monid
	local b = moxue_NPC[2].monid
	local c = moxue_NPC[3].monid
	local d = moxue_NPC[4].monid
	local e = moxue_NPC[5].monid
	local f = moxue_NPC[6].monid
	
	if a == 0 and b == 0 and c == 0 and d == 0 and e == 0 and f == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 26, 405027)--周围没有魔血巨石。不能使用天界爆弹
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 32 ,false
	else
		if MapID ~= 3424072488 and MapID ~= 3424072232 and MapID ~= 3424075048 then
			bRet = 43
		else--如果不在这个坐标范围，则不能使用
			if MapID == 3424072488 and (x < moxue_NPC[1].x - 10 or x > moxue_NPC[1].x + 10 or z < moxue_NPC[1].z - 10 or z > moxue_NPC[1].z + 10) and (x < moxue_NPC[2].x - 10 or x > moxue_NPC[2].x + 10 or z < moxue_NPC[2].z - 10 or z > moxue_NPC[2].z + 10) then
				bRet = 43
			elseif MapID == 3424072232 and (x < moxue_NPC[3].x - 10 or x > moxue_NPC[3].x + 10 or z < moxue_NPC[3].z - 10 or z > moxue_NPC[3].z + 10) and (x < moxue_NPC[4].x - 10 or x > moxue_NPC[4].x + 10 or z < moxue_NPC[4].z - 10 or z > moxue_NPC[4].z + 10) then
				bRet = 43
			elseif MapID == 3424075048 and (x < moxue_NPC[5].x - 10 or x > moxue_NPC[5].x + 10 or z < moxue_NPC[5].z - 10 or z > moxue_NPC[5].z + 10) and (x < moxue_NPC[6].x - 10 or x > moxue_NPC[6].x + 10 or z < moxue_NPC[6].z - 10 or z > moxue_NPC[6].z + 10) then
				bRet = 43
			end
		end
		return bRet, bIgnore
	end
end
aux.RegisterItemEvent(4010229, 0, "zhadan_CanUse")

--炸弹使用
function zhadan_OnUse(MapID, InstanceID, TypeID, RoleID)
	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local monid = 0
	if MapID == 3424072488 then
		local k1 = (moxue_NPC[1].x - x)^2+(moxue_NPC[1].z - z)^2
		local k2 = (moxue_NPC[2].x - x)^2+(moxue_NPC[2].z - z)^2
		if k1 < k2 then
		    monid = moxue_NPC[1].monid
		else
		    monid = moxue_NPC[2].monid
		end
	elseif MapID == 3424072232 then
		local k3 = (moxue_NPC[3].x - x)^2+(moxue_NPC[3].z - z)^2
		local k4 = (moxue_NPC[4].x - x)^2+(moxue_NPC[4].z - z)^2
		if k3 < k4 then
		    monid = moxue_NPC[3].monid
		else
		    monid = moxue_NPC[4].monid
		end
	elseif MapID == 3424075048 then
		local k5 = (moxue_NPC[5].x - x)^2+(moxue_NPC[5].z - z)^2
		local k6 = (moxue_NPC[6].x - x)^2+(moxue_NPC[6].z - z)^2
		if k5 < k6 then
		    monid = moxue_NPC[5].monid
		else
		    monid = moxue_NPC[6].monid
		end
	end
	unit.AddBuff(MapID, InstanceID, monid, 6935901, monid) --减血1W--1093009魔血巨石
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 405003)--魔血巨石被天界爆弹所伤，体力下降，继续努力
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterItemEvent(4010229, 1, "zhadan_OnUse")

---魔血死亡
function moxue_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	if MapID == 3424072488 then
		local k1 = (moxue_NPC[1].x - x)^2+(moxue_NPC[1].z - z)^2
		local k2 = (moxue_NPC[2].x - x)^2+(moxue_NPC[2].z - z)^2
		if k1 < k2 then
			moxue_NPC[1].monid = 0 
			moxue_NPC[1].time = 10	
			map.MapCreateCreature(MapID, InstanceID, 1093010, moxue_NPC[1].x, moxue_NPC[1].y, moxue_NPC[1].z )
		else
			moxue_NPC[2].monid = 0 
			moxue_NPC[2].time = 10
			map.MapCreateCreature(MapID, InstanceID, 1093010, moxue_NPC[2].x, moxue_NPC[2].y, moxue_NPC[2].z )
		end
	elseif MapID == 3424072232 then
		local k3 = (moxue_NPC[3].x - x)^2+(moxue_NPC[3].z - z)^2
		local k4 = (moxue_NPC[4].x - x)^2+(moxue_NPC[4].z - z)^2
		if k3 < k4 then
			moxue_NPC[3].monid = 0 
			moxue_NPC[3].time = 10
			map.MapCreateCreature(MapID, InstanceID, 1093010, moxue_NPC[3].x, moxue_NPC[3].y, moxue_NPC[3].z )
		else
			moxue_NPC[4].monid = 0 
			moxue_NPC[4].time = 10
			map.MapCreateCreature(MapID, InstanceID, 1093010, moxue_NPC[4].x, moxue_NPC[4].y, moxue_NPC[4].z )
		end
	elseif MapID == 3424075048 then
		local k5 = (moxue_NPC[5].x - x)^2+(moxue_NPC[5].z - z)^2
		local k6 = (moxue_NPC[6].x - x)^2+(moxue_NPC[6].z - z)^2
		if k5 < k6 then
			moxue_NPC[5].monid = 0 
			moxue_NPC[5].time = 10
			map.MapCreateCreature(MapID, InstanceID, 1093010, moxue_NPC[5].x, moxue_NPC[5].y, moxue_NPC[5].z )
		else
			moxue_NPC[6].monid = 0 
			moxue_NPC[6].time = 10
			map.MapCreateCreature(MapID, InstanceID, 1093010, moxue_NPC[6].x, moxue_NPC[6].y, moxue_NPC[6].z )
		end
	end
end
aux.RegisterCreatureEvent(1093009,4,"moxue_OnDie")
