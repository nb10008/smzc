
local function OnTalk()
	Quest.ClearNPCTalkOptions()--清除对话框选项
	Quest.SetCompleteOptions(5510203);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>百鬼炼狱嘉奖人：\\n    <color=0xffffffff><font=龙字体,0,17,20>每次只有6位勇士可以获得我给的奖励。");
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	for i = 1,12	do
		if( id == 30100 + i)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30100 + i ,"<font=龙字体,0,19,20><color=0xffffe289>百鬼炼狱嘉奖人：\\n    <color=0xffffffff><font=龙字体,0,17,20>最近能通过百鬼炼狱的人越来越多了，实在鼓舞人心啊。")
		end
	end
end

RBaiGuiJiaJiang = {}
RBaiGuiJiaJiang["OnTalk"] = OnTalk
RBaiGuiJiaJiang["OnCompleteQuestTalk"] = OnCompleteQuestTalk
