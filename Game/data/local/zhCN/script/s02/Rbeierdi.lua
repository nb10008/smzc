
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610094);
	Quest.SetCompleteOptions(5610094);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>贝尔蒂：<color=0xffffffff>\\n只要心中充满爱，你的世界随处可以发现幸福！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30905)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30905,"<font=龙字体,0,19,20><color=0xfffcf157>贝尔蒂：<color=0xffffffff>\\n开启你的爱之光，将幸福传递下去！");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30905)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30905,"<font=龙字体,0,19,20><color=0xfffcf157>贝尔蒂：<color=0xffffffff>\\n幸福已经传递下去了，你的爱将会使你也拥有幸福！");

       end

end

Rbeierdi = {}
Rbeierdi["OnTalk"] = OnTalk
Rbeierdi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rbeierdi["OnCompleteQuestTalk"] = OnCompleteQuestTalk
