
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010015);
	Quest.SetCompleteOptions(5010015);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神教官达蒙：<color=0xffffffff>\\n这里是被所有神遗忘的地方。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20007)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20007,"<font=龙字体,0,19,20><color=0xfffcf157>神教官达蒙：<color=0xffffffff>\\n    这里的<A>花妖</A>，都是曾经的死亡在此的魔族所化，必须及时清除掉。完成后再回复我。");
        elseif( id == 20008)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20008,"<font=龙字体,0,19,20><color=0xfffcf157>神教官达蒙：<color=0xffffffff>\\n    从这里一直往北走，<D>统帅伊万夫</D>就在那里镇守着通往斗战天城的路，新人都要去他那里接受试炼。");
        elseif( id == 20025)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20025,"<font=龙字体,0,19,20><color=0xfffcf157>神教官达蒙：<color=0xffffffff>\\n    在瀑布之下有一个被封印的花妖之神，历经数年早已变成一尊石像，但是那里还是有诡异的感觉，可能是花妖之神留下的神之花粉致使这里拥有众多花妖，请你去那里拿回一些神之花粉。");
	  elseif( id == 20026)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20026,"<font=龙字体,0,19,20><color=0xfffcf157>神教官达蒙：<color=0xffffffff>\\n    为了提高自己的速度，使用坐骑是必要的选择。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20006)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20006,"<font=龙字体,0,19,20><color=0xfffcf157>神教官达蒙：<color=0xffffffff>\\n   在这里我还有工作要做，但是工作太多，你的到来正好可以助我一臂之力。");
        elseif( id == 20007)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20007,"<font=龙字体,0,19,20><color=0xfffcf157>神教官达蒙：<color=0xffffffff>\\n   干得不错！第一阶段顺利通过，你接下来要去接受新的试炼。");
        elseif( id == 20025)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20025,"<font=龙字体,0,19,20><color=0xfffcf157>神教官达蒙：<color=0xffffffff>\\n   恩，看来你也祭拜过花妖之神了，那么接下来的事情，先让我看看。");

       end

end

damon = {}
damon["OnTalk"] = OnTalk
damon["OnAcceptQuestTalk"] = OnAcceptQuestTalk
damon["OnCompleteQuestTalk"] = OnCompleteQuestTalk