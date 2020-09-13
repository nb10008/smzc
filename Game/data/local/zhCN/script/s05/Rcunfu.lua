
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510634);
	Quest.SetCompleteOptions(5510634);
	talk_ID=math.random(1,5)
	if talk_ID==1 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>饥寒交迫的平民<color=0xffffffff>\\n        我的亲人都被抓走了，怎么办啊……帮帮我吧");
	elseif talk_ID==2 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>福临镇的平民<color=0xffffffff>\\n          这里的贸易很繁华，各种货商分布在天雪镇、福临镇、北风岗等地，你可以买到任何你想买的东西。");
	elseif talk_ID==3 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>饥寒交迫的平民<color=0xffffffff>\\n        想买药吗？去找药剂商吧，就在不远。");
	elseif talk_ID==4 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>饥寒交迫的平民<color=0xffffffff>\\n        装备强化师可以帮你强化装备，听说天雪镇好像有。");
	else
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>饥寒交迫的平民<color=0xffffffff>\\n        你可以去看看悬赏榜，有能力的话帮助我们铲除周围的歹人可以获得丰厚的奖励呢！");

	end
end






Rcunfu = {}
Rcunfu["OnTalk"] = OnTalk
