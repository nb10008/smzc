--圣诞卷能否使用
function shengdanjuan_CanUse(MapID, InstanceID, TypeID, RoleID)
	local Rquest_Id = {}
	Rquest_Id[4800007] = {}
	Rquest_Id[4800007][1] = {questid = 30658}
	Rquest_Id[4800007][2] = {questid = 30659}
	Rquest_Id[4800007][3] = {questid = 30660}
	Rquest_Id[4800007][4] = {questid = 30661}
	Rquest_Id[4800007][5] = {questid = 30662}
	Rquest_Id[4800007][6] = {questid = 30663}
	Rquest_Id[4800007][7] = {questid = 30664}
	Rquest_Id[4800007][8] = {questid = 30690}
	Rquest_Id[4800007][9] = {questid = 30691}
	Rquest_Id[4800007][10] = {questid = 30692}
	Rquest_Id[4800007][11] = {questid = 30693}
	Rquest_Id[4800007][12] = {questid = 30694}
	Rquest_Id[4800007][13] = {questid = 30695}
	Rquest_Id[4800007][14] = {questid = 30696}
	Rquest_Id[4800006] = {}
	Rquest_Id[4800006][1] = {questid = 30665}
	Rquest_Id[4800006][2] = {questid = 30666}
	Rquest_Id[4800006][3] = {questid = 30667}
	Rquest_Id[4800006][4] = {questid = 30668}
	Rquest_Id[4800006][5] = {questid = 30669}
	Rquest_Id[4800006][6] = {questid = 30670}
	Rquest_Id[4800006][7] = {questid = 30671}
	Rquest_Id[4800006][8] = {questid = 30697}
	Rquest_Id[4800006][9] = {questid = 30698}
	Rquest_Id[4800006][10] = {questid = 30699}
	Rquest_Id[4800006][11] = {questid = 30700}
	Rquest_Id[4800006][12] = {questid = 30701}
	Rquest_Id[4800006][13] = {questid = 30702}
	Rquest_Id[4800006][14] = {questid = 30703}
	Rquest_Id[4800005] = {}
	Rquest_Id[4800005][1] = {questid = 30672}
	Rquest_Id[4800005][2] = {questid = 30673}
	Rquest_Id[4800005][3] = {questid = 30674}
	Rquest_Id[4800005][4] = {questid = 30675}
	Rquest_Id[4800005][5] = {questid = 30676}
	Rquest_Id[4800005][6] = {questid = 30677}
	Rquest_Id[4800005][7] = {questid = 30678}
	Rquest_Id[4800005][8] = {questid = 30704}
	Rquest_Id[4800005][9] = {questid = 30705}
	Rquest_Id[4800005][10] = {questid = 30706}
	Rquest_Id[4800005][11] = {questid = 30707}
	Rquest_Id[4800005][12] = {questid = 30708}
	Rquest_Id[4800005][13] = {questid = 30709}
	Rquest_Id[4800005][14] = {questid = 30710}

	local a = role.GetRoleQuestNum(RoleID)
	local levle = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local QuestID1 = msg.BeginMsgEvent()
	local bRet, bIgnore = 0, false
	if a >= 30 then
		bRet = 21

	else
		if levle < 30 then
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 100008)--等级不足，无法使用物品
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			bRet = 32
		elseif levle >= 30 then
			for i = 1,14 do
				local a = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, Rquest_Id[TypeID][i].questid)
				if a == true then
					local QuestID1 = msg.BeginMsgEvent()
					msg.AddMsgEvent(QuestID1, 13, 0)
					msg.AddMsgEvent(QuestID1, 1, 100007)--您已经接取了任务
					msg.DispatchRoleMsgEvent(RoleID, QuestID1)
					bRet = 32
				end
			end
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4800005, 0, "shengdanjuan_CanUse")
aux.RegisterItemEvent(4800006, 0, "shengdanjuan_CanUse")
aux.RegisterItemEvent(4800007, 0, "shengdanjuan_CanUse")
-- 使用
function shengdanjuan_onUse(MapID, InstanceID, TypeID, RoleID)
	local Rquest_Id = {}
	Rquest_Id[4800007] = {}
	Rquest_Id[4800007][1] = {questid = 30658}
	Rquest_Id[4800007][2] = {questid = 30659}
	Rquest_Id[4800007][3] = {questid = 30660}
	Rquest_Id[4800007][4] = {questid = 30661}
	Rquest_Id[4800007][5] = {questid = 30662}
	Rquest_Id[4800007][6] = {questid = 30663}
	Rquest_Id[4800007][7] = {questid = 30664}
	Rquest_Id[4800007][8] = {questid = 30690}
	Rquest_Id[4800007][9] = {questid = 30691}
	Rquest_Id[4800007][10] = {questid = 30692}
	Rquest_Id[4800007][11] = {questid = 30693}
	Rquest_Id[4800007][12] = {questid = 30694}
	Rquest_Id[4800007][13] = {questid = 30695}
	Rquest_Id[4800007][14] = {questid = 30696}
	Rquest_Id[4800006] = {}
	Rquest_Id[4800006][1] = {questid = 30665}
	Rquest_Id[4800006][2] = {questid = 30666}
	Rquest_Id[4800006][3] = {questid = 30667}
	Rquest_Id[4800006][4] = {questid = 30668}
	Rquest_Id[4800006][5] = {questid = 30669}
	Rquest_Id[4800006][6] = {questid = 30670}
	Rquest_Id[4800006][7] = {questid = 30671}
	Rquest_Id[4800006][8] = {questid = 30697}
	Rquest_Id[4800006][9] = {questid = 30698}
	Rquest_Id[4800006][10] = {questid = 30699}
	Rquest_Id[4800006][11] = {questid = 30700}
	Rquest_Id[4800006][12] = {questid = 30701}
	Rquest_Id[4800006][13] = {questid = 30702}
	Rquest_Id[4800006][14] = {questid = 30703}
	Rquest_Id[4800005] = {}
	Rquest_Id[4800005][1] = {questid = 30672}
	Rquest_Id[4800005][2] = {questid = 30673}
	Rquest_Id[4800005][3] = {questid = 30674}
	Rquest_Id[4800005][4] = {questid = 30675}
	Rquest_Id[4800005][5] = {questid = 30676}
	Rquest_Id[4800005][6] = {questid = 30677}
	Rquest_Id[4800005][7] = {questid = 30678}
	Rquest_Id[4800005][8] = {questid = 30704}
	Rquest_Id[4800005][9] = {questid = 30705}
	Rquest_Id[4800005][10] = {questid = 30706}
	Rquest_Id[4800005][11] = {questid = 30707}
	Rquest_Id[4800005][12] = {questid = 30708}
	Rquest_Id[4800005][13] = {questid = 30709}
	Rquest_Id[4800005][14] = {questid = 30710}
	local levle = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if levle >= 30 and levle <= 34 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][1].questid)
	elseif levle >= 35 and levle <= 39 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][8].questid)
	elseif levle >= 40 and levle <= 44 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][2].questid)
	elseif levle >= 40 and levle <= 49 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][9].questid)
	elseif levle >= 50 and levle <= 54 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][3].questid)
	elseif levle >= 55 and levle <= 59 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][10].questid)
	elseif levle >= 60 and levle <= 64 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][4].questid)
	elseif levle >= 65 and levle <= 69 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][11].questid)
	elseif levle >= 70 and levle <= 74 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][5].questid)
	elseif levle >= 75 and levle <= 79 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][12].questid)
	elseif levle >= 80 and levle <= 84 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][6].questid)
	elseif levle >= 85 and levle <= 89 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][13].questid)
	elseif levle >= 90 and levle <= 94 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][7].questid)
	elseif levle >= 95 then
		role.AddQuest(RoleID,Rquest_Id[TypeID][14].questid)
	end
	local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206332)--您以接取雪国大作战任务，请到任务界面中确认任务内容。
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
end
aux.RegisterItemEvent(4800005, 1, "shengdanjuan_onUse")
aux.RegisterItemEvent(4800006, 1, "shengdanjuan_onUse")
aux.RegisterItemEvent(4800007, 1, "shengdanjuan_onUse")
