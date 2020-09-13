
local function OnTalk()
	Quest.ClearNPCTalkOptions();  --清除对话框选项
	Quest.SetAcceptOptions(5010161);
	Quest.SetCompleteOptions(5010161);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n 三鬼教还真是麻烦啊。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	 if( id == 20416)then
	  Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20416,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n我以为巴泽尔说的并不是完全正确，这三鬼教是个十分巨大的组织，我们消灭的只是一小部分而已吧。既然这样，我们还要更加努力铲除他们。");
	 elseif( id == 20417)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20417,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n在三鬼教之中，还有头衔为五鬼的敌人，他们是在三鬼教中等级较低的一部分，我探到这附近有五鬼的人在行动，我希望可以从他们身上找到关于三鬼教更多的信息。");
	 elseif( id == 20418)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20418,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n三鬼教，有个精英干部叫做无影，没有影子的人应该就是魔族吧？总之我们要消灭掉他们。");
	 elseif( id == 20419)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20419,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n三鬼教如此强大，我们需要增加自己的实力，当然你也要继续锻炼自己，然后你可以去找辛西娅，我想她会很高兴见到你的。");
	 end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	 if( id == 20415)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20415,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n圣印，这肯定是拉米萨教给你的吧，我就知道。");
	 elseif( id == 20416)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20416,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n一定要消灭这个三鬼教。");
	 elseif( id == 20417)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20417,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n让我看看这五鬼令，到底是有何用处。");
	 elseif( id == 20418)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20418,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n我们要继续努力，这点成绩还是远远不够的呢。");
	 elseif( id == 30342)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30342,"<font=龙字体,0,19,20><color=0xfffcf157>科雷特：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");

	 end

end

Rkeleite = {}
Rkeleite["OnTalk"] = OnTalk
Rkeleite["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rkeleite["OnCompleteQuestTalk"] = OnCompleteQuestTalk