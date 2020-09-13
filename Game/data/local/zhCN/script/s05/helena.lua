
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510625);
	Quest.SetCompleteOptions(5510625);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>伊莲娜<color=0xffffffff>\\n 看什么看，没见过美女啊");

end





helena = {}
helena["OnTalk"] = OnTalk
