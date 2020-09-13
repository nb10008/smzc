
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510619);
	Quest.SetCompleteOptions(5510619);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>艾琳娜<color=0xffffffff>\\n欢迎来到福临村，这里的民风淳朴，我们热情迎接善意的客人。");

end





arlene = {}
arlene["OnTalk"] = OnTalk
