

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    								--清除对话框选项
	Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"创建家族");					--设置对话选项
	Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,0,"解散家族");
	Quest.SetAcceptOptions(5610022);
	Quest.SetCompleteOptions(5610022);
	Quest.NPCTalk(0,0,"家族官员：\\n    如果你想创建一个家族，那么你需要和3个尚未加入家族的朋友组队后再来找我。");
end

local function OnScenarioTalk(id,step)
	if( id == 0 )then --创建家族
		Quest.FamilyCreate();
	elseif( id == 1 )then --解散家族
		Quest.FamilyBreak();
	end
end

local function OnCompleteQuestTalk(id,step)
	if( id == 30801)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30801,QuestTalkBegin+1,"确定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30801,QuestTalkBegin+2,"返回" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>家族官员：<color=0xffffffff>\\n        家族是最亲密最强大的组织，想要组建或者加入家族可以随时来找我。");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.UpdateQuest(30801);
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		end
	end
end


Family = {}
Family["OnTalk"] = OnTalk
Family["OnScenarioTalk"] = OnScenarioTalk
Family["OnCompleteQuestTalk"] = OnCompleteQuestTalk

