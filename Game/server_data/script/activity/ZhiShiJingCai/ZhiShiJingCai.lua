---本文件中baixiao即为F计划的zhishijingcai,为维护方便，脚本中未替换

BaiXiaoSheng_CreatureID = {}			-- 存放创建知识竞猜后返回的id，用于活动结束后删除npc
BaiXiaoSheng_Attend = {}				-- 是否在此活动时间内参加过
BaiXiaoSheng_Questions = {}				-- 问题数
BaiXiaoSheng_RightAnswer = {}			-- 正确答案在四个选项中的索引
BaiXiaoSheng_Normal_Question_Point = 10	-- 百晓普通题目的分数
BaiXiaoSheng_Hard_Question_Point = 20 	-- 百晓困难题目的分数
BaiXiaoSheng_Time = {}					-- 存放答题开始时的系统时间，对应角色id
BaiXiaoSheng_Role_Point = {}			-- 玩家获得的分数，对应角色id
BaiXiaoSheng_Stepindex = {}				-- 每个玩家的活动步骤
BaiXiaoSheng_Question_Type = {}			-- 当前题目类型，百晓困难或百晓普通
BaiXiaoSheng_Role_Level = {}			-- 玩家当前等级
BaiXiaoSheng_Time_Counter = 45			-- 答题的倒计时
BaiXiaoSheng_Current_Question = {}      -- 玩家当前答题

BaiXiaoSheng_Hard_Question_Num = 20	-- 百晓困难的题目数
BaiXiaoSheng_Normal_Question_Num = 98	-- 百晓普通的题目数

BX_CallDeep = 0							-- 递归调用深度


BaiXiao_Hard_Question_Table = {}
BaiXiao_Normal_Question_Table = {}

g_BX_RoleNum = 0						-- 参加知识竞猜答题活动的玩家个数
tbl_BX_RoleID = {}						-- 以g_BX_RoleNum为索引对应玩家的RoleID
tbl_BX_Index = {}						-- 以RoleID为索引对应玩家的答题Index
tbl_BX_Questions = {}					-- 玩家已经回答过的题目

-- npc对话，整个答题过程
function n5510301_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		-- 初始化空表
		BaiXiaoSheng_InitNullTable(RoleID)
	end

	-- 用到的属性
	BaiXiaoSheng_Role_Level[tbl_BX_Index[RoleID]] = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 200001)	-- "您是否决定参加知识竞猜活动？"
		msg.AddMsgEvent(MsgID, 21, 4)		-- 确定按钮
		msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
		msg.AddMsgEvent(MsgID, 21, 5)		-- 取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)	-- "取消"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 2
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 2 then
		if TalkIndex == 4 then			-- 选择了确定参加按钮后
			if BaiXiaoSheng_Role_Level[tbl_BX_Index[RoleID]] <= 30 then	-- 等级低于30级的话弹出提示
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 200002)		-- "知识竞猜活动必须在30级以上才可参与"
				msg.AddMsgEvent(MsgID, 21, 4)			-- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001)		-- "确定"
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif BaiXiaoSheng_Attend[tbl_BX_Index[RoleID]] == 1 then	-- 在此活动时间内已参加过
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 200003)		-- "您已参加过本次知识竞猜活动，可继续参加下个时间的活动。"
				msg.AddMsgEvent(MsgID, 21, 4)			-- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001)		-- "确定"
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else
				-- “答题活动现在开始”
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 200004)		--答题活动现在开始!
				msg.AddMsgEvent(MsgID, 21, 4)
				msg.AddMsgEvent(MsgID, 1, 100001)
				msg.AddMsgEvent(MsgID, 24, TargetID)
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 3
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				BaiXiaoSheng_Attend[tbl_BX_Index[RoleID]] = 1				-- 已参加过本次活动
				BaiXiaoSheng_Current_Question[tbl_BX_Index[RoleID]] = {question, index}
			end
		elseif TalkIndex == 5 then
			BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
		else
			BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
		end


	elseif BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 3 then				-- 答题开始
		BaiXiaoSheng_GenerateQuestion(RoleID, TargetID)
		BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 4


	elseif BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 4 then	-- 计算答题所用时间
		if TalkIndex == 5 then						-- 放弃活动则奖励玩家当前得分应得的奖励
			BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
		else
			-- 计算答题花费的时间
			local time1 = BaiXiaoSheng_Time[tbl_BX_Index[RoleID]]
			local dtime = os.difftime(os.time(), time1)

			-- 判断答案是否正确
			if BaiXiaoSheng_RightAnswer[tbl_BX_Index[RoleID]] == TalkIndex then
				-- 回答所花的时间不同，得分不同
				if dtime <= 45 then				-- 45秒内回答出正确答案，获得题目4分数
					if BaiXiaoSheng_Question_Type[tbl_BX_Index[RoleID]] == 1 then  	-- 百晓困难得分
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Hard_Question_Point
					else							-- 百晓普通得分
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Normal_Question_Point
					end
				else
					if BaiXiaoSheng_Question_Type[tbl_BX_Index[RoleID]] == 1 then  	-- 百晓困难得分
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Hard_Question_Point/2
					else							-- 百晓普通得分
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Normal_Question_Point/2
					end
				end

			end

			-- 产生题目
			if BaiXiaoSheng_Questions[tbl_BX_Index[RoleID]] <= 30 then    -- 答题数小于30题时继续产生题目
				BaiXiaoSheng_GenerateQuestion(RoleID, TargetID)
			else
				-- 答题结束
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 40, -1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
			end

			-- 玩家离线
			if (role.IsRoleOnLine(MapID, InstanceID, RoleID) == 0) then
				-- 如果已经参加过活动，则获得当前分数对应的奖励，并且活动自动终止
				if (BaiXiaoSheng_Attend[tbl_BX_Index[RoleID]] == 1) then
					BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
					BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
				end
			end
		end
	end
