--神兵神器
function I4800214_Use(MapID, InstanceID, TypeID, RoleID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800227, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800214, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800215, 1, 101)
end
function I4800214_CanUse(MapID, InstanceID, TypeID, RoleID)
	if role.GetBagFreeSize(RoleID) < 1 then
        return 40, false
	else
		if role.GetRoleItemNum(RoleID, 4800214) > 0 and role.GetRoleItemNum(RoleID, 4800215) > 0 then
			return 0, false
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444311)				--缺少神器的另一部分
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 102, false
		end
	end
end
aux.RegisterItemEvent(4800214, 1, "I4800214_Use")
aux.RegisterItemEvent(4800214, 0, "I4800214_CanUse")
aux.RegisterItemEvent(4800215, 1, "I4800214_Use")
aux.RegisterItemEvent(4800215, 0, "I4800214_CanUse")

--玄翎神器
function I4800216_Use(MapID, InstanceID, TypeID, RoleID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800228, 1, 6, -1, 420)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800216, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800217, 1, 101)
end
function I4800216_CanUse(MapID, InstanceID, TypeID, RoleID)
	if role.GetBagFreeSize(RoleID) < 1 then
        return 40, false
	else
		if role.GetRoleItemNum(RoleID,4800216) > 0 and role.GetRoleItemNum(RoleID,4800217) > 0 then
			return 0, false
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444311)				--缺少神器的另一部分
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 102, false
		end
	end
end
aux.RegisterItemEvent(4800216, 1, "I4800216_Use")
aux.RegisterItemEvent(4800216, 0, "I4800216_CanUse")
aux.RegisterItemEvent(4800217, 1, "I4800216_Use")
aux.RegisterItemEvent(4800217, 0, "I4800216_CanUse")

--乾坤神器
function I4800218_Use(MapID, InstanceID, TypeID, RoleID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800229, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800218, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800219, 1, 101)
end
function I4800218_CanUse(MapID, InstanceID, TypeID, RoleID)
	if role.GetBagFreeSize(RoleID) < 1 then
        return 40, false
	else
		if role.GetRoleItemNum(RoleID,4800218) > 0 and role.GetRoleItemNum(RoleID,4800219) > 0 then
			return 0, false
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444311)				--缺少神器的另一部分
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 102, false
		end
	end
end
aux.RegisterItemEvent(4800218, 1, "I4800218_Use")
aux.RegisterItemEvent(4800218, 0, "I4800218_CanUse")
aux.RegisterItemEvent(4800219, 1, "I4800218_Use")
aux.RegisterItemEvent(4800219, 0, "I4800218_CanUse")

--仙音神器
function I4800220_Use(MapID, InstanceID, TypeID, RoleID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800230, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800220, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 4800221, 1, 101)
end
function I4800220_CanUse(MapID, InstanceID, TypeID, RoleID)
	if role.GetBagFreeSize(RoleID) < 1 then
        return 40, false
	else
		if role.GetRoleItemNum(RoleID,4800220) > 0 and role.GetRoleItemNum(RoleID,4800221) > 0 then
			return 0, false
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444311)				--缺少神器的另一部分
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 102, false
		end
	end
end
aux.RegisterItemEvent(4800220, 1, "I4800220_Use")
aux.RegisterItemEvent(4800220, 0, "I4800220_CanUse")
aux.RegisterItemEvent(4800221, 1, "I4800220_Use")
aux.RegisterItemEvent(4800221, 0, "I4800220_CanUse")

