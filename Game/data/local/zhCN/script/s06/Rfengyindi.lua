
local function OnTalk()
	Quest.ClearNPCTalkOptions();  --清除对话框选项
	Quest.SetAcceptOptions(5010212);
	Quest.SetCompleteOptions(5010212);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>武岩封印·地<color=0xffffffff>");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取

end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)


		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20411,QuestTalkBegin+1,"解除封印" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20411,QuestTalkBegin+2,"返回" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>武岩封印·地<color=0xffffffff>：\\n你要解除这里的封印石吗？ ");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.UpdateQuest(20411);
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		end

end

Rfengyindi = {}
Rfengyindi["OnTalk"] = OnTalk
Rfengyindi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rfengyindi["OnCompleteQuestTalk"] = OnCompleteQuestTalk