
quest_hechenglu = {}

--合成炉5511023   a=幻林灵石 分值为1,b=淼峰猪牙 分值为2,c=蝶精粉尘 分值为3
function hecheglu1_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local i = role.GetRoleItemNum(RoleID, 4010212)--判断有无合成帖
	local a = role.GetRoleItemNum(RoleID, 4010015)--平粱宝钻（4010015）
	local b = role.GetRoleItemNum(RoleID, 4010034)--冥山炎石（4010034）
	local c = role.GetRoleItemNum(RoleID, 4080041)--狂风符(4080041)
	if i >= 1 then
	    if a >= 10 and b >= 5 and c >= 1 then
			local function HCOnTalk(TargetID,RoleID)
				local MsgID = msg.BeginMsgEvent()
				if quest_hechenglu[RoleID] == 0 then
					msg.AddMsgEvent(MsgID, 20, 405007)	-- 请选择第一次使用的物品，根据不同的选择顺序的最终得到的物品也不一样。
				elseif quest_hechenglu[RoleID] < 10 then
					msg.AddMsgEvent(MsgID, 20, 405008)	-- 请继续选第二次使用的物品，根据不同的选择顺序的最终得到的物品也不一样。
				elseif quest_hechenglu[RoleID] < 100 then
					msg.AddMsgEvent(MsgID, 20, 405009)	-- 请继续选第三次使用的物品，根据不同的选择顺序的最终得到的物品也不一样。
				else
					msg.AddMsgEvent(MsgID, 20, 405030)	-- 您想使用之前已经选择的顺序来合成物品吗？
					msg.AddMsgEvent(MsgID, 21, 8)	    -- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
				end
				if math.floor(quest_hechenglu[RoleID]%100/10) ~= 1 and math.floor(quest_hechenglu[RoleID]%10) ~= 1 and math.floor(quest_hechenglu[RoleID]/100) ~= 1 then
					msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 405018)	-- 平粱宝钻
				end
				if math.floor(quest_hechenglu[RoleID]%100/10) ~= 2 and math.floor(quest_hechenglu[RoleID]%10) ~= 2 and math.floor(quest_hechenglu[RoleID]/100) ~= 2 then
					msg.AddMsgEvent(MsgID, 21, 5)	-- 选项二
					msg.AddMsgEvent(MsgID, 1, 405019)	-- 冥山炎石
				end
				if math.floor(quest_hechenglu[RoleID]%100/10) ~= 3 and math.floor(quest_hechenglu[RoleID]%10) ~= 3 and math.floor(quest_hechenglu[RoleID]/100) ~= 3 then
					msg.AddMsgEvent(MsgID, 21, 6)	-- 选项三
					msg.AddMsgEvent(MsgID, 1, 405020)	-- 狂风符
				end
				if quest_hechenglu[RoleID] ~= 0 then
					msg.AddMsgEvent(MsgID, 21, 7)	-- 选项四
					msg.AddMsgEvent(MsgID, 1, 405031)	-- 重新放入物品
				end
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end

			if quest_hechenglu[RoleID] == nil then
				quest_hechenglu[RoleID] = 0
			end
			if TalkIndex == -1 then
				HCOnTalk(TargetID,RoleID)
			elseif TalkIndex == 4 or TalkIndex == 5 or TalkIndex == 6 then
				local point = TalkIndex - 3
				if quest_hechenglu[RoleID] == 0 then
					quest_hechenglu[RoleID] = quest_hechenglu[RoleID] + point
					HCOnTalk(TargetID,RoleID)
				elseif quest_hechenglu[RoleID] < 10 then
					quest_hechenglu[RoleID] = quest_hechenglu[RoleID] + point * 10
					HCOnTalk(TargetID,RoleID)
				elseif quest_hechenglu[RoleID] < 100 then
					quest_hechenglu[RoleID] = quest_hechenglu[RoleID] + point * 100
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405030)	-- 确定想要以这样的次序合成物品吗？
					msg.AddMsgEvent(MsgID, 21, 8)	    -- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
					msg.AddMsgEvent(MsgID, 21, 7)	    -- 选项一
					msg.AddMsgEvent(MsgID, 1, 405031)	-- 重新选择次序
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			elseif TalkIndex == 7 then
				quest_hechenglu[RoleID] = nil
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 405031)	-- 请重新选择想要放入的物品次序
				msg.AddMsgEvent(MsgID, 21, -1)	    -- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif TalkIndex == 8 then
				local FreeBag = role.GetBagFreeSize(RoleID)
				if FreeBag < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 100003)	-- 请保证背包内有一个空位再来合成
					msg.AddMsgEvent(MsgID, 9, 1)
					msg.AddMsgEvent(MsgID, 21, 8)	    -- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					if a >= 10 and b >= 5 and c >= 1 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4010015, 10, 402)--10个平粱宝钻（4010015）
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4010034, 5, 402)--5个冥山炎石（4010034）
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4080041, 1, 402)--1个狂风符
						local Tbl = {}
						Tbl[123] = 4010224
						Tbl[132] = 4010222
						Tbl[213] = 4010223
						Tbl[231] = 4010220
						Tbl[312] = 4010221
						Tbl[321] = 4010219
						if quest_hechenglu[RoleID] ~= nil then
							role.AddRoleItem(MapID, InstanceID, RoleID, Tbl[quest_hechenglu[RoleID]], 1, 0, 0, 402)
						end
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 405023)	-- 您的材料不足，需要XX个XX,XX个XX和XX个XX。请准备好再来
						msg.AddMsgEvent(MsgID, 21, -1)	    -- 选项一
						msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
					quest_hechenglu[RoleID] = nil
				end
			end
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 405023)	-- 您的材料不足，需要XX个XX,XX个XX和XX个XX。请准备好再来
			msg.AddMsgEvent(MsgID, 21, -1)	    -- 选项一
			msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 20, 405021)--玩家没有合成帖，不能使用魔幻炉
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
	end

end
aux.RegisterCreatureEvent(5511024, 7, "hecheglu1_OnTalk")

function hechenglu1_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local i = role.GetRoleItemNum(RoleID, 4010212)
	if i > 0 then
		role.RemoveFromRole(MapID, InstanceID, RoleID,4010212, 1, 402)
	end

end
aux.RegisterQuestEvent(60207,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60208,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60209,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60210,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60211,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60212,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60213,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60214,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60215,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60216,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60217,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60218,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60219,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60220,11,"hechenglu1_OnComplete")
aux.RegisterQuestEvent(60221,11,"hechenglu1_OnComplete")
