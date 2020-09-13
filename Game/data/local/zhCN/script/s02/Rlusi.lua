
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010024);
	Quest.SetCompleteOptions(5010024);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n在进行生产之前，都要先获得原矿。原矿是在矿洞中使用矿镐采集获得的。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40010)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40010,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n你已经达到了<C>20</C>级，自<C>20</C>级开始，你可以使用<T>生产技能（可在技能列表中查看）</T>了，我来做详细说明下：\\n    生产技能主要分为<C>4</C>种\\n    <C>加工</C>：可以将原矿加工后获得各种材料\\n    <A>铸造</A>：可以制作各种武器和饰品\\n    <D>裁缝</D>：可以制作各种防具\\n    <P>点化</P>：可以将装备分解为各种物品\\n在进行生产之前，都要先获得<I>原矿</I>，<I>原矿</I>是在<N>矿洞</N>中使用<I>矿镐</I>采集获得的。接下来你就前往<N>矿洞</N>，找到那里的<D>弗伊达</D>吧，他会指引你如何进行采集");
        elseif( id == 40013)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40013,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n      <I>原矿</I>是无法直接制作装备的，必须加工为材料后才可以。材料有品质之分，用“几等”来进行标识，<T>数字越高，代表材料的品质越高</T>。\\n<T>      在制作装备时，<T>使用的材料品质越高，制作出高品质装备的几率越大。</T>想要加工出高品质的材料，不能只有<I>原矿</I>，还需要<I>结晶</I>，<I>结晶</I>是各种<T>20级以上魔物</T>都有可能掉落，接下来，去收集<C>10</C>份<I>结晶</I>吧！");
	elseif( id == 40014)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40014,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n从生产<C>50</C>级的装备开始，除了材料以外，还需要元神材料。元神材料也是由各种怪物掉落获得的。\\n请你去尝试收集<C>20</C>份初等元神，完成后我将给予你高额的奖励");
        elseif( id == 40035)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40035,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n在这个世间，存在着一种极其强大的物品，我们统称为“<I>圣印</I>”，这些<I>圣印</I>可以直接追加到<T>装备</T>上，完成后装备名称后会显示“<T>追加等级</T>”。\\n<I>圣印</I>总体来说可以分成两大类：<N>伤害圣印</N>和<C>减免圣印</C>。<N>伤害圣印</N>只能追加到装备和饰品上，<C>减免圣印</C>只可追加到防具上。\\n你帮我去击杀<C>30</C>只<A>三鬼教徒</A>和<C>30</C>只<A>三鬼教众</A>，然后从我这里领取一份<I>圣印奖励</I>吧！");
        elseif( id == 40036)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40036,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n<I>圣印</I>可以通过点化装备、击杀巨型魔物、魔神来获得，而且<I>圣印</I>除了种类有差异外，也分为<T>多个等级</T>，你可以在“<D>圣印升级师</D>”处进行升级。\\n<D>圣印师拉米萨</D>是圣印的专家，你可以找他了解更多的内容");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40012)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40012,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n很好嘛！看来你已经掌握了如何<C>采矿</C>。在你刚才加工原矿成功后，你是否发现<T>加工技能的熟练度</T>增加了？\\n除了采矿和点化，其他生产技能都有<T>熟练度</T>，<T>熟练度</T>满了以后技能会自动升级，升级以后会出现更多的生产配方，你可以在技能界面通过生产技能图标下方的<N>进度条</N>来查看<T>熟练度</T>。");
        elseif( id == 40013)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40013,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n有了<I>原矿</I>和<I>结晶</I>，你就可以加工出更高品质的材料了，一定要牢记。");   
        elseif( id == 40014)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40014,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n恭喜你，你已经全部掌握的材料知识！");  
        elseif( id == 40035)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40035,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n这里有几个<I>伤害圣印</I>，请选择一个自己需要的吧！");  
        elseif( id == 20414)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20414,"<font=龙字体,0,19,20><color=0xfffcf157>工神鲁斯：<color=0xffffffff><font=龙字体,0,17,20>\\n等你很久了。");  
       end


end

Rlusi = {}
Rlusi["OnTalk"] = OnTalk
Rlusi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rlusi["OnCompleteQuestTalk"] = OnCompleteQuestTalk