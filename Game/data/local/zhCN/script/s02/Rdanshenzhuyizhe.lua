
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600079);
	Quest.SetCompleteOptions(5600079);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>单身主义者：<color=0xffffffff>\\n    我最喜欢的节日终于到了，单身的朋友们，赶紧庆祝我们的节日吧！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30843)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30843,"<font=龙字体,0,19,20><color=0xfffcf157>单身主义者：<color=0xffffffff>\\n    你有没有暗恋的人呢，还因为犹豫而不敢表白吗？如果实在没有勇气直接表白，可以用我的匿名情书试一下，放心吧，对方不会知道是谁送的！");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30843)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30843,"<font=龙字体,0,19,20><color=0xfffcf157>单身主义者：<color=0xffffffff>\\n    快去把情书送给你喜欢的人吧，也许明年你就不用过这个节日了！");
       end

end

Rdanshenzhuyizhe = {}
Rdanshenzhuyizhe["OnTalk"] = OnTalk
Rdanshenzhuyizhe["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rdanshenzhuyizhe["OnCompleteQuestTalk"] = OnCompleteQuestTalk