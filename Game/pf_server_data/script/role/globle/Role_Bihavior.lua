--玩家脚本
-- 玩家拜师
function x_RoleHandleMaster(MasterID, ApprenticeID)
	local MsgID = msg.BeginMsgEvent()		-- 全服跑马灯...
	msg.AddMsgEvent(MsgID, 100, 2330)
	msg.AddMsgEvent(MsgID, 2, MasterID)
	msg.AddMsgEvent(MsgID, 2, ApprenticeID)
	msg.DispatchWorldMsgEvent(MsgID)
end

function x_RoleAddFriend(MapID, InstanceID, RoleID, FriendID)
    local k = role.GetFriendNum(RoleID)
	if k > 19 and k % 10 == 0 then
	    role.NotifyFBBS(RoleID,8,k)
	end
	local Trail = {1004,2006,3007,4006,5007,6009}
	for i = 1,6 do
		if role.GetTrialCompleteNum(RoleID,Trail[i]) < k then
			role.SetTrialCompleteNum(RoleID, Trail[i], k)
		end
		if role.GetTrialState(RoleID, Trail[i]) == 1 and role.GetTrialCompleteNum(RoleID,Trail[i]) >= Trail_maxnum[Trail[i]] then
			role.TrialComplete(RoleID,Trail[i])
		end
	end
end

function x_RoleLearnNewSkill(MapID, InstanceID, RoleID, SkillID)
    if SkillID >= 9100101 and SkillID <= 9100305 then
		local Trail = {2013,4014,6014,8024}
		for i = 1,4 do
			if role.GetTrialCompleteNum(RoleID,Trail[i]) < SkillID % 100 then
				role.SetTrialCompleteNum(RoleID, Trail[i], SkillID % 100)
			end
			if role.GetTrialState(RoleID, Trail[i]) == 1 and role.GetTrialCompleteNum(RoleID,Trail[i]) >= Trail_maxnum[Trail[i]] then
				role.TrialComplete(RoleID,Trail[i])
			end
		end
	elseif SkillID == 9000301 then
	    if role.GetTrialCompleteNum(RoleID,3003) ~= 1 then
			role.SetTrialCompleteNum(RoleID, 3003, 1)
		end
		if role.GetTrialState(RoleID, 3003) == 1 and role.GetTrialCompleteNum(RoleID,3003) >= Trail_maxnum[3003] then
			role.TrialComplete(RoleID,3003)
		end
	end

end

function x_RoleCreatAGuild(MapID, InstanceID, RoleID, GuildID)
    role.NotifyFBBS(RoleID,5,GuildID)
end

function x_RoleGetATitle(RoleID, TitleID)
    role.NotifyFBBS(RoleID,10,TitleID)
end

function x_CanAddEnemy(MapID, InstanceID, RoleID, KillID)
    if MapID == 2092183860 or MapID == 2092184116 or MapID == 2092184372 then
	    return 0
	else
	    return 1
	end
end

function x_KillMonster(MapID, InstanceID, RoleID, MonID)
    local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curyear = tonumber(os.date("%Y"))
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if curyear == 2011 and curmon == 1 and curday >= 11 and LaBaZhou_Open == 1 and curday <= 18 and level >= 40 then
	    local LTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["LaBaZhou_Time"])
		local CTime = os.time() --当前时间
		local s = 0
		if LTime == 0 then
		    s = 3000
		else
		    s = os.difftime(CTime,LTime)
		end
	    if role.GetBagFreeSize(RoleID) > 0 and math.random(200) == 50 and s >= 3000 then
		    role.SetRoleScriptData(RoleID, 1, RoleDataType["LaBaZhou_Time"],CTime)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4800050, 1, -1, 8, 420)
		end
	end
end
--注册函数

--获得稀有领域
function x_GetRareField(MapID, InstanceId, RoleID, FieldID)
    local index = 411000 + math.floor(FieldID/100)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,100,index)
	msg.AddMsgEvent(MsgID,2,RoleID)
	msg.DispatchWorldMsgEvent(MsgID)	--全服广播
end


