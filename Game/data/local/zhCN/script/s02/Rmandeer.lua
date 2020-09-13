
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010196);
	Quest.SetCompleteOptions(5010196);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n    如果要转职的话，需要一些特定的道具。在二转之后，也可以到这里来转换职业。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 41001)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41001,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n你确定要转职成为神兵吗？");
	elseif( id == 41002)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41002,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n你确定要转职成为玄翎吗？");
	elseif( id == 41004)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41004,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n你确定要转职成为仙音吗？");
	elseif( id == 41003)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41003,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n你确定要转职成为乾坤吗？");
	elseif( id == 40057)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40057,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n预备天神在达到20级之后才算是真正涉足神的领域。去找三界主宰吧，他会为你以后的成长指明方向。");
	elseif( id == 41005)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41005,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n现在你可以将你的职业进行一次升华，这不仅是一个仪式，也是我对你的一次考验。在<N>西洲幻林</N>中，有一件上古时代的武器，如果你可以成功的挑战它，将那挑战上古圣器的证明拿回来那么我就认可你的实力。还有，那里有很多守护着上古圣器的守护者，我希望你能击败他们，这样的实力我是绝对认可的。");
	elseif( id == 41006)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41006,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n现在你可以将你的职业进行一次升华，这不仅是一个仪式，也是我对你的一次考验。在<N>西洲幻林</N>中，有一件上古时代的武器，如果你可以成功的挑战它，将那挑战上古圣器的证明拿回来那么我就认可你的实力。还有，那里有很多守护着上古圣器的守护者，我希望你能击败他们，这样的实力我是绝对认可的。");
	 elseif( id == 41007)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41007,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n现在你可以将你的职业进行一次升华，这不仅是一个仪式，也是我对你的一次考验。在<N>西洲幻林</N>中，有一件上古时代的武器，如果你可以成功的挑战它，将那挑战上古圣器的证明拿回来那么我就认可你的实力。还有，那里有很多守护着上古圣器的守护者，我希望你能击败他们，这样的实力我是绝对认可的。");	
        elseif( id == 41008)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41008,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n现在你可以将你的职业进行一次升华，这不仅是一个仪式，也是我对你的一次考验。在<N>西洲幻林</N>中，有一件上古时代的武器，如果你可以成功的挑战它，将那挑战上古圣器的证明拿回来那么我就认可你的实力。还有，那里有很多守护着上古圣器的守护者，我希望你能击败他们，这样的实力我是绝对认可的。");
	elseif( id == 41009)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41009,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n现在你可以将你的职业进行一次升华，这不仅是一个仪式，也是我对你的一次考验。在<N>西洲幻林</N>中，有一件上古时代的武器，如果你可以成功的挑战它，将那挑战上古圣器的证明拿回来那么我就认可你的实力。还有，那里有很多守护着上古圣器的守护者，我希望你能击败他们，这样的实力我是绝对认可的。");	
        elseif( id == 41010)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41010,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n现在你可以将你的职业进行一次升华，这不仅是一个仪式，也是我对你的一次考验。在<N>西洲幻林</N>中，有一件上古时代的武器，如果你可以成功的挑战它，将那挑战上古圣器的证明拿回来那么我就认可你的实力。还有，那里有很多守护着上古圣器的守护者，我希望你能击败他们，这样的实力我是绝对认可的。");
	elseif( id == 41011)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41011,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n现在你可以将你的职业进行一次升华，这不仅是一个仪式，也是我对你的一次考验。在<N>西洲幻林</N>中，有一件上古时代的武器，如果你可以成功的挑战它，将那挑战上古圣器的证明拿回来那么我就认可你的实力。还有，那里有很多守护着上古圣器的守护者，我希望你能击败他们，这样的实力我是绝对认可的。");	
        elseif( id == 41012)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41012,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n现在你可以将你的职业进行一次升华，这不仅是一个仪式，也是我对你的一次考验。在<N>西洲幻林</N>中，有一件上古时代的武器，如果你可以成功的挑战它，将那挑战上古圣器的证明拿回来那么我就认可你的实力。还有，那里有很多守护着上古圣器的守护者，我希望你能击败他们，这样的实力我是绝对认可的。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
    if( id == 41000)then
        Quest.CloseNPCTalk();
   		 Quest.CompleteQuestTalk(41000,"<font=龙字体,0,19,20><color=0xfffcf157>转职官曼德尔：<color=0xffffffff>\\n转职官曼德尔：现在就选择你的职业吧，选择对应的<I>武之契</I>，然后到玛斯大人那里开始转职任务，最后到对应的<D>武器之灵</D>那里进行转职就可以了。\\n<color=0xffff00ff>转职需要的武之契不可删除，而且你现在只有一次转职的机会，切记要好好选择，当然以后你还会有转换职业的机会<color=0xffffffff>。");
   	end

end

Rmandeer = {}
Rmandeer["OnTalk"] = OnTalk
Rmandeer["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rmandeer["OnCompleteQuestTalk"] = OnCompleteQuestTalk