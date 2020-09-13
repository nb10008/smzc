
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510626);
	Quest.SetCompleteOptions(5510626);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>年老的妇人<color=0xffffffff>\\n哎，我老伴被白银魔窟的歹人抓走了，不知道生死如何啊。");

end





oldwoman = {}
oldwoman["OnTalk"] = OnTalk
