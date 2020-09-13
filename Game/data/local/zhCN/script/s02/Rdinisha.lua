
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010030);
	Quest.SetCompleteOptions(5010030);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神使迪妮莎：<color=0xffffffff>\\n我的使命就是辅佐斗神之王，管理好整个神界。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20024)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20024,"<font=龙字体,0,19,20><color=0xfffcf157>神使迪妮莎：<color=0xffffffff>\\n当你准备完毕，可以从 <N>城市西部</N>的<D>传送点</D>前往<N>七花之海</N>，到了那里找到<D>幻神艾丽娜</D>，协助她抵御魔物，她需要你的帮助，也是你的试炼的一部分。");
        elseif( id == 40041)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40041,"<font=龙字体,0,19,20><color=0xfffcf157>神使迪妮莎：<color=0xffffffff>\\n我们将诸位斗神的战斗经验、技巧、学识进行萃取，从而制作出了战斗神符。战斗神符可以自动进行战斗，你唯一要做的就是进行设置战斗神符，你想要深入了解么？");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20023)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20023,"<font=龙字体,0,19,20><color=0xfffcf157>神使迪妮莎：<color=0xffffffff>\\n你目前已经到达<C>10</C>级，在技能列表中的<color=0xffff00ff>回城术<color=0xffffffff>已经可以使用，使用后可立刻返回斗战天城，十分方便。\\n另外在出发之前请检查一下是否还有技能点没有投放<T>[快捷键K]</T>，技能升级后战斗效果会大幅度提升。\\n这里还有<I>复活秘卷</I>送你，当你死亡后，可以选择原地复活。");
        elseif( id == 40041)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40041,"<font=龙字体,0,19,20><color=0xfffcf157>神使迪妮莎：<color=0xffffffff>\\n战斗神符内可以自动吃药、自动战斗、自动选怪，在使用了战斗神符后，会出现对应的界面，在界面内，你可以自由地将需要的药水、技能放入对应的栏位，并且可以设置每1个的使用频率，当设置好以后，点击开始修炼，诸位斗神的意志就会自动控制你进行战斗。\\n这里有1个战斗神符，你收下吧！");
       end

end

Rdinisha = {}
Rdinisha["OnTalk"] = OnTalk
Rdinisha["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rdinisha["OnCompleteQuestTalk"] = OnCompleteQuestTalk