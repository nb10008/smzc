
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010019);
	Quest.SetCompleteOptions(5010019);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>萨默斯：<color=0xffffffff>\\n只要完成一系列的试炼，你便可以前往斗战天城。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20013)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20013,"<font=龙字体,0,19,20><color=0xfffcf157>萨默斯：<color=0xffffffff>\\n    附近的<A>紫色魔花</A>十分猖獗，这会成为你的一项考验，你是否愿意接受？如果完成了，便可以向<D>魔神度撒</D>申请进行最终的试炼吧！");
        elseif( id == 20016)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20016,"<font=龙字体,0,19,20><color=0xfffcf157>萨默斯：<color=0xffffffff>\\n    找到<D>罗斯特</D>，他那里有更好的奖励给你，你去找他吧！");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20012)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20012,"<font=龙字体,0,19,20><color=0xfffcf157>萨默斯：<color=0xffffffff>\\n   很好！离最终考验又进了一步！");
        elseif( id == 20015)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20015,"<font=龙字体,0,19,20><color=0xfffcf157>萨默斯：<color=0xffffffff>\\n   能够成功挑战魔神证明你已经具备了成为一名战神的初步实力，这些东西是给你的奖励，收下吧！\\n   记得在每次升级后都有新的技能点数，不要忘记升级你的技能。");
       end

end

samosi = {}
samosi["OnTalk"] = OnTalk
samosi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
samosi["OnCompleteQuestTalk"] = OnCompleteQuestTalk