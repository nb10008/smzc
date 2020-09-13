
local function OnTalk()
	Quest.ClearNPCTalkOptions()--清除对话框选项
	Quest.SetAcceptOptions(5510401)
	Quest.SetCompleteOptions(5510401)
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>女神的侍从：\\n<color=0xffffffff><font=龙字体,0,17,20>我是女神的侍从，每周二、五的【15：00】、【20：00】我会出现在这里，希望诸位斗神能够帮我收集女神所遗落的物品，若可完成，我将给予丰厚的奖励。\\n    整个过程很容易，只需要收集指定数量的物品即可，完成后除可获得大量经验与大量金钱奖励外，若你的等级≥40级，还可以获得“天神勋章”作为奖励。“天神勋章”可在天神勋章兑换人处兑换个人需要的各种道具。\\n    另外需要提醒你，每次一人只能在我这里完成1次任务，若是此期间你的任务没能及时交付，则需等下次出现时再来交付，不过也意味着浪费了一次任务机会，交付后暂时就不能再领取新的任务了。\\n    还有，每次活动期间，第1名、第10名、第100名完成任务之人，可得到一些额外奖励，你准备好了吗？");
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	for i = 1,17 do
		if( id == 30202 + i)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30202 + i ,"<font=龙字体,0,19,20><color=0xffffe289>女神的侍从：\\n    <color=0xffffffff><font=龙字体,0,17,20>如此一来，我可以慢慢将女神之像复原了！很感谢你！")
		end
	end
end

local function OnAcceptQuestTalk(id,step)
	for i = 1,17 do
		if( id == 30202 + i)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(30202 + i ,"<font=龙字体,0,19,20><color=0xffffe289>女神的侍从：\\n    <color=0xffffffff><font=龙字体,0,17,20>自上次天魔大战，斗战天城内的女神之像被魔族摧毁，散落四方，如今的女神像乃是仿造。作为侍从，我一直以来希望能够将神像碎片找回。\\n    此番所要寻找的神像碎片种类繁多且数量巨大，非是单人可以完成，所以我会根据你的等级来决定让你帮我收集什么，当然，不同收集任务对应的奖励也是不同的，越高自然越是丰厚。\\n    你可准备好接取任务，帮我神像碎片了吗？")
		end
	end
end

RNvShenShiCong = {}
RNvShenShiCong["OnTalk"] = OnTalk
RNvShenShiCong["OnCompleteQuestTalk"] = OnCompleteQuestTalk
RNvShenShiCong["OnAcceptQuestTalk"] = OnAcceptQuestTalk
