
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>交易所指引人<color=0xffffffff>\\n    最近前来交易的人越来越多，现有的交易区域已经容不下这么多的人一起摆摊交易了。为此，我们在离这里较远的地方开辟了一片新的交易区域，通过我旁边的这个传送点，你可以很快捷地到达那里。");

end

Rjiaoyisuojieshaoren = {}
Rjiaoyisuojieshaoren["OnTalk"] = OnTalk