--4800222封印水晶
function I4800222_Use(MapID, InstanceID, TypeID, RoleID)
	if role.GetRoleItemNum(RoleID, 8019005) > 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 444327)      	--xxx启动了神器任务，10s后TA将挑战神器之灵
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchWorldMsgEvent(MsgID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444328)			--10秒后在将会出现神器之灵，你需要在2分钟内将其驯服
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		local cid = map.MapCreateColCreature(3424073512, -1, 5600086, 1, 1, 1, 1, "")
		cre.SetCreatureScriptData(3424073512, -1, cid, 1, 0, 3040380)
		cre.SetCreatureScriptData(3424073512, -1, cid, 1, 1, RoleID)
		canStartshenqi = 0
	elseif role.GetRoleItemNum(RoleID, 8029005) > 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 444327)      	--xxx启动了神器任务，1分钟后TA将挑战神器之灵
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchWorldMsgEvent(MsgID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444328)			--10秒后在将会出现神器之灵，你需要在2分钟内将其驯服
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		local cid = map.MapCreateColCreature(3424073512, -1, 5600086, 1, 1, 1, 1, "")
		cre.SetCreatureScriptData(3424073512, -1, cid, 1, 0, 3040381)
		cre.SetCreatureScriptData(3424073512, -1, cid, 1, 1, RoleID)
		canStartshenqi = 0
	elseif role.GetRoleItemNum(RoleID, 8039005) > 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 444327)      	--xxx启动了神器任务，1分钟后TA将挑战神器之灵
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchWorldMsgEvent(MsgID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444328)			--10秒后在将会出现神器之灵，你需要在2分钟内将其驯服
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		local cid = map.MapCreateColCreature(3424073512, -1, 5600086, 1, 1, 1, 1, "")
		cre.SetCreatureScriptData(3424073512, -1, cid, 1, 0, 3040383)
		cre.SetCreatureScriptData(3424073512, -1, cid, 1, 1, RoleID)
		canStartshenqi = 0
	elseif role.GetRoleItemNum(RoleID, 8049005) > 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 444327)      	--xxx启动了神器任务，1分钟后TA将挑战神器之灵
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchWorldMsgEvent(MsgID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444328)			--10秒后在将会出现神器之灵，你需要在2分钟内将其驯服
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		local cid = map.MapCreateColCreature(3424073512, -1, 5600086, 1, 1, 1, 1, "")
		cre.SetCreatureScriptData(3424073512, -1, cid, 1, 0, 3040382)
		cre.SetCreatureScriptData(3424073512, -1, cid, 1, 1, RoleID)
		canStartshenqi = 0
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444313)		--缺少对应的神器
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 102,false
	end
end
function I4800222_CanUse(MapID, InstanceID, TypeID, RoleID)
	if canStartshenqi == 1 then
		local x,y,z = unit.GetPosition(MapID, InstanceID, RoleID)
		if MapID == 3424073512 and (x >= 608 and x <= 618) and (z >= 846 and z <= 856) then
			if role.GetRoleItemNum(RoleID, 8019005) > 0 or role.GetRoleItemNum(RoleID, 8049005) > 0 or role.GetRoleItemNum(RoleID, 8039005) > 0 or role.GetRoleItemNum(RoleID, 8029005) > 0 then
				return 0,false
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 444313)		--缺少对应的神器
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 102,false
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444312)	--错误的位置
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 102,false
		end
	elseif canStartshenqi == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444314)					--当前正在有玩家进行神器任务
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 102,false
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444315)					--已经有玩家开启神器之灵，请下一个时间段再来开启
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 102,false
	end
end
aux.RegisterItemEvent(4800222, 1, "I4800222_Use")
aux.RegisterItemEvent(4800222, 0, "I4800222_CanUse")

function c5600086On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local cid = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
	local roleid = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local creid = map.MapCreateColCreature(3424073512, -1, cid, 613, 23510, 851, 1, "")
	unit.AddBuff(MapID, InstanceID, creid, 6939701, creid)
	map.MapCreateColCreature(3424073512, -1, 5600088, 2, 2, 2, 1, "")
	cre.SetCreatureScriptData(3424073512, -1, creid, 1, 0, cid)
	cre.SetCreatureScriptData(3424073512, -1, creid, 1, 1, roleid)
	local c1 = map.MapCreateColCreature(3424073512, -1, 3040384, 613, 23510, 871, 1, "")
	local c2 = map.MapCreateColCreature(3424073512, -1, 3040384, 633, 23510, 851, 1, "")
	local c3 = map.MapCreateColCreature(3424073512, -1, 3040384, 613, 23510, 831, 1, "")
	local c4 = map.MapCreateColCreature(3424073512, -1, 3040384, 593, 23510, 851, 1, "")
	cre.AddEnmity(3424073512, -1, c1, roleid, 1)
	cre.AddEnmity(3424073512, -1, c2, roleid, 1)
	cre.AddEnmity(3424073512, -1, c3, roleid, 1)
	cre.AddEnmity(3424073512, -1, c4, roleid, 1)
	--[[
	cre.SetCreatureScriptData(3424073512,  -1, c1, 1, 0, roleid)
	cre.SetCreatureScriptData(3424073512,  -1, c2, 1, 0, roleid)
	cre.SetCreatureScriptData(3424073512,  -1, c3, 1, 0, roleid)
	cre.SetCreatureScriptData(3424073512,  -1, c4, 1, 0, roleid)
	--]]
	local stepid = map.MapCreateColCreature(3424073512, -1, 5600084, 1, 1, 1, 1, "")
	cre.SetCreatureScriptData(3424073512,  -1, stepid, 1, 0, 30)
	cre.SetCreatureScriptData(3424073512,  -1, stepid, 1, 1, roleid)
