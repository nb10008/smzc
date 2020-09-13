
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010030);
	Quest.SetCompleteOptions(5010030);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>迪妮莎：<color=0xffffffff>\\n恭喜你战胜了魔族长老！现在你要面对的是一个小小的难题，它将考验你的智慧！\\n 你看到眼前的机关了吗？你用键盘“a s d w”键操作，当你走入地上的方格后，你所站的方格与周围与之相邻的方格的状态会发生变化，原来点燃的火焰会熄灭，原来熄灭的会重新燃烧。你必须把所有的火焰全都踩灭，通往大长老的房间的大门才会打开，你将挑战他会获得丰厚的奖励。不要小看这个东西哦，它可不简单！");

end




Rdinishae = {}
Rdinishae["OnTalk"] = OnTalk
