
YouXiWenDa_Attend = {}				-- 是否在此活动时间内参加过
YouXiWenDa_Questions = {}				-- 问题数
YouXiWenDa_RightAnswer = {}			-- 正确答案在四个选项中的索引
YouXiWenDa_Normal_Question_Point = 10	-- 普通题目的分数
YouXiWenDa_Hard_Question_Point = 0 	-- 百晓困难题目的分数
YouXiWenDa_Time = {}					-- 存放答题开始时的系统时间，对应角色id
YouXiWenDa_Role_Point = {}			-- 玩家获得的分数，对应角色id
YouXiWenDa_Stepindex = {}				-- 每个玩家的活动步骤
YouXiWenDa_Question_Type = {}			-- 当前题目类型，百晓困难或百晓普通
YouXiWenDa_Role_Level = {}			-- 玩家当前等级
YouXiWenDa_Time_Counter = 45			-- 答题的倒计时
YouXiWenDa_Current_Question = {}      -- 玩家当前答题

YouXiWenDa_Hard_Question_Num = 0	-- 百晓困难的题目数
YouXiWenDa_Normal_Question_Num = 77	-- 普通的题目数

WD_CallDeep = 0							-- 递归调用深度


YouXiWenDa_Hard_Question_Table = {}
YouXiWenDa_Normal_Question_Table = {}

g_WD_RoleNum = 0						-- 参加知识竞猜答题活动的玩家个数
tbl_WD_RoleID = {}						-- 以g_WD_RoleNum为索引对应玩家的RoleID
tbl_WD_Index = {}						-- 以RoleID为索引对应玩家的答题Index
tbl_WD_Questions = {}					-- 玩家已经回答过的题目

-- npc对话，整个答题过程
function n5510302_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		-- 初始化空表
		YouXiWenDa_InitNullTable(RoleID)
	end

	-- 用到的属性
	YouXiWenDa_Role_Level[tbl_WD_Index[RoleID]] = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 205001)	-- "您是否决定参加游戏知识问答活动？"
		msg.AddMsgEvent(MsgID, 21, 4)		-- 确定按钮
		msg.AddMsgEvent(MsgID, 1, 100001)	-- "确定"
		msg.AddMsgEvent(MsgID, 21, 5)		-- 取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)	-- "取消"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 2
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] == 2 then
		if TalkIndex == 4 then			-- 选择了确定参加按钮后
			if YouXiWenDa_Role_Level[tbl_WD_Index[RoleID]] < 10 or YouXiWenDa_Role_Level[tbl_WD_Index[RoleID]] > 30 then	-- 等级低于10级的话弹出提示
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 205005)		-- "10级以上可以参与。"
				msg.AddMsgEvent(MsgID, 21, 4)			-- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001)		-- "确定"
				YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif YouXiWenDa_Attend[tbl_WD_Index[RoleID]] == 1 then	-- 在此活动时间内已参加过
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 205002)		-- "您已参加过本次知识竞猜活动，可继续参加下个时间的活动。"
				msg.AddMsgEvent(MsgID, 21, 4)			-- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001)		-- "确定"
				YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else
				-- “答题活动现在开始”
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 205003)		--答题活动现在开始!
				msg.AddMsgEvent(MsgID, 21, 4)
				msg.AddMsgEvent(MsgID, 1, 100001)
				msg.AddMsgEvent(MsgID, 24, TargetID)
				YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 3
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				YouXiWenDa_Attend[tbl_WD_Index[RoleID]]	= 1			-- 已参加过本次活动
				YouXiWenDa_Current_Question[tbl_WD_Index[RoleID]] = {question, index}
			end
		elseif TalkIndex == 5 then
			YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
		else
			YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
		end


	elseif YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] == 3 then				-- 答题开始
		YouXiWenDa_GenerateQuestion(RoleID, TargetID)
		YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 4


	elseif YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] == 4 then	-- 计算答题所用时间
		if TalkIndex == 5 then						-- 放弃活动则奖励玩家当前得分应得的奖励
			YouXiWenDa_RewardRole(MapID, InstanceID, RoleID)
		else
			-- 计算答题花费的时间
			local time1 = YouXiWenDa_Time[tbl_WD_Index[RoleID]]
			local dtime = os.difftime(os.time(), time1)

			-- 判断答案是否正确
			if YouXiWenDa_RightAnswer[tbl_WD_Index[RoleID]] == TalkIndex then
				-- 回答所花的时间不同，得分不同
				if dtime <= 45 then				-- 45秒内回答出正确答案，获得题目4分数
					if YouXiWenDa_Question_Type[tbl_WD_Index[RoleID]] == 1 then  	-- 百晓困难得分
						YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] = YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] + YouXiWenDa_Hard_Question_Point
					else							-- 百晓普通得分
						YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] = YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] + YouXiWenDa_Normal_Question_Point
					end
				else
					if YouXiWenDa_Question_Type[tbl_WD_Index[RoleID]] == 1 then  	-- 百晓困难得分
						YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] = YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] + YouXiWenDa_Hard_Question_Point/2
					else							-- 百晓普通得分
						YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] = YouXiWenDa_Role_Point[tbl_WD_Index[RoleID]] + YouXiWenDa_Normal_Question_Point/2
					end
				end

			end

			-- 产生题目
			if YouXiWenDa_Questions[tbl_WD_Index[RoleID]] <= 10 then    -- 答题数小于10题时继续产生题目
				YouXiWenDa_GenerateQuestion(RoleID, TargetID)
			else
				-- 答题结束
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 40, -1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				YouXiWenDa_RewardRole(MapID, InstanceID, RoleID)
				YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
			end

			-- 玩家离线
			if (role.IsRoleOnLine(MapID, InstanceID, RoleID) == 0) then
				-- 如果已经参加过活动，则获得当前分数对应的奖励，并且活动自动终止
				if (YouXiWenDa_Attend[tbl_WD_Index[RoleID]] == 1) then
					YouXiWenDa_RewardRole(MapID, InstanceID, RoleID)
					YouXiWenDa_Stepindex[tbl_WD_Index[RoleID]] = 1
				end
			end
		end
	end
