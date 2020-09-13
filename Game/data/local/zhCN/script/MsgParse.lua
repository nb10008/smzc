local function ParseString(desc,params)
	if( desc ~= nil ) then
		for i=1, #params do
			pat = string.format("<p%d>",i)
			if params[i] ~= nil then
				desc = string.gsub(desc,pat,params[i])
			else
				desc = string.gsub(desc,pat,"")
			end
		end
	end

	return desc
end

--取整
local function GetInteger(x)
		return x-x%1
end

function OnParse()
	local talk
	local entry
	local ret
	local paramIndex = 1
	local index
	local optIndex = 0
	local optValue
	local params = {}
	local targetType
	local targetValue

	ret = Parse.ParseType()
	if( ret == EMUT_DlgWithSelOpt )	then		--带选项对话框
		--初始化对话框
		Quest.ClearNPCTalkOptions()

		--内容
		index = Parse.ParseDWORD()
		talk = Parse.GetMsgValue(ret,index)

		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--目标
			if( ret == EMUT_DlgTarget_Quest ) then
				targetType = ret
				targetValue = Parse.ParseDWORD()
			elseif( ret == EMUT_DlgTarget_Item ) then
				targetType = ret
				targetValue = Parse.ParseDWORD()
			elseif( ret == EMUT_DlgTarget_Role ) then
				targetType = ret
				targetValue = Parse.ParseDWORD()
			--选项
			elseif( ret == EMUT_DlgOption ) then
				optValue = Parse.ParseDWORD()
			--选项内容
			elseif( ret == EMUT_MsgID ) then
				index = Parse.ParseDWORD()
				if( index ~= nil ) then	--在对话框中增加选项
					entry = Parse.GetMsgValue(ret,index)
					Quest.SetNPCTalkOption(EOT_ServerCtrl,optIndex,optValue,entry)
					optIndex = optIndex + 1
				end
			--参数
			else
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				paramIndex = paramIndex + 1
			end

			ret = Parse.ParseType()
		end

		--解析获得对话
		talk = ParseString(talk,params)

		--显示对话框
		Quest.NPCTalk(targetType,targetValue,talk)

	elseif( ret == EMUT_DlgQuiz ) then			--答题界面
		Quiz.Clear()

		--内容
		index = Parse.ParseDWORD()
		if( index == -1 ) then
			Quiz.Close()
			return
		end

		talk = Parse.GetMsgValue(ret,index)

		integerParam = 0
		msgidParam = 0;

		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--目标
			if( ret == EMUT_DlgTarget_Quest ) then
				targetType = ret
				targetValue = Parse.ParseDWORD()
			elseif( ret == EMUT_DlgTarget_Item ) then
				targetType = ret
				targetValue = Parse.ParseDWORD()
			elseif( ret == EMUT_DlgTarget_Role ) then
				targetType = ret
				targetValue = Parse.ParseDWORD()
			--选项
			elseif( ret == EMUT_DlgOption ) then
				optValue = Parse.ParseDWORD()
			--选项内容
			elseif( ret == EMUT_MsgID ) then
				if( msgidParam == 0 ) then		--得分情况
					index = Parse.ParseDWORD()
					talk = talk .. Parse.GetMsgValue(ret,index)

					msgidParam = msgidParam + 1
				else
					index = Parse.ParseDWORD()
					if( index ~= nil ) then	--在对话框中增加选项
						entry = Parse.GetMsgValue(ret,index)
						Quiz.SetOption(entry,optIndex,optValue)
						optIndex = optIndex + 1
					end
				end
			--倒计时
			elseif( ret == EMUT_Integer ) then
				cd = Parse.ParseDWORD()
				Quiz.SetCD(cd)
			--已答题数量
			elseif( ret == EMUT_Num ) then
				if( integerParam == 0 ) then	--参数
					index = Parse.ParseDWORD()
					params[paramIndex] = Parse.GetMsgValue(ret,index)
					paramIndex = paramIndex + 1

					integerParam = integerParam + 1;
				else
					num = Parse.ParseDWORD()
					Quiz.SetNum(num)
				end
			end

			ret = Parse.ParseType()
		end

		--解析获得对话
		talk = ParseString(talk,params)

		--显示对话框
		Quiz.Show(targetType,targetValue,talk)

	elseif( ret == EMUT_ChatInfo ) then		--聊天频道显示
		chatIndex = Parse.ParseDWORD()
		ret = Parse.ParseType()
		hasRole = 0
		hasGuild = 0
		while( ret ~= nil ) do
			--显示信息
			if( ret == EMUT_MsgID ) then
				index = Parse.ParseDWORD()
				talk = Parse.GetMsgValue(ret,index)
			--人名，异步特殊处理
			elseif (ret == EMUT_RoleID) then
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret, index)
				hasRole = 1
				paramIndex = paramIndex + 1
			--帮派，异步特殊处理
			elseif (ret == EMUT_GuildID) then
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret, index)
				hasGuild = 1
				paramIndex = paramIndex + 1
			--触发器坐标
			elseif( ret == EMUT_TriggerID ) then
				index = Parse.ParseDWORD()
				posx,posz = Common.GetTriggerPos(index)
				params[paramIndex] = GetInteger(posx/50)
				paramIndex = paramIndex + 1
				params[paramIndex] = GetInteger(posz/50)
				paramIndex = paramIndex + 1
			--其他参数
			else
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				paramIndex = paramIndex + 1
			end

			ret = Parse.ParseType()
		end

		--解析获得信息
		talk = ParseString(talk,params)

		--显示信息
		if (hasRole == 1 or hasGuild == 1) then
			Common.ShowChatChannelInfo(talk, chatIndex, 1)
		else
			Common.ShowChatChannelInfo(talk, chatIndex, 0)
		end

	elseif( ret == EMUT_NewBroadcast ) then		--新型广播
		local broadcastID
		index = Parse.ParseDWORD()
		broadcastID = index
		talk = Parse.GetMsgValue(ret,index)
		hasRole = 0
		hasGuild = 0
		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--角色名称是异步获得的，需要特殊处理
			if( ret == EMUT_RoleID ) then
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				hasRole = 1
				paramIndex = paramIndex + 1
			--帮派名称是异步获得的，需要特殊处理
			elseif(ret == EMUT_GuildID) then
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				hasGuild = 1
				paramIndex = paramIndex + 1
			--其他参数
			else
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				paramIndex = paramIndex + 1
			end

			ret = Parse.ParseType()
		end

		--解析获得信息
		talk = ParseString(talk,params)
		if (hasRole == 1 or hasGuild == 1) then
		--显示信息
			Common.ShowInfoNewBoardcast(broadcastID,talk,1)
		else
			Common.ShowInfoNewBoardcast(broadcastID,talk,0)
		end


	elseif( ret == EMUT_Boardcast ) then		--广播
		index = Parse.ParseDWORD()
		talk = Parse.GetMsgValue(ret,index)
		hasRole = 0
		hasGuild = 0
		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--角色名称是异步获得的，需要特殊处理
			if( ret == EMUT_RoleID ) then
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				hasRole = 1
				paramIndex = paramIndex + 1
			--帮派名称是异步获得的，需要特殊处理
			elseif(ret == EMUT_GuildID) then
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				hasGuild = 1
				paramIndex = paramIndex + 1
			--其他参数
			else
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				paramIndex = paramIndex + 1
			end

			ret = Parse.ParseType()
		end

		--解析获得信息
		talk = ParseString(talk,params)
		if (hasRole == 1 or hasGuild == 1) then
		--显示信息
			Common.ShowInfo(EMUT_Boardcast,talk,1)
		else
			Common.ShowInfo(EMUT_Boardcast,talk,0)
		end

	elseif( ret == EMUT_BigBroadcast ) then		--广播
		index = Parse.ParseDWORD()
		talk = Parse.GetMsgValue(ret,index)
		hasRole = 0
		hasGuild = 0
		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--角色名称是异步获得的，需要特殊处理
			if( ret == EMUT_RoleID ) then
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				hasRole = 1
				paramIndex = paramIndex + 1
			--帮派名称是异步获得的，需要特殊处理
			elseif(ret == EMUT_GuildID) then
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				hasGuild = 1
				paramIndex = paramIndex + 1
			--其他参数
			else
				index = Parse.ParseDWORD()
				params[paramIndex] = Parse.GetMsgValue(ret,index)
				paramIndex = paramIndex + 1
			end

			ret = Parse.ParseType()
		end

		--解析获得信息
		talk = ParseString(talk,params)
		if (hasRole == 1 or hasGuild == 1) then
		--显示信息
			Common.ShowInfo(EMUT_BigBroadcast,talk,1)
		else
			Common.ShowInfo(EMUT_BigBroadcast,talk,0)
		end

	elseif( ret == EMUT_QuestID ) then			--获得任务描述
		--任务ID
		questID = Parse.ParseDWORD()

		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--任务总环数
			if( ret == EMUT_TotalLoop )	then
				total = Parse.ParseDWORD()
				ring = (total-1) / 10
				index = total - GetInteger(ring)*10
				msgInfo = "当前为第" .. GetInteger(ring)+1 .. "轮第" .. GetInteger(index) .. "环任务"
				Quest.ShowScreenInfo( msgInfo )
			--任务描述
			elseif( ret == EMUT_MsgID ) then
				index = Parse.ParseDWORD()
				questDesc = Parse.GetMsgValue(EMUT_QuestID,index)
				Quest.SetQuestDesc(questID,questDesc)
			end

			ret = Parse.ParseType()
		end

	elseif( ret == EMUT_ErrorCode ) then			--错误消息
		msgID = Parse.ParseDWORD()
		msgInfo = Parse.GetMsgValue(ret,msgID)

		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--参数
			index = Parse.ParseDWORD()
			params[paramIndex] = Parse.GetMsgValue(ret,index)
			paramIndex = paramIndex + 1

			ret = Parse.ParseType()
		end

		--解析获得信息
		msgInfo = ParseString(msgInfo,params)

		Quest.ShowScreenInfo( msgInfo )

	elseif( ret == EMUT_PVPActivity ) then		--PVP活动个人信息
		local msgID = Parse.ParseINT()

		if( msgID==-1 ) then
			PvP.PvPSetActivityID("", -1)
			return
		end

		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--pvp活动ID、活动名
			if( ret == EMUT_DlgTarget_Activity ) then
				local pvpID = Parse.ParseDWORD()
				local pvpName = Parse.GetMsgValue(EMUT_DlgTarget_Activity, msgID)
				PvP.PvPSetActivityID(pvpName, pvpID)
			--pvp活动剩余时间
			elseif( ret == EMUT_Time ) then
				local pvpCD = Parse.ParseDWORD()
				PvP.PvPSetActivityCD(pvpCD)
			--pvp活动当前积分
			elseif( ret == EMUT_Score ) then
				local pvpScore = Parse.ParseDWORD()
				PvP.PvPSetActivityScore(pvpScore)
			--pvp活动当前剩余复活次数
			elseif( ret == EMUT_RebornTime ) then
				local pvpReborn = Parse.ParseDWORD()
				PvP.PvPSetActivityRebornTime(pvpReborn);
			--pvp活动当前队伍ID
			elseif( ret == EMUT_TeamID ) then
				local pvpTeamID = Parse.ParseDWORD()
				PvP.PvPSetActivityTeamID(pvpTeamID)
			end
			ret = Parse.ParseType()
		end

		elseif( ret == EMUT_Time ) then		--PVP活动时间
		local msgID = Parse.ParseINT()

		if( msgID==-1 ) then
			PvP.PvPSetActivityID("", -1)
			return
		end

		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--pvp活动剩余时间
			if( ret == EMUT_Time ) then
				local pvpCD = Parse.ParseDWORD()
				PvP.PvPSetActivityCD(pvpCD)
			end
			ret = Parse.ParseType()
		end

	elseif( ret == EMUT_DlgArrange ) then		--pvp活动积分排名信息
		local pvpType = Parse.ParseDWORD()
		PvP.PvPSetActivityType(pvpType)
		ret = Parse.ParseType()
		local pvpNameID
		local pvpScore
		local pvpTeamID
		local pvpRank
		local pvpIndex = 0
		while( ret ~= nil ) do
			if( ret == EMUT_Rank  ) then
				pvpRank = Parse.ParseDWORD()
				pvpIndex = pvpIndex + 1
			elseif( ret == EMUT_RoleID ) then
				pvpNameID = Parse.ParseDWORD()
				pvpIndex = pvpIndex + 1
			elseif( ret == EMUT_Score ) then
				pvpScore = Parse.ParseDWORD()
				pvpIndex = pvpIndex + 1
			elseif( ret == EMUT_TeamID ) then
				pvpTeamID = Parse.ParseDWORD()
				pvpIndex = pvpIndex + 1
			end

			if( pvpType==0 and pvpIndex==3 ) then
				PvP.PvPScoreSort(pvpRank, pvpNameID, pvpScore)
				pvpIndex = 0
			elseif( pvpType==1 and pvpIndex==4 ) then
				PvP.PvPMultiScoreSort(pvpRank, pvpNameID, pvpScore, pvpTeamID)
				pvpIndex = 0
			end

			ret = Parse.ParseType()
		end
		PvP.PvPScoreSortInfo()
	elseif( ret == EMUT_DlgPopup ) then				--弹窗提示，可寻路
		local ndx = Parse.ParseDWORD()
		local strContent = Parse.GetMsgValue(ret,ndx)

		ret = Parse.ParseType()
		while( ret ~= nil ) do
			--参数
			index = Parse.ParseDWORD()
			params[paramIndex] = Parse.GetMsgValue(ret,index)
			paramIndex = paramIndex + 1

			ret = Parse.ParseType()
		end

		--解析获得信息
		strContent = ParseString(strContent,params)

		if strContent ~= nil then
			Common.ShowDialog( strContent )
		end
		--local index = Parse.ParseDWORD()
		--local strContent = Parse.GetMsgValue(ret,index)
		--if strContent ~= nil then
		--	Common.ShowDialog( strContent )
		--end
	--elseif( ret == EMUT_DlgShowMsg ) --服务器控制消息对话框，可以显示很多内容多行
	--end
	elseif( ret == EMUT_UIEffect ) then
		local nid = Parse.ParseDWORD()
		local strContent = Parse.GetMsgValue(ret,nid)
		Common.ShowUIEffect(strContent)
	end
end
