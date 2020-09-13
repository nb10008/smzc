
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510624);
	Quest.SetCompleteOptions(5510624);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>青年<color=0xffffffff>\\n我想成为一名英勇的战士，像元冷弟兄一样，保卫我们的镇子。");

end





younger = {}
younger["OnTalk"] = OnTalk
