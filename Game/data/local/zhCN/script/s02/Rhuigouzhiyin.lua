
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5513018);
	Quest.SetCompleteOptions(5513018);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        此次回购活动形式多多，奖励多多。");
end

--如果删除任务，可重新手动接取
local function OnAcceptQuestTalk(id,step)
        if( id == 30800)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30800,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        接受神的旨意，成为天界的守护者，万能的主宰之神在召唤你。");
        elseif( id == 30801)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30801,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        你怎么还是一个人走来走去？难道你不感觉到孤单吗？快去组建一个家族吧，结交更多的战友，世界是你们的。");
        elseif( id == 30802)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30802,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        你想拥有复活的技能吗？快去找寻爱侣结婚吧，做你爱人的急救员。");
        elseif( id == 30803)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30803,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        不要忘却你的梦想，现在的你太弱小了。去找修炼使者吧，他会是你的指路明灯。");
        elseif( id == 30804)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30804,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        当您完成一定等级的历练之旅任务后，即可和我身边的那位财神领取丰富的奖励。");
        elseif( id == 30805)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30805,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        历练之旅刚刚开始，完成属于你自己的历练挑战吧！");
        elseif( id == 30806)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30806,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        属于你的历练还在继续，勇敢的尝试下一阶段吧。");
        elseif( id == 30807)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30807,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        属于你的历练还在继续，勇敢的尝试下一阶段吧。");
        elseif( id == 30808)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30808,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        属于你的历练还在继续，勇敢的尝试下一阶段吧。");
        elseif( id == 30809)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30809,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        属于你的历练还在继续，勇敢的尝试下一阶段吧。");
        elseif( id == 30810)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30810,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        属于你的历练还在继续，勇敢的尝试下一阶段吧。");
        elseif( id == 30811)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30811,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        属于你的历练还在继续，勇敢的尝试下一阶段吧。");
        elseif( id == 30812)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30812,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        属于你的历练还在继续，勇敢的尝试下一阶段吧。");
        elseif( id == 30813)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30813,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        属于你的历练还在继续，勇敢的尝试下一阶段吧。");
        elseif( id == 30814)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30814,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        传说中没人可以完成如此的试炼……");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30800)then
	      		Quest.CloseNPCTalk();
	      		Quest.CompleteQuestTalk(30800,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        这一切的安排，是神的旨意。");
				elseif( id == 30801)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30801,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        家族是最亲密最强大的组织，快去组建或者加入家族吧！");
				elseif( id == 30802)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30802,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        快去寻找属于你自己的恋人吧！");
        elseif( id == 30803)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30803,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        已经找到他了吗？我相信他会为你以后的修炼历程给予帮助。");
        elseif( id == 30804)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30804,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        继续去积攒你的历练经验吧，丰厚奖励在等着你。");
        elseif( id == 30805)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30805,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        要知道，刚才只是仅仅完成了所有阶段的十分之一……");
       elseif( id == 30806)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30806,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        完成了这个阶段的历练，你可以小有收获了哦。");
       elseif( id == 30807)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30807,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        相信伟大的荣耀就在这不久之后属于你。");
       elseif( id == 30808)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30808,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        一切考验都是纸老虎。");
       elseif( id == 30809)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30809,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        历久的磨难你都可以挺到这里，不得不说这是一个奇迹。");
       elseif( id == 30810)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30810,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        完成了这个阶段的历练，你可以小有收获了哦。");
       elseif( id == 30811)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30811,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        相信伟大的荣耀就在这不久之后属于你。");
       elseif( id == 30812)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30812,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        一切考验都是纸老虎。");
       elseif( id == 30813)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30813,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        历久的磨难你都可以挺到这里，不得不说这是一个奇迹。");
       elseif( id == 30814)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30814,"<font=龙字体,0,19,20><color=0xfffcf157>回购指引人：<color=0xffffffff><font=龙字体,0,17,20>\\n        你竟然……");
       end
end

Rhuigouzhiyin = {}
Rhuigouzhiyin["OnTalk"] = OnTalk
Rhuigouzhiyin["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rhuigouzhiyin["OnCompleteQuestTalk"] = OnCompleteQuestTalk