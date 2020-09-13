
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010167);
	Quest.SetCompleteOptions(5010167);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n魔族才是人类应该效忠的对象。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20513)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20513,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n洞中的宝贝就是那些石魔身上的<I>原魔石</I>，能找来越多越好。小心游荡在石魔中间的<color=0xffff6000>南蛮精英勇士<color=0xffffffff>！");
        elseif( id == 20514)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20514,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n聪明的家伙！你竟然猜到<N>石狼洞</N>中的<A>狼人</A>和<A>狼妖</A>身上也有宝贝！那就去吧！多多益善！不过要先清除<color=0xffff6000>密林狂狼<color=0xffffffff>和他忠实的<color=0xffff6000>沼泽巨魔熊<color=0xffffffff>！");
        elseif( id == 20515)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20515,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n刚刚有人来报，<color=0xffff0000>武者伐天<color=0xffffffff>、<color=0xffff0000>九天石魔<color=0xffffffff>、<color=0xffff0000>风影<color=0xffffffff>，这三个还算老实的家伙竟敢偷袭我魔族百姓。也不知是何人为他们撑腰！交给你了！杀！");
	elseif( id == 20516)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20516,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n七十二魔子中的<D>左德</D>大人会告诉你<I>原魔石</I>和<I>妖核</I>的用途，不过以你的实力，嘿嘿！相信左德大人是不会理睬你的！<C>76</C>级！是的，至少要这样！");
	
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20512)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20512,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n居然真是个天神，虽然魔纳尔有说过，我还是不能相信。");
        elseif( id == 20513)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20513,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n这么好的本事，当天神可惜了，怎样？有没有兴趣加入魔族？");
	elseif( id == 20514)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20514,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n想知道这两种宝贝的用途，你还要再去趟洞中。");
	elseif( id == 20515)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20515,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n这个标志……哼！很好，你又猜对了，三个都是原神教的走狗！！");
   elseif( id == 30344)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30344,"<font=龙字体,0,19,20><color=0xfffcf157>魔纳特：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");
      
       end

end

RmonaTe = {}
RmonaTe["OnTalk"] = OnTalk
RmonaTe["OnAcceptQuestTalk"] = OnAcceptQuestTalk
RmonaTe["OnCompleteQuestTalk"] = OnCompleteQuestTalk