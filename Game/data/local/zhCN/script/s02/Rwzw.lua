local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600017);
	Quest.SetCompleteOptions(5600017);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>火腿王中王：<color=0xffffffff><font=龙字体,0,17,20>\\n我是火腿王者中的王者。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30681)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30681,"<font=龙字体,0,19,20><color=0xfffcf157>火腿王中王：<color=0xffffffff><font=龙字体,0,17,20>\\n你知道吗，新年的主菜是什么？是火腿，不是火鸡！我恨那些火鸡，每年都来跟我们抢风头，只有火腿才是新年大餐的王者，找到那些火鸡，让它们永远消失！");
        elseif( id == 30682)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30682,"<font=龙字体,0,19,20><color=0xfffcf157>火腿王中王：<color=0xffffffff><font=龙字体,0,17,20>\\n你知道吗，新年的主菜是什么？是火腿，不是火鸡！我恨那些火鸡，每年都来跟我们抢风头，只有火腿才是新年大餐的王者，找到那些火鸡，让它们永远消失！");
        elseif( id == 30683)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30683,"<font=龙字体,0,19,20><color=0xfffcf157>火腿王中王：<color=0xffffffff><font=龙字体,0,17,20>\\n你知道吗，新年的主菜是什么？是火腿，不是火鸡！我恨那些火鸡，每年都来跟我们抢风头，只有火腿才是新年大餐的王者，找到那些火鸡，让它们永远消失！");
	elseif( id == 30684)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30684,"<font=龙字体,0,19,20><color=0xfffcf157>火腿王中王：<color=0xffffffff><font=龙字体,0,17,20>\\n你知道吗，新年的主菜是什么？是火腿，不是火鸡！我恨那些火鸡，每年都来跟我们抢风头，只有火腿才是新年大餐的王者，找到那些火鸡，让它们永远消失！");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30681)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30681,"<font=龙字体,0,19,20><color=0xfffcf157>火腿王中王：<color=0xffffffff><font=龙字体,0,17,20>\\n请你记住，在新年大餐中，火腿才是王道。");
        elseif( id == 30682)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30682,"<font=龙字体,0,19,20><color=0xfffcf157>火腿王中王：<color=0xffffffff><font=龙字体,0,17,20>\\n请你记住，在新年大餐中，火腿才是王道。");
	elseif( id == 30683)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30683,"<font=龙字体,0,19,20><color=0xfffcf157>火腿王中王：<color=0xffffffff><font=龙字体,0,17,20>\\n请你记住，在新年大餐中，火腿才是王道。");
	 elseif( id == 30684)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30684,"<font=龙字体,0,19,20><color=0xfffcf157>火腿王中王：<color=0xffffffff><font=龙字体,0,17,20>\\n请你记住，在新年大餐中，火腿才是王道。");
      
       end

end

Rwzw = {}
Rwzw["OnTalk"] = OnTalk
Rwzw["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rwzw["OnCompleteQuestTalk"] = OnCompleteQuestTalk