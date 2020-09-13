
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010025);
	Quest.SetCompleteOptions(5010025);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>弗伊达：<color=0xffffffff><font=龙字体,0,17,20>\\n在进行生产之前，都要先获得原矿。原矿是在矿洞中使用矿镐采集获得的。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40011)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40011,"<font=龙字体,0,19,20><color=0xfffcf157>弗伊达：<color=0xffffffff><font=龙字体,0,17,20>\\n      在<N>矿洞内</N>的<T>采集区域内（非通道地带）</T>可以进行采矿，你可以在小地图上查看。\\n      <T>使用背包中的矿镐</T>即可进行持续采矿，直到<T>手动取消采矿</T>、<N>背包已满</N>或是<A>矿镐的使用次数消耗完毕</A>才会停止。\\n      你去尝试使用我刚才送你的<I>矿镐</I>，采集<C>10</C>块<I>原矿</I>吧！");
        elseif( id == 40012)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40012,"<font=龙字体,0,19,20><color=0xfffcf157>弗伊达：<color=0xffffffff><font=龙字体,0,17,20>\\n你刚才已经获得了一些<I>原矿</I>，现在你可以使用你的<T>加工技能（技能列表中可以查看）</T>，将这些<I>原矿</I>加工出<C>２</C>块<I>一等生铁</I>，然后交给<D>工神鲁斯</D>。\\n \\n<T>注意，加工技能会消耗你的活力值，你可以在属性界面中查看你的当前活力值，活力值会随着时间缓慢恢复</T>");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40010)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40010,"<font=龙字体,0,19,20><color=0xfffcf157>弗伊达：<color=0xffffffff><font=龙字体,0,17,20>\\n想来学习如何采矿？那你先拿好这个工具！");
        elseif( id == 40011)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40011,"<font=龙字体,0,19,20><color=0xfffcf157>弗伊达：<color=0xffffffff><font=龙字体,0,17,20>\\n当矿镐使用次数消耗完毕，矿镐就会消失。你可以在<D>杂货商</D>处购买新的矿镐。");
        
       end


end

Rfoyida = {}
Rfoyida["OnTalk"] = OnTalk
Rfoyida["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rfoyida["OnCompleteQuestTalk"] = OnCompleteQuestTalk