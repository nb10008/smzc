local function OnTalk()
	Quest.ClearNPCTalkOptions();
	Quest.SetAcceptOptions(5610095);
	Quest.SetCompleteOptions(5610095);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff>\\n        情人节到了，你准备好了吗？");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30912)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30912,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n带着我给你的卡片，去寻找你的情人吧！");
        elseif( id == 30917)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30917,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n通过塑形<M>沙雕</M>，会获得情人节道具<I>黄金贝</I>，<I>黄金贝</I>可以在<D>情人节商人</D>处兑换各种道具。不过，想要塑形<M>沙雕</M>，就需要携带这个<I>魔法水滴</I>。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30917)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30917,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n<I>魔法水滴</I>每天可以领取<S>120</S>个，快去和心爱的人一起塑形<M>沙雕</M>吧！");
        elseif( id == 30916)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30916,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
        elseif( id == 30915)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30915,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
        elseif( id == 30914)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30914,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
        elseif( id == 30913)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30913,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
        elseif( id == 30912)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30912,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n找到你的情人了吧，不过情人节还没结束，抓紧时间多找几个吧！");
        elseif( id == 30911)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30911,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
        elseif( id == 30910)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30910,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
        elseif( id == 30909)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30909,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
        elseif( id == 30908)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30908,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
        elseif( id == 30907)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30907,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
        elseif( id == 30906)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30906,"<font=龙字体,0,19,20><color=0xfffcf157>情人节天使：<color=0xffffffff><font=龙字体,0,17,20>\\n赶紧去找你的情人吧！");
       end


end
Rqingrenjietianshi = {}
Rqingrenjietianshi["OnTalk"] = OnTalk
Rqingrenjietianshi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rqingrenjietianshi["OnCompleteQuestTalk"] = OnCompleteQuestTalk