end
aux.RegisterCreatureEvent(5600086, 13, "c5600086On_Disappear")

--神器之灵
function c3040380On_Die(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	local tempTable = {}
	tempTable[3040380] = 5600080
	tempTable[3040381] = 5600081
	tempTable[3040382] = 5600082
	tempTable[3040383] = 5600083
	for k,v in pairs(tempTable) do
		if CreatureTypeID == k then
			local roleID = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 1)
			local creid = map.MapCreateColCreature(3424073512, -1, v, 613, 23510, 851, 1, "")
			cre.SetCreatureScriptData(3424073512,  -1, creid, 1, 0, roleID)
			break
		end
	end
	canStartshenqi = -1
end
aux.RegisterCreatureEvent(3040380, 4, "c3040380On_Die")
aux.RegisterCreatureEvent(3040381, 4, "c3040380On_Die")
aux.RegisterCreatureEvent(3040382, 4, "c3040380On_Die")
aux.RegisterCreatureEvent(3040383, 4, "c3040380On_Die")

--检测完成状态
function c5600088On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	if canStartshenqi == 0 then
		canStartshenqi = 1
	end
end
aux.RegisterCreatureEvent(5600088, 13, "c5600088On_Disappear")

--6937501-3buff
function c5600080On_Talk(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID, TalkIndex)
	local theRole = cre.GetCreatureScriptData(MapID,  InstanceID, CreatureID, 1, 0)
	if theRole == RoleID then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444317) 			--确定要灌注神器吗？\n注意：背包中需要携带上古神器以及被封印的水晶
			msg.AddMsgEvent(MsgID, 21, 4) 				--按钮
			msg.AddMsgEvent(MsgID, 1, 100001) 			--确定
			msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
			msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			if role.GetBagFreeSize(RoleID) < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 100003) 			--需要一格背包
				msg.AddMsgEvent(MsgID, 9, 1)
				msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
				msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
				msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if isGuanzhu == 0 then
					if CreatureTypeID == 5600080 then
						if role.GetRoleItemNum(RoleID,4800222) > 0 and role.GetRoleItemNum(RoleID,8019005) > 0 then
							role.RemoveFromRole(MapID, InstanceID, RoleID, 4800222, 1, 101)
							role.RemoveFromRole(MapID, InstanceID, RoleID, 8019005, 1, 101)
							role.AddRoleItem(MapID, InstanceID, RoleID, 8019003, 1, 6, 8, 420)
							isGuanzhu = 1
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 444316)			--灌注成功
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 444318) 			--缺少上古神器或水晶
							msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
							msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
							msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
					elseif CreatureTypeID == 5600081 then
						if role.GetRoleItemNum(RoleID,4800222) > 0 and role.GetRoleItemNum(RoleID,8029005) > 0 then
							role.RemoveFromRole(MapID, InstanceID, RoleID, 4800222, 1, 101)
							role.RemoveFromRole(MapID, InstanceID, RoleID, 8029005, 1, 101)
							role.AddRoleItem(MapID, InstanceID, RoleID, 8029003, 1, 6, 8, 420)
							isGuanzhu = 1
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 444316)			--灌注成功
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 444318) 			--缺少上古神器或水晶
							msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
							msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
							msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
					elseif CreatureTypeID == 5600082 then
						if role.GetRoleItemNum(RoleID,4800222) > 0 and role.GetRoleItemNum(RoleID,8049005) > 0 then
							role.RemoveFromRole(MapID, InstanceID, RoleID, 4800222, 1, 101)
							role.RemoveFromRole(MapID, InstanceID, RoleID, 8049005, 1, 101)
							role.AddRoleItem(MapID, InstanceID, RoleID, 8049003, 1, 6, 8, 420)
							isGuanzhu = 1
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 444316)			--灌注成功
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 444318) 			--缺少上古神器或水晶
							msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
							msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
							msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
					elseif CreatureTypeID == 5600083 then
						if role.GetRoleItemNum(RoleID,4800222) > 0 and role.GetRoleItemNum(RoleID,8039005) > 0 then
							role.RemoveFromRole(MapID, InstanceID, RoleID, 4800222, 1, 101)
							role.RemoveFromRole(MapID, InstanceID, RoleID, 8039005, 1, 101)
							role.AddRoleItem(MapID, InstanceID, RoleID, 8039003, 1, 6, 8, 420)
							isGuanzhu = 1
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 444316)			--灌注成功
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 444318) 			--缺少上古神器或水晶
							msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
							msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
							msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 444319)				--错误的NPC
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 444320) 			--已经完成神器灌注过程
					msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
					msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
					msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444321) 			--不是所属
			msg.AddMsgEvent(MsgID, 21, 4) 				--按钮
			msg.AddMsgEvent(MsgID, 1, 444322) 			--了解神器相关
			msg.AddMsgEvent(MsgID, 21, 6) 				--按钮
			msg.AddMsgEvent(MsgID, 1, 444323) 			--领取神灵赐福
			msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
			msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444324) 			--讲故事
			msg.AddMsgEvent(MsgID, 21, 5) 				--按钮
			msg.AddMsgEvent(MsgID, 1, 100002) 			--取消
			msg.AddMsgEvent(MsgID, 24, CreatureID) 		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 6 then
			local curday = tonumber(os.date("%d"))
			if role.GetRoleScriptData(RoleID, 1, RoleDataType["shenqicifu"]) == curday then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 444325)		--每天只能领一次奖励
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 444326)		--领取成功
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				unit.AddBuff(MapID, InstanceID, RoleID, 6937500+math.random(3), RoleID)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["shenqicifu"], curday)
			end
		end
	end