end


-- 初始化空表的默认值
function BaiXiaoSheng_InitNullTable(RoleID)
	-- 设置一些空表的默认值
	if tbl_BX_Index[RoleID] == nil then
		g_BX_RoleNum = g_BX_RoleNum + 1
		tbl_BX_RoleID[g_BX_RoleNum] = RoleID
		tbl_BX_Index[RoleID] = g_BX_RoleNum
		tbl_BX_Questions[g_BX_RoleNum] = {}
	end

	if BaiXiaoSheng_Stepindex[g_BX_RoleNum] == nil then
		BaiXiaoSheng_Stepindex[g_BX_RoleNum] = 1
	end
	if BaiXiaoSheng_Attend[g_BX_RoleNum] == nil then
		BaiXiaoSheng_Attend[g_BX_RoleNum] = 0
	end
	if BaiXiaoSheng_Questions[g_BX_RoleNum] == nil then
		BaiXiaoSheng_Questions[g_BX_RoleNum] = 1
	end
    if BaiXiaoSheng_Role_Point[g_BX_RoleNum] == nil then
        BaiXiaoSheng_Role_Point[g_BX_RoleNum] = 0
    end
	if BaiXiaoSheng_Question_Type[g_BX_RoleNum] == nil then
	    BaiXiaoSheng_Question_Type[g_BX_RoleNum] = 0
	end
end

-- 初始化题库
function BaiXiaoSheng_InitQuestionTable()
	-- 将id_msg中百晓困难答题内容放入表中
	for i=1, BaiXiaoSheng_Hard_Question_Num do
		local id = 601001 + (i-1)*5
		BaiXiao_Hard_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end

	-- 将id_msg中百晓普通答题内容放入表中
	for i=1, BaiXiaoSheng_Normal_Question_Num do
		local id = 600001 + (i-1)*5
		BaiXiao_Normal_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end

end

