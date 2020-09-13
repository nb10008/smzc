
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610045);
	Quest.SetCompleteOptions(5610045);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>信仰之怒兑换人<color=0xffffffff><font=龙字体,0,17,20>\\n你可以用信仰之怒和白银宝鉴来兑换白银·信仰之怒\\n<I>信仰之怒</I>*<C>1</C>+<I>白银宝鉴</I>*<C>1</C>=<I>白银·信仰之怒</I>*<C>1</C>\\n<I>信仰之怒</I>*<C>5</C>+<I>白银宝鉴</I>*<C>5</C>=<I>白银·信仰之怒</I>*<C>5</C>\\n<I>信仰之怒</I>*<C>50</C>+<I>白银宝鉴</I>*<C>50</C>=<I>白银·信仰之怒</I>*<C>50</C>\\n<I>信仰之怒</I>*<C>100</C>+<I>白银宝鉴</I>*<C>100</C>=<I>白银·信仰之怒</I>*<C>100</C>");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30366)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30366,"<font=龙字体,0,19,20><color=0xfffcf157>信仰之怒兑换人<color=0xffffffff><font=龙字体,0,17,20>\\n给我足够的信仰之怒和白银宝鉴，我就给你你想要的东西。");
        elseif( id == 30367)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30367,"<font=龙字体,0,19,20><color=0xfffcf157>信仰之怒兑换人<color=0xffffffff><font=龙字体,0,17,20>\\n给我足够的信仰之怒和白银宝鉴，我就给你你想要的东西。");
        elseif( id == 30368)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30368,"<font=龙字体,0,19,20><color=0xfffcf157>信仰之怒兑换人<color=0xffffffff><font=龙字体,0,17,20>\\n给我足够的信仰之怒和白银宝鉴，我就给你你想要的东西。");
         elseif( id == 30369)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30369,"<font=龙字体,0,19,20><color=0xfffcf157>信仰之怒兑换人<color=0xffffffff><font=龙字体,0,17,20>\\n给我足够的信仰之怒和白银宝鉴，我就给你你想要的东西。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30366)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30366,"<font=龙字体,0,19,20><color=0xfffcf157>信仰之怒兑换人<color=0xffffffff><font=龙字体,0,17,20>\\n想要兑换的时候就来找我吧。");
	elseif( id == 30367)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30367,"<font=龙字体,0,19,20><color=0xfffcf157>信仰之怒兑换人<color=0xffffffff><font=龙字体,0,17,20>\\n想要兑换的时候就来找我吧。");
        elseif( id == 30368)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30368,"<font=龙字体,0,19,20><color=0xfffcf157>信仰之怒兑换人<color=0xffffffff><font=龙字体,0,17,20>\\n想要兑换的时候就来找我吧。");
      elseif( id == 30369)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30369,"<font=龙字体,0,19,20><color=0xfffcf157>信仰之怒兑换人<color=0xffffffff><font=龙字体,0,17,20>\\n想要兑换的时候就来找我吧。");

       end


end

Rxyzndhr = {}
Rxyzndhr["OnTalk"] = OnTalk
Rxyzndhr["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxyzndhr["OnCompleteQuestTalk"] = OnCompleteQuestTalk