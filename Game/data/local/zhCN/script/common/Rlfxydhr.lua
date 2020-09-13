--神界储备官

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610015);
	Quest.SetCompleteOptions(5610015);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>老服信仰兑换人：<color=0xffffffff>\\n只要你能提供我想要的东西，我就给你信仰。\\n<T>该任务只有神格在10级以下(包括10级)的玩家才能接取，每天最多完成20次，任务给予的信仰不会计算在每天信仰上限的2000点中。</T>");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30307)then
  	--  Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30307,"<font=龙字体,0,19,20><color=0xfffcf157>老服信仰兑换人:<color=0xffffffff>\\n请为我收集资财，我将会给予你100点信仰作为回报");
	end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30307)then
  	--  Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30307,"<font=龙字体,0,19,20><color=0xfffcf157>老服信仰兑换人:<color=0xffffffff>\\n你做的很好!众神以你为傲!");
	end
end

Rlfxydhr = {}
Rlfxydhr["OnTalk"] = OnTalk
Rlfxydhr["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rlfxydhr["OnCompleteQuestTalk"] = OnCompleteQuestTalk
