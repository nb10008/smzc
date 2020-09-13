
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010021);
	Quest.SetCompleteOptions(5010021);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>斗神希拉：<color=0xffffffff><font=龙字体,0,17,20>\\n这到底是怎么回事啊，怎么这里有那么多怪物啊！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40000)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40000,"<font=龙字体,0,19,20><color=0xfffcf157>斗神希拉：<color=0xffffffff><font=龙字体,0,17,20>\\n你是否注意到在屏幕左上自身属性部分，除了体力、真气外，<color=0xff9933ff>紫色槽<color=0xffffffff>表示是你的<color=0xffff00ff>怒气<color=0xffffffff>。每次命中怪物都可以积攒怒气值。当怒气值满以后，你可以使用<color=0xff80ff00>天神下凡技能<color=0xffffffff>，进入天神下凡状态，你的属性在一定时间内会大幅度提高。\\n \\n接下来你便去尝试使用<C>1</C>次<color=0xff80ff00>天神下凡技能<color=0xffffffff>，并击杀<C>8</C>只<A>灵蝶</A>吧！");
        elseif( id == 20105)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20105,"<font=龙字体,0,19,20><color=0xfffcf157>斗神希拉：<color=0xffffffff>\\n你才我刚才看到了什么，是可怕的野人。跟我以前在这里见过的野人很不一样，眼神凶恶，还好没被他们发现。我想知道这些野人身上发生了什么事，帮我采集一点“样本”来吧。有时候这是必须要做的，将这些交给艾丽娜大人就好。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40000)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40000,"<font=龙字体,0,19,20><color=0xfffcf157>斗神希拉：<color=0xffffffff><font=龙字体,0,17,20>\\n干得不错！看来你已经熟练掌握了<color=0xff80ff00>天神下凡技能<color=0xffffffff>。\\n记住，天神下凡可以使你变得十分强大，但是技能恢复时间较长，所以在危机时刻或者是与别人争斗时使用效果最佳。\\n    这里有几瓶<I>试用怒气药水</I>，使用后可恢复一定的怒气，请珍惜使用。");
        elseif( id == 20104)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20104,"<font=龙字体,0,19,20><color=0xfffcf157>斗神希拉：<color=0xffffffff><font=龙字体,0,17,20>\\n我们好像遇到了更大的麻烦。");
       
     
       end


end

Rxila = {}
Rxila["OnTalk"] = OnTalk
Rxila["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxila["OnCompleteQuestTalk"] = OnCompleteQuestTalk