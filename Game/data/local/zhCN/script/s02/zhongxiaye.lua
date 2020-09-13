
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(6011056);
	Quest.SetCompleteOptions(6011056);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>仲夏夜祈福活动使者：<color=0xffffffff><font=龙字体,0,17,20>\\n在这仲夏时分，在漫天星斗的夜晚，让我们进行狂欢。     \n仲夏夜祈福任务需要50级才能接取。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30798)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30798,"<font=龙字体,0,19,20><color=0xfffcf157>仲夏夜祈福活动使者：<color=0xffffffff><font=龙字体,0,17,20>\\n仲夏的夜晚，狂欢的时节，就是送出礼物的时候。要去仲夏夜祈福使者那里交付任务。");
       
        end
end



zhongxiaye = {}
zhongxiaye["OnTalk"] = OnTalk
zhongxiaye["OnAcceptQuestTalk"] = OnAcceptQuestTalk
zhongxiaye["OnCompleteQuestTalk"] = OnCompleteQuestTalk