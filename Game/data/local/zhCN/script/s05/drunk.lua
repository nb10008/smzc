
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510622);
	Quest.SetCompleteOptions(5510622);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神秘的醉翁<color=0xffffffff>\\n啊，你知道吗，天雪镇和福临镇之间有特殊的传送点，福临镇的镇民们每当遇到敌人侵犯的时候都通过传送点到天雪镇躲避。");

end





drunk = {}
drunk["OnTalk"] = OnTalk
