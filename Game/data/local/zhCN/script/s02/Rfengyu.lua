
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010026);
	Quest.SetCompleteOptions(5010026);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>凤羽：<color=0xffffffff>\\n斗战天城是整个神界的核心，就职、交易、生活均聚集于此。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20019)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20019,"<font=龙字体,0,19,20><color=0xfffcf157>凤羽：<color=0xffffffff>\\n<N>斗战天城</N>乃天界战神居住之城，听命于众神，统辖人界的四大洲。东、西可分别前往<N>七花之海[10-20级]</N>与<N>奔雷裂谷[20-30级]</N>。有时间的话你可以在城里多转转！\\n<D>神使基拉</D>正在等你，你去找他吧。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20018)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20018,"<font=龙字体,0,19,20><color=0xfffcf157>凤羽：<color=0xffffffff>\\n欢迎来到斗战天城！接下来向你介绍一下吧！");

       end

end

Rfengyu = {}
Rfengyu["OnTalk"] = OnTalk
Rfengyu["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rfengyu["OnCompleteQuestTalk"] = OnCompleteQuestTalk