--抢夺稀有领域
function x_CaptureRareField(MapID, InstanceId, TargetID, RoleID, FieldID)
	local index = 412000 + math.floor(FieldID/100)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,100,index)
	msg.AddMsgEvent(MsgID,2,RoleID)
	msg.AddMsgEvent(MsgID,2,TargetID)
	msg.DispatchWorldMsgEvent(MsgID)	--全服广播
end

--玩家神格提升
function x_GodLevelUp(MapID, InstanceID, RoleID, Level)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if Level >9 then
	   Level = 9
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"], math.floor(k/10)*10+Level)
	if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40059) then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40059, 2)
	end
	if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40064)  and Level >= 3 then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40064, 2)
	end
end

--[[玩家使用变身技能
function x_GodChangeModle(MapID, InstanceId, RoleID, SkillID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	role.SetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"], math.floor(k/100)*100+k%10+10)
    if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40060) then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40060, 2)
	end
end]]

--玩家激活一个自选领域
function x_FieldWakeUp(MapID, InstanceId, RoleID, FieldID)
    if FieldID == 2003401 then
		local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
		role.SetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"], k%100+100)
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40061) then
			role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40061, 2)
		end
    end
end

--玩家是否可以使用驿站
function x_CanUseDakNPC(MapID, InstanceId, RoleID, ToMap, x, y, z)
    if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61016) then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444017)				--接取速度极限任务之后将无法使用此功能
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return false
	else
	    if ToMap == 3424071720 and role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 20300) then
		    role.SetRoleScriptData(RoleID, 1, RoleDataType["BeiZhouTianXue"], 1)
			role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 20300, 2)
		end
	    return true
	end
end

--玩家是否可以使用土地庙
function x_CanUseEarthTemple(MapID, InstanceId, RoleID, ToMap, x, y, z)
    if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61016) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444017)				--接取速度极限任务之后将无法使用此功能
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return false
	else
	    return true
	end
end

--3级衣橱套装数量变化
function x_OnFashionSuitNumChg(MapID, InstanceID, RoleID)
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"]) >= 10 then
		local _, _, _, playerSuitNum = role.GetWardrobeDetailInfo(MapID, InstanceID, RoleID)
		local changetable = {}
		changetable[3201401] = 40
		changetable[3201301] = 36
		changetable[3201201] = 31
		changetable[3201101] = 27
		changetable[3201001] = 23
		changetable[3200901] = 19
		changetable[3200801] = 16
		changetable[3200701] = 13
		changetable[3200601] = 10
		changetable[3200501] = 8
		changetable[3200401] = 6
		changetable[3200301] = 4
		changetable[3200201] = 3
		changetable[3200101] = 2
		if playerSuitNum < 2 and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 32001) then
			unit.CancelBuff(MapID, InstanceID, RoleID, 3200101)
		else
			for i=3201401,3200101,-100 do
				if playerSuitNum and playerSuitNum >= changetable[i] then
					unit.AddBuff(MapID, InstanceID, RoleID, i, RoleID)
					break
				end
			end
		end
	end
end

--衣橱升级
function x_OnWardLevelUp(MapID, InstanceID, RoleID)
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiShangPingLun"]) >= 10 then
		local _, _, _, playerSuitNum = role.GetWardrobeDetailInfo(MapID, InstanceID, RoleID)
		local changetable = {}
		changetable[3201401] = 40
		changetable[3201301] = 36
		changetable[3201201] = 31
		changetable[3201101] = 27
		changetable[3201001] = 23
		changetable[3200901] = 19
		changetable[3200801] = 16
		changetable[3200701] = 13
		changetable[3200601] = 10
		changetable[3200501] = 8
		changetable[3200401] = 6
		changetable[3200301] = 4
		changetable[3200201] = 3
		changetable[3200101] = 2
		if playerSuitNum < 2 and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 32001) then
			unit.CancelBuff(MapID, InstanceID, RoleID, 3200101)
		else
			for i=3201401,3200101,-100 do
				if playerSuitNum and playerSuitNum >= changetable[i] then
					unit.AddBuff(MapID, InstanceID, RoleID, i, RoleID)
					break
				end
			end
		end
	end
end

