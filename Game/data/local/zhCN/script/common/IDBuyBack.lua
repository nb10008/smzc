local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"查看我的历练进度");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,0,"领取历练豪华大礼包（可多次领取)");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,2,0,"放弃礼包同意收回账号");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,3,0,"关闭");
		Quest.SetAcceptOptions(5513017);
	  Quest.SetCompleteOptions(5513017);
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>账号回购人：<color=0xffffffff>\\n        来看看我能给予的奖励吧。");
end

local function OnScenarioTalk(id,step)

	if( id == 0 )then  --查看我的历练进度
		Quest.ViewLiLian();
		Quest.CloseNPCTalk();
	elseif( id == 1 )then --领取历练豪华大礼包（可多次领取
		Quest.ReceiveGift();
		Quest.CloseNPCTalk();
	elseif( id == 2 )then --放弃礼包同意收回账号
		Quest.IDBuyBack();
		Quest.CloseNPCTalk();
	elseif( id == 3 ) then --关闭
		Quest.CloseNPCTalk();
	end

end

local function OnCompleteQuestTalk(id,step)
	if( id == 30804)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30804,QuestTalkBegin+1,"确定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30804,QuestTalkBegin+2,"返回" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>账号回购人：<color=0xffffffff>\\n        你是不是已经垂涎我的奖励了呢。");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.UpdateQuest(30804);
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		end
	end
end

IDBuyBack = {}
IDBuyBack["OnTalk"] = OnTalk
IDBuyBack["OnScenarioTalk"] = OnScenarioTalk
IDBuyBack["OnCompleteQuestTalk"] = OnCompleteQuestTalk