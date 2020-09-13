
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010023);
	Quest.SetCompleteOptions(5010023);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n查看妖精的属性中可以看到灵魂槽，灵魂槽可以融合各种灵魂，妖精之魂有种类和等级之分，融合之后可增加各种属性。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40004)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40004,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n你的妖精已经达到了<C>10</C>级，此时你会发现妖精属性中增加<C>１</C>个空的<N>灵魂槽</N>。\\n灵魂槽可以融合各种灵魂，<I>灵魂</I>分为不同等级为很多种类，融合之后可增加各种属性。融合灵魂的操作可以在妖精界面中进行。\\n接下来与我对话，我会告诉你更多的内容。");
        elseif( id == 40005)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40005,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n你是否发现了灵魂分为不同类型，而灵魂槽也分为了不同类型？如果融合的灵魂与灵魂槽是一致的，则会激活妖精的<color=0xff9933ff>灵魂觉醒能力<color=0xffffffff>！\\n灵魂槽的属性是可以通过<color=0xff9933ff>妖精融合<color=0xffffffff>来洗的，不过这个功能需要妖精的等级达到<C>60</C>级以后才能使用。\\n努力地把妖精的等级提升到<C>20</C>级，再回来找我吧！");
        elseif( id == 40006)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40006,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n每个妖精的基础能力和灵魂觉醒能力是固定不变的，因此在培养妖精之前，你还是要认真确认该妖精是否是自己所需要的。\\n希望你的妖精等级到达<C>30</C>级，我会给予你更多的奖励。");
        elseif( id == 40007)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40007,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n<I>妖精灵魂</I>整体上可分为十个等级，很多灵魂需要进入副本内获得。\\n若你的妖精成长到了<C>40</C>级，我这里有新的<I>灵魂</I>送予你。");
        elseif( id == 40008)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40008,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n妖精同样可以<T>镶嵌宝石</T>，但是和装备所镶嵌的宝石不同，妖精的宝石是独特的，只可镶嵌于妖精上。\\n你若将妖精的等级提升至<C>50</C>级，就可以从我这里获得<I>妖精宝石</I>。");
        elseif( id == 40009)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40009,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n等你的妖精到达<C>60</C>级之后，我会告诉你如何融合妖精，以及培育极品妖精的心得，加油吧！");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40004)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40004,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n当妖精等级达到<C>10、20、30、40、60</C>级的时候，均会开启新的<N>灵魂槽</N>，每个妖精最多可以开启<A>5个</A>灵魂槽。\\n这里有几种妖精之魂，你选择一种自己所需要的吧，然后尝试将灵魂融合入灵魂槽。");
        elseif( id == 40005)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40005,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n你的妖精已经开启了<C>２</C>个灵魂槽，需要注意的是，融合灵魂时，有可能将之前已融合的灵魂覆盖。\\n妖精之魂在很多地方可以获得，比如副本里打怪掉落，努力地去提升妖精的能力吧！");
        elseif( id == 40006)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40006,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n很好很好！你对于培育妖精越来越精通了。");
        elseif( id == 40007)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40007,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n通过融合灵魂，你的妖精会越来越强大，若能够使得妖精的灵魂觉醒，还可以更上一层楼。");  
        elseif( id == 40008)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40008,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n这里有1颗妖精专用宝石，你收好！记得这种宝石只可镶嵌于妖精上"); 
        elseif( id == 40009)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40009,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n你的妖精终于进化为了高级形态，当妖精到达这个等级后，便可以开始进行<color=0xff9933ff>妖精融合<color=0xffffffff>。\\n妖精融合就是消耗一只妖精，去改变主妖精的灵魂槽属性。<color=0xff9933ff>两个妖精的资质与融合妖精的等级会影响融合的效果<color=0xffffffff>，最好的情况下，会出现五个灵魂槽属性完全相同的情况。妖精融合时，不会改变已经嵌入的灵魂，所以请放心。\\n以下是我总结的培育妖精的心得，希望对你有所帮助：\\n    <color=0xffff6000>１、首先要获得一个品质、属性优异的妖精，副本会掉落这样的妖精<color=0xffffffff>\\n    <color=0xffff6000>２、洗资质，提升妖精的资质<color=0xffffffff>\\n    <color=0xffff6000>３、提升妖精等级，开启全部灵魂槽<color=0xffffffff>\\n    <color=0xffff6000>４、将所有灵魂槽都嵌入自己所需的极品灵魂<color=0xffffffff>\\n    <color=0xffff6000>５、融合妖精，转化灵魂槽属性，将所有灵魂槽的觉醒能力激活<color=0xffffffff>\\n好了，你基本已经全面掌握的妖精的内容，接下来就是你自己努力探索了！"); 
	elseif( id == 40003)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40003,"<font=龙字体,0,19,20><color=0xfffcf157>灵魂使者法娜：<color=0xffffffff><font=龙字体,0,17,20>\\n很好！你发现妖精的形态已经起了变化么？妖精会在<C>10</C>级、<C>60</C>级两次进行进化变换形态。");  
       end


end

Rfana = {}
Rfana["OnTalk"] = OnTalk
Rfana["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rfana["OnCompleteQuestTalk"] = OnCompleteQuestTalk