--玩家完成一个悬赏榜任务
function x_OnCompleteBoardQuest(MapID, InstanceID, RoleID, QuestID, BoardID, Money, Exp)
	local jinqiantequan = 0
	for i = 1,6 do
		if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90048+i) then
			jinqiantequan = 1
		elseif role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90048+i+6) then
			jinqiantequan = 2
		elseif role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90048+i+12) then
			jinqiantequan = 3
		end
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61045) then
		local Exp1 = 3*Exp
		role.AddRoleExp(MapID, InstanceID, RoleID, Exp1)
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61044) then
		local y,z = role.GetBuffLayer(RoleID, 6104401)
		local Money1 = math.floor(y*0.04*Money)
		local Exp1 = math.floor(y*0.04*Exp)
		role.AddRoleExp(MapID, InstanceID, RoleID, Exp1)
		role.AddRoleSilver(MapID, InstanceID, RoleID, Money1)
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61050) then
		local Exp2 = 5*Exp
		role.AddRoleExp(MapID, InstanceID, RoleID, Exp2)
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61051) then   --不能叠加
	    if not role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61050) then
			local Exp2 = 2*Exp
			local Money2 = 2*Money
			role.AddRoleExp(MapID, InstanceID, RoleID, Exp2)
			role.AddRoleSilver(MapID, InstanceID, RoleID, Money2)
		end
	elseif jinqiantequan == 1 then
		local Money2 = 0.2*Money
		role.AddRoleSilver(MapID, InstanceID, RoleID, Money2)
	elseif jinqiantequan == 2 then
		local Money2 = 1*Money
		role.AddRoleSilver(MapID, InstanceID, RoleID, Money2)
	elseif jinqiantequan == 3 then
		local Money2 = 2*Money
		role.AddRoleSilver(MapID, InstanceID, RoleID, Money2)
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20552) then
		local Exp1 = 9*Exp
		role.AddRoleExp(MapID, InstanceID, RoleID, Exp1)
	end

end

--玩家穿着特殊装备切换地图
function x_OnCheckEquipAttWhenChangeMap(MapID, InstanceID, RoleID, TypeID, High_64, Low_64)
	if MapID == S59_MapID then
	    return 1
	else
	    return 0
	end
end
--玩家穿着特殊装备时做判断用
function x_OnCheckEquipCanBeUsed(MapID, InstanceID, RoleID, TypeID, High_64, Low_64)
	if MapID == S59_MapID then
		if InstanceID ~= 100 and InstanceID ~= 101 then
			return 1
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 211038) -- 跨服争霸赛中无法使用该装备
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			return 0
		end
	else
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 305001) -- 装备只能在斗神试炼场中才能装备
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	    return 0
	end
end
aux.RegisterRoleEvent(27, "x_RoleHandleMaster")
aux.RegisterRoleEvent(46, "x_RoleAddFriend")
aux.RegisterRoleEvent(47, "x_RoleLearnNewSkill")
aux.RegisterRoleEvent(57, "x_RoleCreatAGuild")
aux.RegisterRoleEvent(31, "x_RoleGetATitle")
aux.RegisterRoleEvent(58, "x_CanAddEnemy")
aux.RegisterRoleEvent(59, "x_KillMonster")
aux.RegisterRoleEvent(60, "x_GodLevelUp")
--aux.RegisterRoleEvent(62, "x_GodChangeModle")
aux.RegisterRoleEvent(63, "x_GetRareField")
aux.RegisterRoleEvent(64, "x_CaptureRareField")
aux.RegisterRoleEvent(65, "x_FieldWakeUp")
aux.RegisterRoleEvent(67, "x_CanUseDakNPC")
aux.RegisterRoleEvent(68, "x_CanUseEarthTemple")
aux.RegisterRoleEvent(72, "x_OnFashionSuitNumChg")
aux.RegisterRoleEvent(73, "x_OnWardLevelUp")
aux.RegisterRoleEvent(79, "x_OnCompleteBoardQuest")
aux.RegisterRoleEvent(87, "x_OnCheckEquipAttWhenChangeMap")
aux.RegisterRoleEvent(88, "x_OnCheckEquipCanBeUsed")