end


-- 初始化空表的默认值
function YouXiWenDa_InitNullTable(RoleID)
	-- 设置一些空表的默认值
	if tbl_WD_Index[RoleID] == nil then
		g_WD_RoleNum = g_WD_RoleNum + 1
		tbl_WD_RoleID[g_WD_RoleNum] = RoleID
		tbl_WD_Index[RoleID] = g_WD_RoleNum
		tbl_WD_Questions[g_WD_RoleNum] = {}
	end

	if YouXiWenDa_Stepindex[g_WD_RoleNum] == nil then
		YouXiWenDa_Stepindex[g_WD_RoleNum] = 1
	end
	if YouXiWenDa_Attend[g_WD_RoleNum] == nil then
		YouXiWenDa_Attend[g_WD_RoleNum] = 0
	end
	if YouXiWenDa_Questions[g_WD_RoleNum] == nil then
		YouXiWenDa_Questions[g_WD_RoleNum] = 1
	end
    if YouXiWenDa_Role_Point[g_WD_RoleNum] == nil then
        YouXiWenDa_Role_Point[g_WD_RoleNum] = 0
    end
	if YouXiWenDa_Question_Type[g_WD_RoleNum] == nil then
	    YouXiWenDa_Question_Type[g_WD_RoleNum] = 0
	end
end

-- 初始化题库
function YouXiWenDa_InitQuestionTable()
	-- 将id_msg中百晓困难答题内容放入表中
	for i=1, YouXiWenDa_Normal_Question_Num do
		local id = 602001 + (i-1)*5
		YouXiWenDa_Normal_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end
end

-- 活动结束或者玩家掉线，给与玩家奖励
function YouXiWenDa_RewardRole(MapID, InstanceID, RoleID)
	local RoleIndex = tbl_WD_Index[RoleID]
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level > 100 then
		level = 100
	end
	level = math.floor(level/5)*5

	-- 经验
	local YouXiWenDa_reward = {
	[10]=1800,[15]=3000,
	[20]=5995,[25]=8190,[30]=15624
	}
	local addexp = YouXiWenDa_reward[level]
	role.AddRoleExp(MapID, InstanceID, RoleID, addexp*(YouXiWenDa_Role_Point[RoleIndex]/100))

	YouXiWenDa_Stepindex[RoleIndex] = 1

	if role.GetTrialState(RoleID, 1006) == 1 then
		role.SetTrialCompleteNum(RoleID, 1006, role.GetTrialCompleteNum(RoleID,1006) + 1)
    end
	if role.GetTrialState(RoleID, 1006) == 1 and role.GetTrialCompleteNum(RoleID,1006) >= Trail_maxnum[1006] then
		role.TrialComplete(RoleID,1006)
	end
end

