
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010022);
	Quest.SetCompleteOptions(5010022);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精嘉迪拉：<color=0xffffffff><font=龙字体,0,17,20>\\n只要将妖精携带在身上，当你获得经验时，妖精也会有经验分成，可以提升妖精的培育程度和等级。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40003)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40003,"<font=龙字体,0,19,20><color=0xfffcf157>妖精嘉迪拉：<color=0xffffffff><font=龙字体,0,17,20>\\n妖精的<N>资质</N>是一个重要的属性，它影响了妖精基础属性的成长量。而不同品质的妖精，他们的资质上限也是有区分的。\\n接下来如果你的妖精达到<C>10</C>级之后，你可以去<N>斗战天城</N>找到<D>灵魂使者法娜</D>，她会给你更多的指引。");


        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40002)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40002,"<font=龙字体,0,19,20><color=0xfffcf157>妖精嘉迪拉：<color=0xffffffff><font=龙字体,0,17,20>\\n只要将妖精携带在身上，如果获得了经验，妖精也会有经验分成，可以提升妖精的培育程度和升级。\\n除此之外，还可以使用<I>妖精之粉</I>来提升妖精的培育程度，妖精之粉等级越高，提升的培育程度越大。你可以在妖精界面中执行这个操作。\\n这里我先赠送你份<I>妖精之粉</I>，能够帮助你快速提升妖精的等级。");

        
       end


end

Rjiadila = {}
Rjiadila["OnTalk"] = OnTalk
Rjiadila["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rjiadila["OnCompleteQuestTalk"] = OnCompleteQuestTalk