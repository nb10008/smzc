
local function OnTalk()
	Quest.ClearNPCTalkOptions()--清除对话框选项
	Quest.SetAcceptOptions(5510402)
	Quest.SetCompleteOptions(5510402)
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>天界战神：\\n<color=0xffffffff><font=龙字体,0,17,20>我乃天界72战神之一，每周一的【15：00】、【20：00】和每周四的【15：00】、【20：00】均会在这里出现，发布“战神的荣耀”任务，招募天界战士们清剿魔物，换取奖励。\\n    若你能顺利完成任务，即有大量经验与大量金钱奖励，每次活动只可完成一次任务。\\n    活动期间，第1名、第10名、第100名完成任务之人，还可获得额外奖赏！");
end

local function OnAcceptQuestTalk(id,step)
		if( id == 30202)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(30202,"<font=龙字体,0,19,20><color=0xffffe289>天界战神：\\n    <color=0xffffffff><font=龙字体,0,17,20>魔物横行，你可愿意前往讨伐？这是作为一名战神的荣耀！完成后会获得丰厚的奖励，但是最好和朋友一起组队完成，这样能大大提高效率。\\n    每次活动只可完成一次任务。")
		end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
		if( id == 30202 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30202 ,"<font=龙字体,0,19,20><color=0xffffe289>天界战神：\\n    <color=0xffffffff><font=龙字体,0,17,20>好！这是你应得的奖励！")
		end
end

RTianJieZhanShen = {}
RTianJieZhanShen["OnTalk"] = OnTalk
RTianJieZhanShen["OnCompleteQuestTalk"] = OnCompleteQuestTalk
RTianJieZhanShen["OnAcceptQuestTalk"] = OnAcceptQuestTalk
