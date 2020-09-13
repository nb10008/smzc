local function OnTalk()
	Quest.ClearNPCTalkOptions();
	Quest.SetNPCTalkOption(13,0,0,"晋升神职");
	Quest.SetNPCTalkOption(13,1,0,"竞选神职");
	Quest.SetNPCTalkOption(13,2,0,"查询上期结果");
	Quest.SetNPCTalkOption(13,3,0,"查看神之系");
	Quest.SetNPCTalkOption(13,4,0,"神之血脉");
	Quest.SetNPCTalkOption(13,5,0,"取消");
	Quest.SetAcceptOptions(5010243);
	Quest.SetCompleteOptions(5010243);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        如果你觉得自己足够强大，那么你应该来和我谈一谈。");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 40058)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40058,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        学会灵魂净化了吗？好的，现在来练习一下，我这里有一个魔化之瓶，打开它之后会放出一个带有魔气萦绕状态的怪物。使用灵魂净化杀死它，然后回来告诉我。")
	elseif( id == 40059)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40059,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        想要提升自己的神格就必须要使用凝聚技能把信仰转化成凝聚度才行。当你拥有1级的神格的时候再来找我吧。")
	elseif( id == 40060)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40060,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你可以在我这里选一个合适的神职来晋升，之后就可以使用神化技能了。来，使用一次神化技能让我看看。")
	elseif( id == 40061)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40061,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你想激活旅行的自选领域吗？你必须要将自己的神格提升到2级才行。")
	elseif( id == 40062)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40062,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你现在已经足够强大了，是时候进入上古妖精庇护所去获取更多的信仰了。上古妖精庇护所会在每个小时的最后10分钟开启大门，你需要等到那个时候前往西洲幻林、南洲迷沼、东洲冥山才能进入。")
  elseif( id == 40063)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40063,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        成为神之后将可以装备专属的圣衣了。每件圣衣的制作都是相当困难的，因为它需要用到许多平时无法取得的珍贵材料。去了解一下如何取得这些材料，然后带一个深渊水晶回来。")
	elseif( id == 40064)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40064,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        圣衣里蕴含的力量非常强大，想要装备圣衣的话必须要达到三级神格。当你觉得自己能够胜任的时候再来找我。")
	elseif( id == 40065)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40065,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        现在你已经是一个非常优秀的新神了。去吧，去弄一件属于你自己的圣衣武器，只有圣衣武器才能配得上你现在的实力。")
  elseif( id == 40066)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40066,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        预言者盖拉是我在亿万凡人中选定的代言人，他已经跟随了我好几百年。如果你想获得更多的信仰的话就去奔雷裂谷找预言者盖拉吧。")
	elseif( id >= 30350 and id <= 30365)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(id,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        如果你想继续突破现有的能力极限，那就找到我需要的东西，我就教你超越自己的方法。")
	elseif( id == 30874)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(30874,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        在你将它呈来之前，我几乎认定它们已经跟随上一次的神魔大战而湮没，我担心接下来发生的事情会让三界更加震惊……我们的确需要有驾驭它的人……而你，你是否有支配它的力量……")
	elseif( id == 30875)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(30875,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        在你将它呈来之前，我几乎认定它们已经跟随上一次的神魔大战而湮没，我担心接下来发生的事情会让三界更加震惊……我们的确需要有驾驭它的人……而你，你是否有支配它的力量……")
	elseif( id == 30876)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(30876,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        在你将它呈来之前，我几乎认定它们已经跟随上一次的神魔大战而湮没，我担心接下来发生的事情会让三界更加震惊……我们的确需要有驾驭它的人……而你，你是否有支配它的力量……")
	elseif( id == 30877)then
	    Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(30877,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        在你将它呈来之前，我几乎认定它们已经跟随上一次的神魔大战而湮没，我担心接下来发生的事情会让三界更加震惊……我们的确需要有驾驭它的人……而你，你是否有支配它的力量……")
	end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 40057)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40057,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        如何使用灵魂净化是我要给你上的第一堂课。这本书上记录了灵魂净化的使用方法，你学会之后我还有别的东西要教给你。")
	elseif ( id == 40058)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40058,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你干得很好。作为奖励，这200点信仰和这本凝聚的技能书就送给你吧。")
	elseif ( id == 40059)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40059,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        干得不错，我为你感到骄傲，我的孩子！")
	elseif ( id == 40060)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40060,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        为了奖励你，我将会给你一个新的领域——旅行！")
	elseif ( id == 40061)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40061,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你果然没有让我失望！")
	elseif ( id == 40063)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40063,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        很好，这些是给你的奖励！")
	elseif ( id == 40064)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40064,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你果然没有让我失望！")
	elseif ( id == 40065)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40065,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你让我想起了很久以前我率领军队征战三界的那段日子……")
	elseif ( id >= 30350 and id <= 30365)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(id,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        好好研习一遍，许多精妙的方法需要融会贯通。")
	elseif ( id == 30800)then
			if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30800,QuestTalkBegin+1,"确定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30800,QuestTalkBegin+2,"返回" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        欢迎你回来，我的勇士。神界的召唤，让大家看看你有多神。");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.UpdateQuest(30800);
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		end
	elseif ( id == 30874)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30874,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你又让我增强了一份信心，好好利用这颗水晶，想要成为一位真正的强者，还要走很长的一段路，我对你寄予厚望……")
	elseif ( id == 30875)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30875,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你又让我增强了一份信心，好好利用这颗水晶，想要成为一位真正的强者，还要走很长的一段路，我对你寄予厚望……")
	elseif ( id == 30876)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30876,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你又让我增强了一份信心，好好利用这颗水晶，想要成为一位真正的强者，还要走很长的一段路，我对你寄予厚望……")
	elseif ( id == 30877)then
	    Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30877,"<font=龙字体,0,19,20><color=0xfffcf157>三界主宰艾辛萨：<color=0xffffffff>\\n        你又让我增强了一份信心，好好利用这颗水晶，想要成为一位真正的强者，还要走很长的一段路，我对你寄予厚望……")
	end
end

God = {}
God["OnTalk"] = OnTalk
God["OnAcceptQuestTalk"] = OnAcceptQuestTalk
God["OnCompleteQuestTalk"] = OnCompleteQuestTalk