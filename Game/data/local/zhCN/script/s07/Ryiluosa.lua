
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010165);
	Quest.SetCompleteOptions(5010165);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>易罗萨：<color=0xffffffff>\\n这怎么可能？也确实有可能。不过……");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20505)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20505,"<font=龙字体,0,19,20><color=0xfffcf157>易罗萨：<color=0xffffffff>\\n这些<A>野人</A>被什么影响了，但我不能确定和魔族有关，也许是另一种力量，也许从他们身上可以找到线索。");
        elseif( id == 20506)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20506,"<font=龙字体,0,19,20><color=0xfffcf157>易罗萨：<color=0xffffffff>\\n也许你不会相信我，但这确实是天神才可使用的东西，那我们再来看看那里的妖迷之花之类的植物，有什么变化吧。");
        elseif( id == 20507)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20507,"<font=龙字体,0,19,20><color=0xfffcf157>易罗萨：<color=0xffffffff>\\n到<N>古茶山</N>中去，那里有个奇怪的<M>祭坛</M>，试试看能找到些什么。");
	elseif( id == 20508)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20508,"<font=龙字体,0,19,20><color=0xfffcf157>易罗萨：<color=0xffffffff>\\n这样吧，这附近有个<N>纳溪沼泽</N>，那里有一些还算和善的魔族，他们是魔族派来的使者，如果你等级到达<C>69</C>级，就可以去那里问问看。他们对天神的敌意应该比较淡。");
	
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20504)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20504,"<font=龙字体,0,19,20><color=0xfffcf157>易罗萨：<color=0xffffffff>\\n还有天神来到了这里？");
        elseif( id == 20505)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20505,"<font=龙字体,0,19,20><color=0xfffcf157>易罗萨：<color=0xffffffff>\\n这个是？！怎么可能？");
	elseif( id == 20506)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20506,"<font=龙字体,0,19,20><color=0xfffcf157>易罗萨：<color=0xffffffff>\\n看来这果然是真的。");
	elseif( id == 20507)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20507,"<font=龙字体,0,19,20><color=0xfffcf157>易罗萨：<color=0xffffffff>\\n你说那是<A>原神咒师</A>，这下事情可复杂了。");
       
       end

end

Ryiluosa = {}
Ryiluosa["OnTalk"] = OnTalk
Ryiluosa["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryiluosa["OnCompleteQuestTalk"] = OnCompleteQuestTalk