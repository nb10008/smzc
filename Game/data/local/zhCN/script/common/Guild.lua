

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"创建公会");					--设置对话选项
	Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,0,"解散公会");
	Quest.NPCTalk(0,0,"公会官员：您好，请问您需要什么帮助。");
end

local function OnScenarioTalk(id,step)
	if( id == 0 )then --创建公会
		local playerlv = Tooltip.GetLocalPlayerLevel()
		if playerlv >= 20 then
			Quest.GuildCreate();
		else
			Quest.NPCTalk(0,0,"公会官员：等级大于等于20级的玩家才可创建工会，所需花费为20金。");
		end
	elseif( id == 1 )then --解散公会
		Quest.GuildDismiss();
	end
end

Guild = {}
Guild["OnTalk"] = OnTalk
Guild["OnScenarioTalk"] = OnScenarioTalk
Guild["OnCompleteQuestTalk"] = OnCompleteQuestTalk
