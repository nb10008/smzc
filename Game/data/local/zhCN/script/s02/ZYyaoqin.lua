
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010194);
	Quest.SetCompleteOptions(5010194);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n优秀的治疗和辅助职业，同时也拥有很强的攻击法术，是个让人觉得十分可靠的职业。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 41016)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41016,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n看来你是想换个职业，如果你已经决定了。若要转换职业，首先要变为一转职业并需要一个一转印记，然后再去完成对应的二转任务即可。我会将你的职业改为仙音，如果你需要的话。");
        elseif( id == 41020)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41020,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n你已经转职成为仙音，现在回去斗神之王处接取新的任务吧。");	
	elseif( id == 41027)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41027,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n你只要把二转印记交给我，就可以转换你的职业成为神侍了。");	
	elseif( id == 41028)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41028,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n你只要把二转印记交给我，就可以转换你的职业成为天仪了。");	

	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
       if( id == 41000)then
                if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,41000,QuestTalkBegin+1,"我知道了" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,41000,QuestTalkBegin+2,"取消" );
	     	        Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff><font=龙字体,0,17,20>\\n        优秀的治疗和辅助职业，同时也拥有很强的攻击法术，是个让人觉得十分可靠的职业。");
		elseif( step == QuestTalkBegin+1 )then
	                Quest.UpdateQuest(41000);
	        elseif( step == QuestTalkBegin+2 )then
	                OnTalk();
                end
	elseif( id == 41004)then
			if( not Quest.QuestCanComplete(41004) ) then

				if( step == QuestTalkBegin )then
					Quest.ClearNPCTalkOptions();
					Quest.SetNPCTalkOption( EOT_CompleteQuest,41004,QuestTalkBegin+1,"好的，我已经决定了" );
					Quest.SetNPCTalkOption( EOT_CompleteQuest,41004,QuestTalkBegin+2,"我要在想一想" );
					Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n        既然已经决定了，那就将武之契交给我吧，我会将仙音的力量赋予你！仙音拥有优秀的治疗和辅助技能，同时也拥有很强的攻击法术。");
				elseif( step == QuestTalkBegin+1 )then
					Quest.ClearNPCTalkOptions();
					Quest.SetNPCTalkOption( EOT_CompleteQuest,41004,QuestTalkBegin+3,"确定" );
					Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n    那么请将武之契交给我吧。");
				elseif( step == QuestTalkBegin+3 )then
					Quest.UpdateQuest(41004);
				elseif( step == QuestTalkBegin+2 ) then
					Quest.CloseNPCTalk();
				end
			else

				Quest.CloseNPCTalk();
				Quest.CompleteQuestTalk(41004,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n        那么仙音的力量你马上就可以感受到了。 \\n    <color=0xffff00ff>提示：因只可拥有一种职业，若同时接取其他转职任务，在完成此任务后请删除即可<color=0xffffffff>。");

			end
        elseif( id == 41011)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41011,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n既然你已经拿到了圣武之契，说明你已经将那上古圣器击败了，那么来感受下<color=0xff80ff00>神侍<color=0xffffffff>的威力吧！ ");
        elseif( id == 41012)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41012,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n既然你已经拿到了圣武之契，说明你已经将那上古圣器击败了，那么来感受下<color=0xff80ff00>天仪<color=0xffffffff>的威力吧！  ");
        elseif( id == 41016)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,41016,QuestTalkBegin+1,"交付一转印记" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,41016,QuestTalkBegin+2,"我再想想" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>宝器之灵：<color=0xffffffff>\\n    转职后所有的技能点数也会被重置，你真的决定转职为仙音的话，就把一转印记交给我吧。");
		elseif( step == QuestTalkBegin+2 ) then
			Quest.CloseNPCTalk();
		elseif( step == QuestTalkBegin+1 )then
			Quest.ClearNPCTalkOptions();
			Quest.CompleteQuestTalk(41016,"<font=龙字体,0,19,20><color=0xfffcf157>宝器之灵：<color=0xffffffff>\\n    点击完成，你就转职为仙音了。");
		end
       elseif( id == 41027)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41027,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n那么请将二转印记交给我吧。");
       elseif( id == 41028)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41028,"<font=龙字体,0,19,20><color=0xfffcf157>魔琴之灵：<color=0xffffffff>\\n那么请将二转印记交给我吧。");
       end

end

ZYyaoqin = {}
ZYyaoqin["OnTalk"] = OnTalk
ZYyaoqin["OnAcceptQuestTalk"] = OnAcceptQuestTalk
ZYyaoqin["OnCompleteQuestTalk"] = OnCompleteQuestTalk