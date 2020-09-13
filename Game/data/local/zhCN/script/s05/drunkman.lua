
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510623);
	Quest.SetCompleteOptions(5510623);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神秘的醉翁<color=0xffffffff>\\n要不要来壶酒啊，福临镇的酒可是最美味的了，来去很方便的");

end





drunkman = {}
drunkman["OnTalk"] = OnTalk
