
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetCompleteOptions(5510503);
	Quest.NPCTalk(0,0,"魔窟嘉奖人：每天的9:00、13:00、18:00、23:00都会发生魔窟之门降临的事件，请大家在以上时间内到魔窟中击败魔窟首领，然后到我这领取奖励。");

end



--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30301)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30301,"魔窟嘉奖人：这是你应得的奖励！");
    end

end

RMoKuJiaJiangRen = {}
RMoKuJiaJiangRen["OnTalk"] = OnTalk
RMoKuJiaJiangRen["OnCompleteQuestTalk"] = OnCompleteQuestTalk
