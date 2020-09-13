
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010084);
	Quest.SetCompleteOptions(5010084);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>器魂师凯亚：<color=0xffffffff><font=龙字体,0,17,20>\\n当装备进行过强化，相当于装备拥有了灵魂。在这个世界上，有一种特殊的石头，可以将装备的灵魂吸取出来，它被称之为“<I>吸魂石</I>”。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40028)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40028,"<font=龙字体,0,19,20><color=0xfffcf157>器魂师凯亚：<color=0xffffffff><font=龙字体,0,17,20>\\n      当装备进行过强化，相当于装备拥有了灵魂。在这个世界上有一种特殊的石头，可以将装备的灵魂吸取出来，它被称之为“<I>吸魂石</I>”。\\n      <I>完整的吸魂石</I>在世界中存量相当稀有，更多以<I>吸魂石碎片</I>的形式存在，有以下几个途径可获得碎片：\\n      <T>1、击杀魔物有较小几率获得</T>\\n      <M>2、点化装备也有可能获得</M>\\n      接下来你先去收集<C>3</C>块<I>吸魂石碎片</I>，完成后我会指引下一步。");
        elseif( id == 40029)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40029,"<font=龙字体,0,19,20><color=0xfffcf157>器魂师凯亚：<color=0xffffffff><font=龙字体,0,17,20>\\n接下来我们完成一次<T>吸魂</T>和<T>附魂</T>的过程，这个过程实际上可以让你在一件装备上所进行强化的价值，<T>完整地转移</T>到其他的装备上。\\n再次与我对话，领取一些实验用道具吧");
        elseif( id == 40030)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40030,"<font=龙字体,0,19,20><color=0xfffcf157>器魂师凯亚：<color=0xffffffff><font=龙字体,0,17,20>\\n当你拥有<I>吸魂石</I>后，你便可以进行<T>吸魂</T>：\\n    1、首先选择需要吸魂的装备\\n    2、对该装备使用吸魂石，之后你会获得一个已经拥有装备灵魂的<M>附魂石</M>\\n    <T>3、进行吸魂的装备，强化等级会退回至0级</T>\\n用你获得的<I>初级吸魂石</I>对<I>实验戒指+2</I>进行吸魂，最后你应该获得<C>1</C>颗<I>附魂石+2</I>，完成后再来找我。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40028)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40028,"<font=龙字体,0,19,20><color=0xfffcf157>器魂师凯亚：<color=0xffffffff><font=龙字体,0,17,20>\\n      当你拥有了<M>5</M>块<I>吸魂石碎片</I>后，<T>直接使用这些碎片</T>，他们会自动转化为1块<I>初级吸魂石</I>。\\n      <I>初级吸魂石</I>可以将<M>+10</M>以下的装备灵魂吸取出来，而世界上还存在着可以吸取+10以上装备灵魂的高级吸魂石");
        elseif( id == 40029)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40029,"<font=龙字体,0,19,20><color=0xfffcf157>器魂师凯亚：<color=0xffffffff><font=龙字体,0,17,20>\\n这里有件<I>实验戒指+2</I>，这个戒指本身并没有任何好的属性，不过它具备<T>+2的强化等级</T>，接下来，我们就要用<I>吸魂石</I>将这件装备的灵魂吸出来。");
        elseif( id == 40030)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40030,"<font=龙字体,0,19,20><color=0xfffcf157>器魂师凯亚：<color=0xffffffff><font=龙字体,0,17,20>\\n      很好！这样你拥有了1颗<I>附魂石+2</I>，你可以使用这颗附魂石<T>对任何装备进行附魂</T>，附魂后的装备会自动拥有+2的强化等级。\\n      不过要注意的是，附魂后强化等级不会进行叠加，而是直接进行覆盖。若强化等级高于附魂石等级，是无法进行附魂的。\\n      你目前已经基本了解了吸魂附魂的知识，就暂时告一段落吧！");   
      
       end


end

Rkaya = {}
Rkaya["OnTalk"] = OnTalk
Rkaya["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rkaya["OnCompleteQuestTalk"] = OnCompleteQuestTalk