-- 活动结束或者玩家掉线，给与玩家奖励
function BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)

	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level > 100 then
		level = 100
	end
	local FreeSize = role.GetBagFreeSize(RoleID)
	if FreeSize >= 1 then
	    if level >= 75 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4090006, 5, -1, 8, 420)
		elseif level >= 55 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4090005, 4, -1, 8, 420)
		elseif level >= 40 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4090004, 3, -1, 8, 420)
		end
	end

	level = math.floor(level/5)*5
	--[[金钱
	local RoleIndex = tbl_BX_Index[RoleID]
	local addmoney = 5000 * BaiXiaoSheng_Role_Point[RoleIndex] / 360
	role.AddRoleSilver(MapID, InstanceID, RoleID, addmoney, 0)
]]
	-- 经验
	local BaiXiaoSheng_reward = {
	[20]=7245,[25]=8820,[30]=23940,[35]=30240,
	[40]=45360,[45]=54180,[50]=79500,[55]=93000,
	[60]=120960,[65]=139860,[70]=192780,[75]=253260,
	[80]=327600,[85]=406350,[90]=495810,[95]=595980,[100]=595980
	}

	local addexp = BaiXiaoSheng_reward[level]
	role.AddRoleExp(MapID, InstanceID, RoleID, addexp * BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]]/360)

	BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1

	if role.GetTrialState(RoleID, 3014) == 1 then
		role.SetTrialCompleteNum(RoleID, 3014, role.GetTrialCompleteNum(RoleID,3014) + 1)
    end
	if role.GetTrialState(RoleID, 3014) == 1 and role.GetTrialCompleteNum(RoleID,3014) >= Trail_maxnum[3014] then
		role.TrialComplete(RoleID,3014)
	end
	role.NotifyFBBS(RoleID,11,0)
end

-- 返回不重复的题目  参数0为困难题目 1为普通题目
function BX_GetQuestion(RoleIndex, n)
	BX_CallDeep = BX_CallDeep + 1		-- 递归深度+1
	local question
	local index



	if n == 0 then
		if table.maxn(BaiXiao_Hard_Question_Table) == 0 then	--判断之前的活动进行过程中，是否因重载脚本或服务器重启等原因造成题库表被置空
			BaiXiaoSheng_InitQuestionTable()
		end
		index = math.random(1, table.maxn(BaiXiao_Hard_Question_Table))
		question = BaiXiao_Hard_Question_Table[index]
	else
		if table.maxn(BaiXiao_Normal_Question_Table) == 0 then	--判断之前的活动进行过程中，是否因重载脚本或服务器重启等原因造成题库表被置空
			BaiXiaoSheng_InitQuestionTable()
		end
		index = math.random(1, table.maxn(BaiXiao_Normal_Question_Table))
		question = BaiXiao_Normal_Question_Table[index]
	end

	if tbl_BX_Questions[RoleIndex] == nil then
		local question_table = {}
		tbl_BX_Questions[RoleIndex] = {question_table}
		table.insert(tbl_BX_Questions[RoleIndex], question.Content)
		return question
	end

	local flag = 0
	for k, v in pairs(tbl_BX_Questions[RoleIndex]) do
		if v == question.Content then
			flag = 1
			break
		end
	end

	if flag == 1 then
		if BX_CallDeep >= 30 then			-- 递归深度超过30的话就直接返回重复的题
			return question
		else
			return BX_GetQuestion(RoleIndex, n)
		end
	else
		table.insert(tbl_BX_Questions[RoleIndex], question.Content)
		return question
	end
end

