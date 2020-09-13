
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010080);
	Quest.SetCompleteOptions(5010080);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n这些魔族，真是越来越猖狂了。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20215)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20215,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n       你也应该看到了吧，这山谷里全是被魔族影响的石元素，我要把他们的<I>元素之心</I>拿回来，不再受折磨。");
        elseif( id == 20216)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20216,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n        我们还要去一趟<N>岩祖洞</N>，那里的魔岩已经被魔族完全控制了，如果它们从洞中冲出来，那就不好了。");
        elseif( id == 20217)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20217,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n        那个恶魔，又叫做<color=0xffff0000>熔岩领主<color=0xffffffff>，这一切都该做个了断了。集结你的所有力量，一定要消灭它。");
        elseif( id == 20218)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20218,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n        等你锻炼到<C>30</C>级，就回<N>斗战天城</N>去找<D>斗神之王玛斯</D>吧，他应该会非常满意你的表现。");
        elseif( id == 20223)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20223,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n        即使消灭了<A>熔岩领主</A>,还是有很多部下的存在，去消灭它们吧。还有收集一些石头元素，我要知道它们目前的状况。");
        
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20214)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20214,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n        难道你是来帮忙的？");
        elseif( id == 20215)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20215,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n        这么纯洁的东西，却被魔族侵蚀了，真是可恶");
        elseif( id == 20216)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20216,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n        是时候找出真凶了。");
        elseif( id == 20217)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20217,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n        一切都结束了，我要好好的休息一下，你的试炼也完成了。准备回到斗战天城去吧。");
	elseif( id == 20223)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20223,"<font=龙字体,0,19,20><color=0xfffcf157>火神加尔：<color=0xffffffff>\\n        这样才是真的结束了，我终于可以放心的休息了。");
       end

end

Rjiaer = {}
Rjiaer["OnTalk"] = OnTalk
Rjiaer["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rjiaer["OnCompleteQuestTalk"] = OnCompleteQuestTalk