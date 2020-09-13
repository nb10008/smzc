
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010015);
	Quest.SetCompleteOptions(5010015);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>新手教官泰达：<color=0xffffffff>\\n这里就是曾经神魔大战的古战场。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20007)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20007,"<font=龙字体,0,19,20><color=0xfffcf157>新手教官泰达：<color=0xffffffff>\\n    这南面的<A>幻魔蝶</A>，都是曾经的死亡在此的魔族所化，必须及时清除掉。完成后再回复我。");
        elseif( id == 20008)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20008,"<font=龙字体,0,19,20><color=0xfffcf157>新手教官泰达：<color=0xffffffff>\\n    从这里一直往东走，你就会看到神冢关了，<color=0xff00ddbb>新兵统帅翔天<color=0xffffffff>就在那里镇守着通往斗战天城的天门。新来的天兵都要去那里接受考验，只有通过的人才有资格进入天城，祝你好运吧。");
        elseif( id == 20025)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20025,"<font=龙字体,0,19,20><color=0xfffcf157>新手教官泰达：<color=0xffffffff>\\n    这个水池中，供奉着曾在此战斗的天神，你也去祭拜一下吧。带一些神冢附近潮湿的土壤回来，快去吧。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20006)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20006,"<font=龙字体,0,19,20><color=0xfffcf157>新手教官泰达：<color=0xffffffff>\\n   我在这里就是要保护天神冢的安全，你来协助我，同时也是你试炼的一部分。");
        elseif( id == 20007)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20007,"<font=龙字体,0,19,20><color=0xfffcf157>新手教官泰达：<color=0xffffffff>\\n   干得不错！第一阶段顺利通过，你接下来要去神冢关接受新的试炼。");
        elseif( id == 20025)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20025,"<font=龙字体,0,19,20><color=0xfffcf157>新手教官泰达：<color=0xffffffff>\\n   恩，看来你也祭拜过天神了，那么接下来的事情，先让我看看。");

       end

end

Rtaida = {}
Rtaida["OnTalk"] = OnTalk
Rtaida["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rtaida["OnCompleteQuestTalk"] = OnCompleteQuestTalk