-- 产生题目和选项
function BaiXiaoSheng_GenerateQuestion(RoleID, TargetID)
	local RoleIndex = tbl_BX_Index[RoleID]
	if math.fmod(BaiXiaoSheng_Questions[RoleIndex], 5	) == 0 then  -- 答题数是5的倍数时抽取百晓困难的题目
		BX_CallDeep = 0
		BaiXiaoSheng_Current_Question[RoleIndex].question = BX_GetQuestion(RoleIndex, 0)

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 40, BaiXiaoSheng_Current_Question[RoleIndex].question.Content)	-- 题目内容
		msg.AddMsgEvent(MsgID, 1, 200005)
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Role_Point[RoleIndex])		-- 得分


		-- 正确答案在四个选项中的随机位置
		BaiXiaoSheng_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 0
		for i=0, 3 do									-- 四个选项
			if BaiXiaoSheng_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)			-- 选项
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.RightAnswer)	-- 选项内容
			else
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.WrongAnswer1+index)
				index = index + 1
			end
		end

		BaiXiaoSheng_Question_Type[RoleIndex] = 1		-- 当前题目类型
		-- 将当前时间放入table中
		BaiXiaoSheng_Time[RoleIndex] = os.time()
		-- 倒计时
		msg.AddMsgEvent(MsgID, 9, BaiXiaoSheng_Time_Counter)
		-- 已回答题目数
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Questions[RoleIndex]-1)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Questions[RoleIndex] = BaiXiaoSheng_Questions[RoleIndex] + 1 	-- 当前答题数+1
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	-- 产生百晓普通题目
	else
		BX_CallDeep = 0
		BaiXiaoSheng_Current_Question[RoleIndex].question = BX_GetQuestion(RoleIndex, 1)

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 40, BaiXiaoSheng_Current_Question[RoleIndex].question.Content)	-- 题目内容
		msg.AddMsgEvent(MsgID, 1, 200005)
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Role_Point[RoleIndex])		-- 得分

		-- 正确答案在四个选项中的随机位置
		BaiXiaoSheng_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 0
		for i=0, 3 do
			if BaiXiaoSheng_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.RightAnswer)
			else
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.WrongAnswer1+index)
				index = index + 1
			end
		end

		BaiXiaoSheng_Question_Type[RoleIndex] = 0		-- 当前题目类型
		-- 将当前时间放入table中
		BaiXiaoSheng_Time[RoleIndex] = os.time()

		-- 倒计时45秒
		msg.AddMsgEvent(MsgID, 9, BaiXiaoSheng_Time_Counter)

		-- 已回答题目数
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Questions[RoleIndex]-1)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Questions[RoleIndex] = BaiXiaoSheng_Questions[RoleIndex] + 1 	-- 已答题数+1
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

-- 活动开始
function BaiXiaoSheng_OnStart(actID)
	g_BX_RoleNum = 0
	tbl_BX_RoleID = {}
	tbl_BX_Index = {}
	tbl_BX_Questions = {}

	-- 设置随机种子
	math.randomseed(os.time())
	-- 初始化题库
	BaiXiaoSheng_InitQuestionTable()

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100037)
	msg.DispatchBroadcast(MsgID, -1, -1, -1)

	-- 在各城市中刷出npc知识竞猜

	BaiXiaoSheng_CreatureID[1] = map.MapCreateCreature(3424073512, -1, 5510301, 510, 23167, 300)---------------------------------
	BaiXiaoSheng_CreatureID[2] = map.MapCreateCreature(3424073512, -1, 5510301, 520, 23167, 310)---------------------------------
	BaiXiaoSheng_CreatureID[3] = map.MapCreateCreature(3424073512, -1, 5510301, 510, 23167, 310)----------------------------------

end


-- npc生存时间到，活动结束，在各城市中删除此npc
function BaiXiaoSheng_OnEnd(actID)
	map.MapDeleteCreature(3424073512, -1, BaiXiaoSheng_CreatureID[1])
	map.MapDeleteCreature(3424073512, -1, BaiXiaoSheng_CreatureID[2])
	map.MapDeleteCreature(3424073512, -1, BaiXiaoSheng_CreatureID[3])

	-- 清空表中内容
	BaiXiaoSheng_Attend = {}
	BaiXiaoSheng_Questions = {}
	BaiXiaoSheng_RightAnswer = {}
	BaiXiaoSheng_Time = {}
	BaiXiaoSheng_Role_Point = {}
	BaiXiaoSheng_Stepindex = {}
	BaiXiaoSheng_Role_Questions = {}
	BaiXiaoSheng_Question_Type = {}
	BaiXiao_Hard_Question_Table = {}
	BaiXiao_Normal_Question_Table = {}
	BaiXiaoSheng_Role_Level = {}
	BaiXiaoSheng_Current_Question = {}
	tbl_BX_RoleID = {}
	tbl_BX_Index = {}
	tbl_BX_Questions = {}
	g_BX_RoleNum = 0
end

-- 注册相应事件
aux.RegisterCreatureEvent(5510301, 7, "n5510301_OnTalk")
aux.RegisterActEvent(23, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(24, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(23, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(24, 2, "BaiXiaoSheng_OnStart")
