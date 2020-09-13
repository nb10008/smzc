
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510620);
	Quest.SetCompleteOptions(5510620);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>卢卡<color=0xffffffff>\\n        艾琳娜和伊莲娜两姐妹都很漂亮，姐姐温柔，妹妹彪悍，也代表了福临镇和天雪镇的不同民风。");

end





luca = {}
luca["OnTalk"] = OnTalk
