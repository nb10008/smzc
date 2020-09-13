
local function OnTalk()
	Quest.ClearNPCTalkOptions()--清除对话框选项
	Quest.SetAcceptOptions(5510403)
	Quest.SetCompleteOptions(5510403)
	Quest.NPCTalk(0,0,"<font=龙字体,0,18,20><color=0xffffe289>猎魔使者：\\n<color=0xffffffff><font=龙字体,0,16,20>我奉命缉拿三界中的各大魔神，每周三、六的【15：00】与【20：00】会在这里出现，发布“魔神的咆哮”任务，希望诸位能够协助我。\\n    若接受任务，则需要击杀一系列的各种魔神头目，最好是组队前往。若可完成任务，即有大量经验与大量金钱奖励，若你的等级≥40级，同时还能获得天神勋章，在天神勋章兑换人处兑换道具。\\n    活动期间，第1名与第10名、第50名、第100名完成任务之人，还可获得额外奖赏！");
end


local function OnAcceptQuestTalk(id,step)
	if( id == 30201)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30201,"<font=龙字体,0,18,20><color=0xffffe289>猎魔使者：\\n    <color=0xffffffff><font=龙字体,0,16,20>你是否愿意接受猎魔任务？接受任务后你需要找到一系列的魔神头目并且将其击杀，请小心谨慎。\\n    另外，每次活动期间第1名、第10名、第50名、第100名完成任务之人可获得额外奖励。")
	end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30201)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30201,"<font=龙字体,0,18,20><color=0xffffe289>猎魔使者：\\n    <color=0xffffffff><font=龙字体,0,16,20>猎杀魔族越多，对你我就越有好处！")
	end
end
RLieMoShiZhe = {}
RLieMoShiZhe["OnTalk"] = OnTalk
RLieMoShiZhe["OnCompleteQuestTalk"] = OnCompleteQuestTalk
RLieMoShiZhe["OnAcceptQuestTalk"] = OnAcceptQuestTalk
