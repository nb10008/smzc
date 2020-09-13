
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5511013);
	Quest.SetCompleteOptions(5511013);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>市场管理员：<color=0xffffffff>\\n在这里是斗战天城的摆摊区，在这里可以将自己的物品卖给其他玩家。摆摊的话，打开背包就可以看到“摆摊”的按钮，然后将物品放到对应的位置上，输入想要卖出的价格就好了。");

end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取      

end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 20021)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20021,QuestTalkBegin+1,"确定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20021,QuestTalkBegin+2,"返回" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>市场管理员：<color=0xffffffff>\\n这里就是摆摊区了，我是这里的管理员。在这里可要公平交易，打开你的背包就能看到摆摊的按钮了，有兴趣就来做生意吧。");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.UpdateQuest(20021);
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		end
	end
end

Rshichang = {}
Rshichang["OnTalk"] = OnTalk
Rshichang["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rshichang["OnCompleteQuestTalk"] = OnCompleteQuestTalk