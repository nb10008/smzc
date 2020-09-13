
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010016);
	Quest.SetCompleteOptions(5010016);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>新兵统帅翔天：<color=0xffffffff><font=龙字体,0,17,20>\\n我在神冢关负责看守这上古战场，因为这里是通向斗战天城的唯一通路，不可有一丝马虎。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20009)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20009,"<font=龙字体,0,19,20><color=0xfffcf157>新兵统帅翔天：<color=0xffffffff><font=龙字体,0,17,20>\\n    创世之时诞生三界：天、魔、人。一直以来三界互不相通，平安无事，直到魔界开通了与天界的通路，天魔之战持续百年。如今这里埋葬了无数当年英勇的天神，也残留着很多的魔物，与他们对抗需要更好的装备，你可找到我的<color=0xff00ddbb>军需官纳兰<color=0xffffffff>领取更好的武器。");
        elseif( id == 20011)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20011,"<font=龙字体,0,19,20><color=0xfffcf157>新兵统帅翔天：<color=0xffffffff><font=龙字体,0,17,20>\\n    现在你可以前往神冢关的北部，讨伐那些一直骚扰这天神冢的<A>幻妖蝶</A>，之后再回来找我接受新的任务。");
        elseif( id == 20012)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20012,"<font=龙字体,0,19,20><color=0xfffcf157>新兵统帅翔天：<color=0xffffffff><font=龙字体,0,17,20>\\n    接下来前往神冢关的北部，从天神冢北部的<A>神冢虚魔</A>身上收集<C>6</C>个<I>神冢塔牌</I>，然后交给<D>教官古依萨</D>。");
        elseif( id == 20018)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20018,"<font=龙字体,0,19,20><color=0xfffcf157>新兵统帅翔天：<color=0xffffffff><font=龙字体,0,17,20>\\n    你可前往<N>东南方的传送门</N>，踏上这个传送门之后，你便可以前往恢弘的<N>斗战天城</N>。以后我们可能再无机会相见，希望你能够更加精进，在你到达<color=0xffff0000>20级<color=0xffffffff>的时候，若通过了就职任务，那么你便可以成为一名真正的战神。\\n    在抵达斗战天城以后，你可以找到凤羽，她将为你进行下一步的指引。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20008)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20008,"<font=龙字体,0,19,20><color=0xfffcf157>新兵统帅翔天：<color=0xffffffff><font=龙字体,0,17,20>\\n    新来的天兵吧，准备在这里接受新一轮的试炼吧！");
        elseif( id == 20010)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20010,"<font=龙字体,0,19,20><color=0xfffcf157>新兵统帅翔天：<color=0xffffffff><font=龙字体,0,17,20>\\n    接下来就准备进行战斗吧，使用<S>快捷键</S>中的技能，可更加快速的击杀怪物。");	
        elseif( id == 20011)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20011,"<font=龙字体,0,19,20><color=0xfffcf157>新兵统帅翔天：<color=0xffffffff><font=龙字体,0,17,20>\\n    做的很好。现在给予你的任务还是很简单的，那你也要认真起来啊！");
        elseif( id == 20017)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20017,"<font=龙字体,0,19,20><color=0xfffcf157>新兵统帅翔天：<color=0xffffffff><font=龙字体,0,17,20>\\n    我就知道你没有问题，看，我已经为你准备好了，这些物件全都是送你的。");
       end

end

Rxiangtian = {}
Rxiangtian["OnTalk"] = OnTalk
Rxiangtian["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxiangtian["OnCompleteQuestTalk"] = OnCompleteQuestTalk