local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5511027);
	Quest.SetCompleteOptions(5511027);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉：<color=0xffffffff>\\n了解自己的妖精，对自己很有帮助。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40048)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40048,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff>\\n   你现在应该也拥有属于自己的妖精了吧？你知道吗，在妖精达到30级以后，就会萌发自己对主人的感情，我们叫做妖精的心情。下面就让我好好给你讲解一下吧！");
        elseif( id == 40049)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40049,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff>\\n   既然要提升妖精的心情需要妖精食物，那我们就先去买一个妖精食物来吧。在天雪镇就有一个妖精食物商，先去买个秘制话梅回来吧，之后给你讲讲妖精食物的分类。");
        elseif( id == 40050)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40050,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff>\\n   如果你的妖精心情不是在亢奋状态的话，就试着将刚才买的秘制话梅喂给你的妖精吧。只要直接使用妖精食物就可以了，快去试试吧。");
 
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40048)then
             if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40048,QuestTalkBegin+1,"好的，我了解了" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40048,QuestTalkBegin+2,"稍等，我再看看" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff><font=龙字体,0,17,20>\\n妖精的心情分为：沉寂、低迷、沮丧、失落、愉悦、亢奋，当妖精达到30级，就可以在妖精的界面看到妖精的心情了。");

		elseif( step == QuestTalkBegin+1 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40048,QuestTalkBegin+3,"好的，我了解了" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40048,QuestTalkBegin+4,"稍等，我再看看" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff><font=龙字体,0,17,20>\\n妖精的心情会影响他们能力的发挥，比如他们获得经验的多少，对主人能力增加的多少，还有触发技能的几率，都会因为妖精的心情或高或低。");

		elseif( step == QuestTalkBegin+3 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40048,QuestTalkBegin+5,"好的，我了解了" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40048,QuestTalkBegin+6,"稍等，我再看看" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff><font=龙字体,0,17,20>\\n若要提升妖精的心情，就要使用妖精食物，妖精食物可以在妖精食物商处购买，也可通过神秘的妖精厨师合成获得。某些食物还具备额外状态以及可以提升一定的培育程度，这个就靠你自己去发现了。");
			
		elseif( step == QuestTalkBegin+5 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40048,QuestTalkBegin+7,"好的，我都了解了" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40048,QuestTalkBegin+8,"稍等，我再看看" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff><font=龙字体,0,17,20>\\n最后要说一下，妖精的心情会根据时间而减少的，包括主人死亡的时候也会减少妖精的心情。最最最后说一下！在妖精处于亢奋状态的时候是不会吃妖精食物的，所以就不要在这个时候浪费你的妖精食物了，记住了吗？");
		
		elseif( step == QuestTalkBegin+7 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40048,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff><font=龙字体,0,17,20>\\n我想你应该懂了，这就是妖精的心情，自己打开妖精界面去看看吧。");
		end
        elseif( id == 40049)then
		if( step == QuestTalkBegin ) then
			Quest.ClearNPCTalkOptions();                    --清除对话框选项
			Quest.SetNPCTalkOption(EOT_CompleteQuest,40049,QuestTalkBegin+1, "选项一：普通食物：" );
			Quest.SetNPCTalkOption(EOT_CompleteQuest,40049,QuestTalkBegin+2, "选项二：优质食物：" );
			Quest.SetNPCTalkOption(EOT_CompleteQuest,40049,QuestTalkBegin+3, "选项三：稀有食物：" );
			Quest.SetNPCTalkOption(EOT_CompleteQuest,40049,QuestTalkBegin+4, "我懂了" );
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff>\\n请选择要查看的信息：")
		elseif( step == QuestTalkBegin+1 ) then 
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff>\\n用游戏币购买即可获得，使用后可恢复少量的心情");
		elseif( step == QuestTalkBegin+2 ) then 
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff>\\n需要收集食材合成后获得，使用后可提升一定的心情，某些食物还具备额外状态以及可以提升一定的培育程度");
		elseif( step == QuestTalkBegin+3 ) then 
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff>\\n需要收集食材、食谱后合成获得，使用后可提升心情和大量培育程度，具备比较优秀的额外状态");
		elseif( step == QuestTalkBegin+4 ) then 
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40049,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff><font=龙字体,0,17,20>\\n以后要好好照顾你的妖精哦。");
		end
              
        elseif( id == 40050)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40050,"<font=龙字体,0,19,20><color=0xfffcf157>妖精法蒂拉<color=0xffffffff>\\n很好，看来你已经知道如何控制和提升妖精的心情了，那么这两个妖精食物就送给你吧。");


       end

end

Rfadila = {}
Rfadila["OnTalk"] = OnTalk
Rfadila["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rfadila["OnCompleteQuestTalk"] = OnCompleteQuestTalk