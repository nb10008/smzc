
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010020);
	Quest.SetCompleteOptions(5010020);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>斗神阿拉斯：<color=0xffffffff>\\n准备好接受最后的考验了吗？");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20014)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20014,"<font=龙字体,0,19,20><color=0xfffcf157>斗神阿拉斯：<color=0xffffffff>\\n    作为七十二斗神之一，我奉命在此看守<A>六天神魔</A>，它是十分凶悍的魔物，我们已经将他大部分的邪魔之力封印，因此最后的任务就是去击败这<A>六天神魔</A>！若多人协作，则会更加容易。");
        elseif( id == 20015)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20015,"<font=龙字体,0,19,20><color=0xfffcf157>斗神阿拉斯：<color=0xffffffff>\\n    你可以向<D>教官古依萨</D>报告，说明已经完成最后的试炼，可以成为一个真正的天兵了。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20013)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20013,"<font=龙字体,0,19,20><color=0xfffcf157>斗神阿拉斯：<color=0xffffffff>\\n   接下来的可是十分艰难的挑战，做好准备吧！若不幸死亡，可以使用这个<I>复活秘卷</I>进行<color=0xffff00ff>原地复活<color=0xffffffff>。");
        elseif( id == 20014)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20014,"<font=龙字体,0,19,20><color=0xfffcf157>斗神阿拉斯：<color=0xffffffff>\\n   祝贺你！通过了最终的考验！");
	elseif( id == 30788)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30788,"<font=龙字体,0,19,20><color=0xfffcf157>斗神阿拉斯：<color=0xffffffff>\\n   我的儿子实在是太忙了，谢谢你的帮忙。");
       end

end

Ralasi = {}
Ralasi["OnTalk"] = OnTalk
Ralasi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ralasi["OnCompleteQuestTalk"] = OnCompleteQuestTalk