-- 返回不重复的题目  参数0为困难题目 1为普通题目
function WD_GetQuestion(RoleIndex, n)
	WD_CallDeep = WD_CallDeep + 1		-- 递归深度+1
	local question
	local index



	if n == 0 then
		if table.maxn(YouXiWenDa_Hard_Question_Table) == 0 then	--判断之前的活动进行过程中，是否因重载脚本或服务器重启等原因造成题库表被置空
			YouXiWenDa_InitQuestionTable()
		end
		index = math.random(1, table.maxn(YouXiWenDa_Hard_Question_Table))
		question = YouXiWenDa_Hard_Question_Table[index]
	else
		if table.maxn(YouXiWenDa_Normal_Question_Table) == 0 then	--判断之前的活动进行过程中，是否因重载脚本或服务器重启等原因造成题库表被置空
			YouXiWenDa_InitQuestionTable()
		end
		index = math.random(1, table.maxn(YouXiWenDa_Normal_Question_Table))
		question = YouXiWenDa_Normal_Question_Table[index]
	end

	if tbl_WD_Questions[RoleIndex] == nil then
		local question_table = {}
		tbl_WD_Questions[RoleIndex] = {question_table}
		table.insert(tbl_WD_Questions[RoleIndex], question.Content)
		return question
	end

	local flag = 0
	for k, v in pairs(tbl_WD_Questions[RoleIndex]) do
		if v == question.Content then
			flag = 1
			break
		end
	end

	if flag == 1 then
		if WD_CallDeep >= 10 then			-- 递归深度超过10的话就直接返回重复的题
			return question
		else
			return WD_GetQuestion(RoleIndex, n)
		end
	else
		table.insert(tbl_WD_Questions[RoleIndex], question.Content)
		return question
	end
end

-- 产生题目和选项
function YouXiWenDa_GenerateQuestion(RoleID, TargetID)
	local RoleIndex = tbl_WD_Index[RoleID]

	-- 产生题目

	WD_CallDeep = 0
	YouXiWenDa_Current_Question[RoleIndex].question = WD_GetQuestion(RoleIndex, 1)

	local MsgID = msg.BeginMsgEvent()

	msg.AddMsgEvent(MsgID, 40, YouXiWenDa_Current_Question[RoleIndex].question.Content)	-- 题目内容
	msg.AddMsgEvent(MsgID, 1, 205004)
	msg.AddMsgEvent(MsgID, 12, YouXiWenDa_Role_Point[RoleIndex])		-- 得分

	-- 正确答案在四个选项中的随机位置
	YouXiWenDa_RightAnswer[RoleIndex] = math.random(0, 3)
	local index = 0
	for i=0, 3 do
		if YouXiWenDa_RightAnswer[RoleIndex] == i then
			msg.AddMsgEvent(MsgID, 21, i)
			msg.AddMsgEvent(MsgID, 1, YouXiWenDa_Current_Question[RoleIndex].question.RightAnswer)
		else
			msg.AddMsgEvent(MsgID, 21, i)
			msg.AddMsgEvent(MsgID, 1, YouXiWenDa_Current_Question[RoleIndex].question.WrongAnswer1+index)
			index = index + 1
		end
	end

	YouXiWenDa_Question_Type[RoleIndex] = 0		-- 当前题目类型
	-- 将当前时间放入table中
	YouXiWenDa_Time[RoleIndex] = os.time()

	-- 倒计时45秒
	msg.AddMsgEvent(MsgID, 9, YouXiWenDa_Time_Counter)

	-- 已回答题目数
	msg.AddMsgEvent(MsgID, 12, YouXiWenDa_Questions[RoleIndex]-1)
	msg.AddMsgEvent(MsgID, 24, TargetID)
	YouXiWenDa_Questions[RoleIndex] = YouXiWenDa_Questions[RoleIndex] + 1 	-- 已答题数+1
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

-- 活动开始
function YouXiWenDa_OnStart(actID)
	g_WD_RoleNum = 0
	tbl_WD_RoleID = {}
	tbl_WD_Index = {}
	tbl_WD_Questions = {}

	-- 设置随机种子
	math.randomseed(os.time())
	-- 初始化题库
	YouXiWenDa_InitQuestionTable()
end


-- npc生存时间到，活动结束，在各城市中删除此npc
function YouXiWenDa_OnEnd(actID)

	-- 清空表中内容
	YouXiWenDa_Attend = {}
	YouXiWenDa_Questions = {}
	YouXiWenDa_RightAnswer = {}
	YouXiWenDa_Time = {}
	YouXiWenDa_Role_Point = {}
	YouXiWenDa_Stepindex = {}
	YouXiWenDa_Role_Questions = {}
	YouXiWenDa_Question_Type = {}
	YouXiWenDa_Hard_Question_Table = {}
	YouXiWenDa_Normal_Question_Table = {}
	YouXiWenDa_Role_Level = {}
	YouXiWenDa_Current_Question = {}
	tbl_WD_RoleID = {}
	tbl_WD_Index = {}
	tbl_WD_Questions = {}
	g_WD_RoleNum = 0
end

-- 注册相应事件
aux.RegisterCreatureEvent(5510302, 7, "n5510302_OnTalk")-------------------------
aux.RegisterActEvent(26, 3, "YouXiWenDa_OnEnd")
aux.RegisterActEvent(26, 2, "YouXiWenDa_OnStart")
