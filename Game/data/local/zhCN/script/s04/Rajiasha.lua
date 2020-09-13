local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5513010);
	Quest.SetCompleteOptions(5513010);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>阿嘉莎的幻像：<color=0xffffffff>\\n我是阿嘉莎的幻像。");

end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30650)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30650,"<font=龙字体,0,19,20><color=0xfffcf157>阿嘉莎的幻像：<color=0xffffffff>\\n找到妖精之泪，就可兑换到幻妖姬召唤石。");
       
        end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30650)then
              if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30650,QuestTalkBegin+1,"确定！" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30650,QuestTalkBegin+2,"再考虑一下" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>阿嘉莎的幻像：<color=0xffffffff>\\n你已经决定要进行兑换了么？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30650,"<font=龙字体,0,19,20><color=0xfffcf157>阿嘉莎的幻像：<color=0xffffffff>\\n看来你已经做好了决定，那么拿走幻妖姬召唤石吧。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30650,QuestTalkBegin,"离开" );
			Quest.NPCTalk(0,0,"还没准备好");
		end
       end

end

Rajiasha = {}
Rajiasha["OnTalk"] = OnTalk
Rajiasha["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rajiasha["OnCompleteQuestTalk"] = OnCompleteQuestTalk