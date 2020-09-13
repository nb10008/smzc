
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010075);
	Quest.SetCompleteOptions(5010075);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        这里是我们的土地，外来人都要注意你们的举动，因为时刻有我的人监视着你们！想留着脑袋就老老实实的！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20110)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20110	,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        我的子民，被那个恶魔控制了，他们在自相残杀，那些变成恶魔的、我的子民。。。我自己下不了手，给你一个机会。");
        elseif( id == 20111)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20111,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        我虽是个野人，但也是七十二天神之一，我为我的子民而战，获得其他天神的尊重。相对七十二神，在魔界也有七十二魔子，两股势力其实力相当。在北边有个叫做卡斯坦洞穴的地方，我曾看到黑暗的野人进入其中，你帮我去调查一下吧。");
        elseif( id == 20112)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20112,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        这个恶魔的名字叫做拉斯，曾经就一直与我为敌，现在居然就在我的面前折磨我的子民，我要让他血债血还。");
        elseif( id == 20113)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20113,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        好了，现在你就回去向<D>幻神艾丽娜</D>复命吧。");
        elseif( id == 20117)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20117,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        先问问你，对野人有多少了解，然后我再考虑是否应该相信你。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20109)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20109,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        我已经十分气愤了，你来到这里有什么事快点说，在我还没想杀死你之前。");
        elseif( id == 20110)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20110,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        让一个异族来屠杀我的子民，我的心里真是说不出的无奈，我要那个恶魔付出代价，必须！");
        elseif( id == 20111)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20111,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        果然是恶魔的味道，而且，还很熟悉。");
        elseif( id == 20112)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20112,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff>\\n        真是太感谢你了，你就如这花海中的灯塔，指引了大家的方向。");
	elseif( id == 20117)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin+1,"买不起房子的人" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin+2,"没有理性的人" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin+3,"生活在野外的人" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff><font=龙字体,0,17,20>\\n问题一：你认为野人是怎样的人？");


		elseif( step == QuestTalkBegin+1 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin+4,"卖了换钱" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin+5,"摆摊等别人买" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin+6,"当礼物送人" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff><font=龙字体,0,17,20>\\n问题二：自己不想要的垃圾装备物品最好的利用方法？");

		elseif( step == QuestTalkBegin+6 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin+7,"被天神消灭的" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin+8,"食量大吃不饱，饿死的" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin+9,"整天没事做，无聊死的" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff><font=龙字体,0,17,20>\\n问题三：恐龙是怎么灭绝的？");


		elseif( step == (QuestTalkBegin+2))then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+3))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+4))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+5))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+8))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+9))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20117,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");

		elseif( step == QuestTalkBegin+7 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20117,"<font=龙字体,0,19,20><color=0xfffcf157>杰克·萨里：<color=0xffffffff><font=龙字体,0,17,20>\\n看来你也有潜质当一名野人，你有兴趣加入到我们的行列吗？");
		end
       end

end

Rjacksali = {}
Rjacksali["OnTalk"] = OnTalk
Rjacksali["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rjacksali["OnCompleteQuestTalk"] = OnCompleteQuestTalk