end
aux.RegisterCreatureEvent(5600080, 7, "c5600080On_Talk")
aux.RegisterCreatureEvent(5600081, 7, "c5600080On_Talk")
aux.RegisterCreatureEvent(5600082, 7, "c5600080On_Talk")
aux.RegisterCreatureEvent(5600083, 7, "c5600080On_Talk")

function c5600080On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	canStartshenqi = 1
	isGuanzhu = 0
end
aux.RegisterCreatureEvent(5600080, 13, "c5600080On_Disappear")
aux.RegisterCreatureEvent(5600081, 13, "c5600080On_Disappear")
aux.RegisterCreatureEvent(5600082, 13, "c5600080On_Disappear")
aux.RegisterCreatureEvent(5600083, 13, "c5600080On_Disappear")

--4800223道具
--613	23510	851 center

--3040384	御灵神兽
--3040385	器灵守卫
--3040386	虚冥

function c5600084On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	local step = cre.GetCreatureScriptData(MapID,  InstanceID, TargetID, 1, 0)
	local tarid = cre.GetCreatureScriptData(MapID,  InstanceID, TargetID, 1, 1)
	if step == 30 then
		local c1 = map.MapCreateColCreature(3424073512, -1, 3040385, 613, 23510, 871, 1, "")
		local c2 = map.MapCreateColCreature(3424073512, -1, 3040385, 627, 23510, 865, 1, "")
		local c3 = map.MapCreateColCreature(3424073512, -1, 3040385, 633, 23510, 851, 1, "")
		local c4 = map.MapCreateColCreature(3424073512, -1, 3040385, 627, 23510, 837, 1, "")
		local c5 = map.MapCreateColCreature(3424073512, -1, 3040385, 613, 23510, 831, 1, "")
		local c6 = map.MapCreateColCreature(3424073512, -1, 3040385, 599, 23510, 837, 1, "")
		local c7 = map.MapCreateColCreature(3424073512, -1, 3040385, 593, 23510, 851, 1, "")
		local c8 = map.MapCreateColCreature(3424073512, -1, 3040385, 599, 23510, 865, 1, "")
		cre.AddEnmity(3424073512, -1, c1, tarid, 1)
		cre.AddEnmity(3424073512, -1, c2, tarid, 1)
		cre.AddEnmity(3424073512, -1, c3, tarid, 1)
		cre.AddEnmity(3424073512, -1, c4, tarid, 1)
		cre.AddEnmity(3424073512, -1, c5, tarid, 1)
		cre.AddEnmity(3424073512, -1, c6, tarid, 1)
		cre.AddEnmity(3424073512, -1, c7, tarid, 1)
		cre.AddEnmity(3424073512, -1, c8, tarid, 1)
		g_SynCreatureEnmity(MapID, InstanceID, c1, tarid, 2, 50, 50)
		--[[
		cre.SetCreatureScriptData(3424073512,  -1, c1, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512,  -1, c2, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512,  -1, c3, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512,  -1, c4, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512,  -1, c5, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512,  -1, c6, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512,  -1, c7, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512,  -1, c8, 1, 0, tarid)
		--]]
		local stepid = map.MapCreateColCreature(3424073512, -1, 5600084, 1, 1, 1, 1, "")
		cre.SetCreatureScriptData(3424073512,  -1, stepid, 1, 0, 60)
		cre.SetCreatureScriptData(3424073512,  -1, stepid, 1, 1, tarid)
	elseif step == 60 then
		local c1 = map.MapCreateColCreature(3424073512, -1, 3040386, 613, 23510, 886, 1, "")
		local c2 = map.MapCreateColCreature(3424073512, -1, 3040386, 630, 23510, 880, 1, "")
		local c3 = map.MapCreateColCreature(3424073512, -1, 3040386, 642, 23510, 868, 1, "")
		local c4 = map.MapCreateColCreature(3424073512, -1, 3040386, 648, 23510, 851, 1, "")
		local c5 = map.MapCreateColCreature(3424073512, -1, 3040386, 642, 23510, 834, 1, "")
		local c6 = map.MapCreateColCreature(3424073512, -1, 3040386, 630, 23510, 822, 1, "")
		local c7 = map.MapCreateColCreature(3424073512, -1, 3040386, 613, 23510, 816, 1, "")
		local c8 = map.MapCreateColCreature(3424073512, -1, 3040386, 596, 23510, 822, 1, "")
		local c9 = map.MapCreateColCreature(3424073512, -1, 3040386, 584, 23510, 834, 1, "")
		local c10 = map.MapCreateColCreature(3424073512, -1, 3040386, 578, 23510, 851, 1, "")
		local c11 = map.MapCreateColCreature(3424073512, -1, 3040386, 584, 23510, 868, 1, "")
		local c12 = map.MapCreateColCreature(3424073512, -1, 3040386, 596, 23510, 880, 1, "")
		cre.AddEnmity(3424073512, -1, c1, tarid, 1)
		cre.AddEnmity(3424073512, -1, c2, tarid, 1)
		cre.AddEnmity(3424073512, -1, c3, tarid, 1)
		cre.AddEnmity(3424073512, -1, c4, tarid, 1)
		cre.AddEnmity(3424073512, -1, c5, tarid, 1)
		cre.AddEnmity(3424073512, -1, c6, tarid, 1)
		cre.AddEnmity(3424073512, -1, c7, tarid, 1)
		cre.AddEnmity(3424073512, -1, c8, tarid, 1)
		cre.AddEnmity(3424073512, -1, c9, tarid, 1)
		cre.AddEnmity(3424073512, -1, c10, tarid, 1)
		cre.AddEnmity(3424073512, -1, c11, tarid, 1)
		cre.AddEnmity(3424073512, -1, c12, tarid, 1)
		g_SynCreatureEnmity(MapID, InstanceID, c1, tarid, 2, 50, 50)
		--[[
		cre.SetCreatureScriptData(3424073512, -1, c1, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c2, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c3, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c4, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c5, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c6, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c7, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c8, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c9, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c10, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c11, 1, 0, tarid)
		cre.SetCreatureScriptData(3424073512, -1, c12, 1, 0, tarid)
		--]]
	end
end
aux.RegisterCreatureEvent(5600084, 13, "c5600084On_Disappear")

--[[
--怪物离开战斗自动追击开启神器任务玩家
function c3040384_On_Leave_Combat(MapID, InstanceID, CreatureID)
	local tarid = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)
	if tarid ~= 0 then
		cre.AddEnmity(MapID, InstanceID, CreatureID, tarid, 3)
	end
end
aux.RegisterCreatureEvent(3040384, 7, "c3040384_On_Leave_Combat")
aux.RegisterCreatureEvent(3040385, 7, "c3040384_On_Leave_Combat")
aux.RegisterCreatureEvent(3040386, 7, "c3040384_On_Leave_Combat")
--]]

