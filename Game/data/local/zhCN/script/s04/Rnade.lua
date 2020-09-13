
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010077);
	Quest.SetCompleteOptions(5010077);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>纳德：<color=0xffffffff>\\n别老是限制我发挥，我自己可以解决这些问题，可我的老师总是不让做这个，不让做那个。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20204)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20204,"<font=龙字体,0,19,20><color=0xfffcf157>纳德：<color=0xffffffff>\\n         这里的元素和你在七花之海看到的野人有些相似，它们都被某种力量影响了。没错，也许就是魔族干的，我需要那些石元素的样本。");
        elseif( id == 20205)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20205,"<font=龙字体,0,19,20><color=0xfffcf157>纳德：<color=0xffffffff>\\n         我记得附近有一个巨大的石元素，叫做<color=0xffff6000>布多巨岩<color=0xffffffff>。我很想知道在它身上有没有发生什么，你懂的，我需要样本。然后将这样本交给我的老师吧。");
	elseif( id == 20220)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20220,"<font=龙字体,0,19,20><color=0xfffcf157>纳德：<color=0xffffffff>\\n         快去告诉其他的人这里的情况，让大家快来支援这里，让裂谷得以安宁祥和。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20203)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20203,"<font=龙字体,0,19,20><color=0xfffcf157>纳德：<color=0xffffffff>\\n        难道是我的老师让你来找我的，这可麻烦了。不过我也是出来办正事的。");
        elseif( id == 20204)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20204,"<font=龙字体,0,19,20><color=0xfffcf157>纳德：<color=0xffffffff>\\n        这些还是要交给我的老师好好看看。");
	elseif( id == 20220)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20220,"<font=龙字体,0,19,20><color=0xfffcf157>纳德：<color=0xffffffff>\\n        恩~看来这里马上就会有人来支援了。");
       end

end

Rnade= {}
Rnade["OnTalk"] = OnTalk
Rnade["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rnade["OnCompleteQuestTalk"] = OnCompleteQuestTalk