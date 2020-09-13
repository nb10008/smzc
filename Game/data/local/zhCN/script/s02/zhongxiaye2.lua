
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(6011057);
	Quest.SetCompleteOptions(6011057);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>仲夏夜祈福使者：<color=0xffffffff><font=龙字体,0,17,20>\\n在这仲夏时分，在漫天星斗的夜晚，让我们进行狂欢。     \n不要忘记仲夏夜祈福活动使者仅仅在7月17到30日出现。");

end



--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30798)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30798,"<font=龙字体,0,19,20><color=0xfffcf157>仲夏夜祈福使者：<color=0xffffffff><font=龙字体,0,17,20>\\n拿好我送你的礼物，记得在活动时间内多多领取。");
      
       end


end

zhongxiaye2 = {}
zhongxiaye2["OnTalk"] = OnTalk
zhongxiaye2["OnAcceptQuestTalk"] = OnAcceptQuestTalk
zhongxiaye2["OnCompleteQuestTalk"] = OnCompleteQuestTalk