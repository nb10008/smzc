
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010166);
	Quest.SetCompleteOptions(5010166);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳尔：<color=0xffffffff>\\n魔族的荣耀不容侵犯！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20509)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20509,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳尔：<color=0xffffffff>\\n你是说，你有一些问题要来这里询问或是证明么？我们还不能相信你，除非你先证明你的诚实，比如将那些天界派来的暗杀者……");
        elseif( id == 20510)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20510,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳尔：<color=0xffffffff>\\n你看看那些<A>蛇妖</A>，已经没有利用价值了，因为它们被什么给污染了。等你做完了之后，我可以告诉你是什么污染了它们。");
        elseif( id == 20511)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20511,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳尔：<color=0xffffffff>\\n想知道更多的话，还是等等吧。我现在有个工作必去清理沼泽里该死的<A>鳄鱼</A>什么的。");
	elseif( id == 20512)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20512,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳尔：<color=0xffffffff>\\n这里有一个叫做<N>石狼洞</N>的地方，里面有很珍贵的东西，可那里的土著十分不希望我们得到那些宝贝。等你等级到达<C>72</C>级的时候，就去洞口找<D>魔纳特</D>吧。");
	
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20508)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20508,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳尔：<color=0xffffffff>\\n一个天神？竟敢来到这里，看来你有必死的觉悟了！");
        elseif( id == 20509)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20509,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳尔：<color=0xffffffff>\\n你还真是不错啊，为了目的，天界派来的杀手你都敢出手。");
	elseif( id == 20510)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20510,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳尔：<color=0xffffffff>\\n看来你确实想知道什么，可是这对你没有好处，因为你是在为真正的灾难服务。");
	elseif( id == 20511)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20511,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳尔：<color=0xffffffff>\\n还算不错吧。");
       
       end

end

Rmonaer = {}
Rmonaer["OnTalk"] = OnTalk
Rmonaer["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rmonaer["OnCompleteQuestTalk"] = OnCompleteQuestTalk