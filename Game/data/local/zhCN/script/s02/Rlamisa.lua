
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010086);
	Quest.SetCompleteOptions(5010086);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n一件装备，若追加了圣印，就很难更改，唯一的办法是使用“归元石”。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40037)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40037,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n一件装备，若追加了圣印，就很难更改，唯一的办法是使用“<I>归元石</I>”。\\n<I>归元石</I>会<T>直接清除掉装备全部的圣印</T>，因此一定要小心慎用。\\n你去帮我杀掉<A>无影</A>，我就白送你1颗归元石。");
        elseif( id == 40038)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40038,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n如果你拿<C>20</C>个<I>强化石</I>来，我可以用<I>2级圣印</I>来进行交换，怎么样？");
        elseif( id == 40039)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40039,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n这次的条件比较苛刻，希望你能够全力达成，需要你击杀<C>40</C>只<A>原神精英教徒</A>与<C>40</C>只</A>原神精英刺客</A>，你是否愿意？");
        elseif( id == 40040)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40040,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n只要你收集到<C>300</C>份<I>千年元神</I>，我就用<I>4级圣印</I>来与你交换！");
        elseif( id == 20415)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20415,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n关于圣印，就说到这里吧，接下来继续你的征程吧。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40037)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40037,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n做的不错啊，归元石拿去吧！");
        elseif( id == 40038)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40038,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n这种交易你是不会亏的！放心吧！");
        elseif( id == 40039)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40039,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n天界有你这样的斗神精英，我们都感到十分欣慰！");
        elseif( id == 40040)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40040,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n很辛苦吧！这个是你该得的！");
        elseif( id == 40036)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40036,"<font=龙字体,0,19,20><color=0xfffcf157>圣印师拉米萨：<color=0xffffffff><font=龙字体,0,17,20>\\n在追加圣印的时候是有几率失败的，而且一件装备追加等级越高，越不容易成功，这个你可要记住。\\n在<T>升级圣印</T>的时候，如果失败了，则会<T>摧毁掉1个合成用的圣印</T>。");
       end

end

Rlamisa = {}
Rlamisa["OnTalk"] = OnTalk
Rlamisa["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rlamisa["OnCompleteQuestTalk"] = OnCompleteQuestTalk