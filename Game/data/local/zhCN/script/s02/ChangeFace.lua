local function OnTalk()
	Quest.ClearNPCTalkOptions();
	Quest.SetAcceptOptions(5010247);
	Quest.SetCompleteOptions(5010247);
	Quest.SetNPCTalkOption(14,0,0,"美容");
	Quest.SetNPCTalkOption(14,1,0,"取消");
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>美容师：<color=0xffffffff>\\n        达到30级的玩家可以免费在我这进行一次美容。");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30370)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30370,"<font=龙字体,0,19,20><color=0xfffcf157>美容师：<color=0xffffffff><font=龙字体,0,17,20>\\n有了美容卡才可以进行美容。我这里正好有一张，你拿去用吧。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30370)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30370,"<font=龙字体,0,19,20><color=0xfffcf157>美容师：<color=0xffffffff><font=龙字体,0,17,20>\\n想改变自己容貌的话就来找我吧。");
       end


end
ChangeFace = {}
ChangeFace["OnTalk"] = OnTalk
ChangeFace["OnAcceptQuestTalk"] = OnAcceptQuestTalk
ChangeFace["OnCompleteQuestTalk"] = OnCompleteQuestTalk