
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010102);
	Quest.SetCompleteOptions(5010102);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n天神来啦！哈哈！我们有救啦！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20411)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20411,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n    这里有四个封印，就在武岩城内，这封印必须消灭掉，否则人类是无法进入城内的，除了 你这样的天神。");
        elseif( id == 20412)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20412,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n    将幻林红速龙和幻林绿速龙全部除掉，然后回来。");
        elseif( id == 20413)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20413,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n    这里有个很危险的邪教干部，被称为痛苦之王，因为他最喜爱为别人制造痛苦，我们需要消灭这个危险的家伙。");
        elseif( id == 20414)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20414,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n    工神鲁斯说希望你在<C>60</C>级的时候可以去找他一趟，好像是说关于圣印的一些事情，你还是直接去斗战天城问他吧。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20410)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20410,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n       这里面有些危险，都是邪教的人。");
        elseif( id == 20411)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20411,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n       太好了，这样我们就可以夺回这个武岩城了。");
        elseif( id == 20412)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20412,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n       厉害一个人就将这里的速龙都解决了。");
        elseif( id == 20413)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20413,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n       希望三鬼教不要再来这里了。");
        elseif( id == 30341)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30341,"<font=龙字体,0,19,20><color=0xfffcf157>巴泽尔：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");

        end

end

Rbazer = {}
Rbazer["OnTalk"] = OnTalk
Rbazer["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rbazer["OnCompleteQuestTalk"] = OnCompleteQuestTalk