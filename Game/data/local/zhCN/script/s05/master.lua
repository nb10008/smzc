
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510621);
	Quest.SetCompleteOptions(5510621);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>福临镇镇长<color=0xffffffff>\\n北州天雪有两股邪恶势力，独霸一方的奥德和自称神使的普罗迪。他们常常来欺压我的子民，抓走了很多无辜平民，看看这些天真可爱的孩子，请帮助我们吧。");

end





master = {}
master["OnTalk"] = OnTalk
