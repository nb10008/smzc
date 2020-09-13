
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010027);
	Quest.SetCompleteOptions(5010027);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神使基拉：<color=0xffffffff>\\n最近想要通过考验来到这里的新天神还真是多啊。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20020)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20020,"<font=龙字体,0,19,20><color=0xfffcf157>神使基拉：<color=0xffffffff>\\n你通过了之前的试炼，也一定获得了不少的奖励。有些不想随身携带的物品可以存放到<N>仓库</N>内，接下来去找到<D>仓库说明人</D>，他会介绍如何使用仓库。");
        elseif( id == 20022)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20022,"<font=龙字体,0,19,20><color=0xfffcf157>神使基拉：<color=0xffffffff>\\n接下来请前往城市北部的大殿，<D>斗神之王玛斯</D>正在等你。\\n大殿需要通过两侧的旋梯才可上去，别迷路了哦！");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20019)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20019,"<font=龙字体,0,19,20><color=0xfffcf157>神使基拉：<color=0xffffffff>\\n你就是刚通过考验的人？不错，有实力有潜力！");
        elseif( id == 20021)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20021,"<font=龙字体,0,19,20><color=0xfffcf157>神使基拉：<color=0xffffffff>\\n很好！我继续向你介绍下斗战天城。\\n<N>广场西侧</N>是<N>商业区</N>，那里有各式的商店，可用于购买药剂、杂货和出售物品。\\n<N>广场东侧</N>是<N>强化区</N>，可进行装备强化，虽然目前你不需要，但在日后会相当重要。\\n<N>广场东南方</N>是<N>摆摊区</N>，可在那里摆摊出售物品或是挑选他人的货物，城市的其他地方是无法摆摊的。\\n大体上就是这样，这个<I>随身仓库</I>你收下，使用后可随时随地打开仓库，注意，它是有<N>时限</N>的！");
	elseif( id == 30867)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30867,QuestTalkBegin+1,"确定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30867,QuestTalkBegin+2,"返回" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神使基拉：<color=0xffffffff>\\n    你是给我拜年来了？呵呵，真是谢谢你，我也给你拜个年，祝你新年快乐。");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.UpdateQuest(30867);
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		end
	end

end

Rjila = {}
Rjila["OnTalk"] = OnTalk
Rjila["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rjila["OnCompleteQuestTalk"] = OnCompleteQuestTalk