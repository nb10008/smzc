
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010197);
	Quest.SetCompleteOptions(5010197);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>蘑菇收集人：<color=0xffffffff><font=龙字体,0,17,20>\\n七花之海里的魔物，在击杀后会掉落各种蘑菇，收集到这些蘑菇后，可以在我这里兑换各种不同类型的蘑菇");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 50001)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50001,"<font=龙字体,0,19,20><color=0xfffcf157>蘑菇收集人：<color=0xffffffff><font=龙字体,0,17,20>\\n只要你拥有<C>2</C>个<I>小人菇</I>，我便可以将<C>1</C>个<I>巨人菇</I>给你，如何？");
        elseif( id == 50002)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50002,"<font=龙字体,0,19,20><color=0xfffcf157>蘑菇收集人：<color=0xffffffff><font=龙字体,0,17,20>\\n只要你拥有<C>2</C>个<I>巨人菇</I>，我便可以将<C>1</C>个<I>忧郁菇</I>给你，如何？");
        elseif( id == 50003)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50003,"<font=龙字体,0,19,20><color=0xfffcf157>蘑菇收集人：<color=0xffffffff><font=龙字体,0,17,20>\\n只要你拥有<C>2</C>个<I>忧郁菇</I>，我便可以将<C>1</C>个<I>生命菇</I>给你，如何？");
	elseif( id == 50004)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50004,"<font=龙字体,0,19,20><color=0xfffcf157>蘑菇收集人：<color=0xffffffff><font=龙字体,0,17,20>\\n只要你拥有<C>2</C>个<I>生命菇</I>，我便可以将<C>1</C>个<I>毁灭菇</I>给你，如何？");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 50001)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50001,"<font=龙字体,0,19,20><color=0xfffcf157>蘑菇收集人：<color=0xffffffff><font=龙字体,0,17,20>\\n巨人菇可以让你变大~变大~再变大~！");
        elseif( id == 50002)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50002,"<font=龙字体,0,19,20><color=0xfffcf157>蘑菇收集人：<color=0xffffffff><font=龙字体,0,17,20>\\n忧郁菇吃了可以恢复真气！");
        elseif( id == 50003)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50003,"<font=龙字体,0,19,20><color=0xfffcf157>蘑菇收集人：<color=0xffffffff><font=龙字体,0,17,20>\\n生命菇吃了可以恢复体力。");
        elseif( id == 50004)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50004,"<font=龙字体,0,19,20><color=0xfffcf157>蘑菇收集人：<color=0xffffffff><font=龙字体,0,17,20>\\n毁灭菇，你用了就知道效果了！另外，若拥有3个毁灭菇，你可以去找那边的<D>僵尸博士</D>，他正在收集这东西！");
       end

end

Rmogushoujiren = {}
Rmogushoujiren["OnTalk"] = OnTalk
Rmogushoujiren["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rmogushoujiren["OnCompleteQuestTalk"] = OnCompleteQuestTalk