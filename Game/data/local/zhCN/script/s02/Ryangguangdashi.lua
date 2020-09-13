local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610089);
	Quest.SetCompleteOptions(5610089);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      阳光养成计划，助您快速体验圣魔之血各特色系统。各中奥秘，请您自行体验。勇士，成龙成虫只在你的一念之间。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30888)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30888,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30889)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30889,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30890)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30890,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30891)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30891,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30892)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30892,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30893)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30893,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30894)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30894,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30895)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30895,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30896)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30896,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30897)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30897,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30898)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30898,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30899)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30899,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30900)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30900,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30901)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30901,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        elseif( id == 30902)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30902,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      让我们开始阳光养成计划吧~！");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30888)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30888,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30889)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30889,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30890)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30890,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30891)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30891,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30892)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30892,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30893)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30893,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30894)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30894,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30895)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30895,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30896)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30896,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30897)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30897,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30898)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30898,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30899)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30899,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30900)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30900,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30901)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30901,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        elseif( id == 30902)then
              Quest.CloseNPCTalk();
	      			Quest.CompleteQuestTalk(30902,"<font=龙字体,0,19,20><color=0xfffcf157>阳光大使：<color=0xffffffff><font=龙字体,0,17,20>\\n      恭喜您，更加丰富多彩的内容在等待你。");
        end
end

Ryangguangdashi = {}
Ryangguangdashi["OnTalk"] = OnTalk
Ryangguangdashi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryangguangdashi["OnCompleteQuestTalk"] = OnCompleteQuestTalk