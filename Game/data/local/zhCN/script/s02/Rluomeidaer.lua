
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010083);
	Quest.SetCompleteOptions(5010083);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n当你获得了装备以后，品质越高越有进行强化的价值.");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40021)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40021,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n这回是需要击杀<C>40</C>只<A>雪地猪</A>，可以交换奖励。");
        elseif( id == 40022)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40022,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n你又来了，还想和我进行交易么？那么你就去击杀<C>1</C>只<A>蛇妖杜莎梅</A>。");
        elseif( id == 40023)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40023,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n新的交换条件是<C>50</C>份<I>初等元神</I>，可以交换奖励。");
        elseif( id == 40024)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40024,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n又到了新的等级，我这里也有新的条件，希望你能替我击杀<C>20</C>只<A>三鬼强力巫师</A>。");
        elseif( id == 40025)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40025,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n<C>50</C>只<A>觅食蛇妖</A>和<C>50</C>只<A>饥饿蛇妖</A>，废话不多说，达到了就来换奖励吧！");
        elseif( id == 40026)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40026,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n这份清单你收下，上面的条件是：<A>魔舞之花</A>×1、<A>千人杀米</A>×1、<A>护法雷芒</A>×1、<A>风斩</A>×1，我想你是明白什么意思的！");
        elseif( id == 40027)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40027,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n这应该是我最后一次与你做交易了，这次希望你能替我击杀<C>500<C>只<A>冥山龙</A>，奖励会非常丰厚！");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40020)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40020,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n我目前只有这些强化物品了。等你<C>30</C>级后可以再来找我换取更多的奖励。");
        elseif( id == 40021)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40021,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n强化的时候消耗还是很大的，所以你一定要注意日常的积累，当然，从其他人那里交易获得也是一个很好的办法。");
        elseif( id == 40022)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40022,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n升级宝石以及日后的升级圣印也会需要强化石，因此它的功用还是很多的！");
        elseif( id == 40023)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40023,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n条件达成，奖励拿走，下一次交换是你在<C>60</C>级的时候。");    
        elseif( id == 40024)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40024,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n若强化失败，装备被损坏是一件很坏的事情，所以一定要使用保底符。");  
        elseif( id == 40025)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40025,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n我觉得对你越来越苛刻了，但是既然你能做到，这些奖励是你该得的。"); 
        elseif( id == 40026)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40026,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff><font=龙字体,0,17,20>\\n看着你一路走来，不断成长不断强大，我也十分欣慰，这些东西能够有助你更进一步。"); 	
        elseif( id == 40027)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40027,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff>\\ns朋友，我们俩缘分已尽，接下来就是靠你自己冲向斗神的颠峰！加油吧！");
        elseif( id == 30868)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30868,QuestTalkBegin+1,"确定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30868,QuestTalkBegin+2,"返回" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>罗梅达亚：<color=0xffffffff>\\n    还是去年的年货表？好吧，我会准时送达的。");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.UpdateQuest(30868);
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		end
	end

end

Rluomeidaer = {}
Rluomeidaer["OnTalk"] = OnTalk
Rluomeidaer["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rluomeidaer["OnCompleteQuestTalk"] = OnCompleteQuestTalk