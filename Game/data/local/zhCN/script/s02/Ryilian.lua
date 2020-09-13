
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010081);
	Quest.SetCompleteOptions(5010081);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>匠神依利安：<color=0xffffffff><font=龙字体,0,17,20>\\n按“<C>C</C>”打开属性面板后，你会看见一项属性名为“<T>活力</T>”，当你使用生产技能进行制造时，便会消耗活力.");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40015)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40015,"<font=龙字体,0,19,20><color=0xfffcf157>匠神依利安：<color=0xffffffff><font=龙字体,0,17,20>\\n按“<C>C</C>”打开属性面板后，你会看见一项属性名为“<T>活力</T>”，当你使用生产技能进行制造时，便会消耗<T>活力</T>，若<T>活力不足</T>则无法进行生产，而随着你等级的提高，活力上限也会对应地提升。\\n你是不是很想知道如何恢复活力值？先帮我去<D>杂货商</D>那里买<C>1</C>份<I>空白制作书</I>，回来我再告诉你。");
        elseif( id == 40016)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40016,"<font=龙字体,0,19,20><color=0xfffcf157>匠神依利安：<color=0xffffffff><font=龙字体,0,17,20>\\n接下来我告诉你什么是“<I>装备制作书</I>”。你使用你的生产技能后，会发现有专门的<T>制作书配方</T>，可以制作<T>武器</T>、<T>防具</T>和<T>饰品</T>的<I>制作书</I>，接下来去制作<C>1</C>份<I>防具制作书[20级]</I>，然后回来找我，我会告诉你接下来该做什么。\\n \\n<T>提示：制作防具制作书需要使用裁缝技能后，选择对应的制作书配方</T>");
        elseif( id == 40017)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40017,"<font=龙字体,0,19,20><color=0xfffcf157>匠神依利安：<color=0xffffffff><font=龙字体,0,17,20>\\n      生产技能等级越高，可制作的东西越多。可是有些人不愿意修炼生产技能，因此无法制作高级的装备。这种情况下可以去找到“<D>装备代工师傅</D>”，在他那里可以进行很多装备的制作，但是和生产技能的区别是，在他那里生产装备需要“<I>装备制作书</I>”。\\n      接下来你去找到“<D>装备代工师傅</D>”，使用刚才获得的<I>防具制作书[20级]</I>制作<C>1</C>件<I>神恩火色披风</I>，完成后再来找我。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40015)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40015,"<font=龙字体,0,19,20><color=0xfffcf157>匠神依利安：<color=0xffffffff><font=龙字体,0,17,20>\\n恩！<T>活力</T>可以在线缓慢回复，每分钟会恢复<A>3</A>点，所以当活力不足的时候，稍微休息一下便可以继续生产。当然<I>活力药水</I>也是可以直接恢复活力的。");
        elseif( id == 40016)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40016,"<font=龙字体,0,19,20><color=0xfffcf157>匠神依利安：<color=0xffffffff><font=龙字体,0,17,20>\\n记住，合成装备制作书同样可以获得<T>技能熟练度</T>，但是也一样会消耗<C>活力</C>。");
        elseif( id == 40017)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40017,"<font=龙字体,0,19,20><color=0xfffcf157>匠神依利安：<color=0xffffffff><font=龙字体,0,17,20>\\n哈哈！很好！这一阶段的指引就到这里了！你自制的披风，最好还是你自己留着用！");
        
       end


end

Ryilian = {}
Ryilian["OnTalk"] = OnTalk
Ryilian["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryilian["OnCompleteQuestTalk"] = OnCompleteQuestTalk