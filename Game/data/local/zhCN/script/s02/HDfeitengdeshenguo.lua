
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(6040018);	
	Quest.SetCompleteOptions(6040018);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n春节到了，我特意为诸位斗神制作丰盛的饺子宴，有饺子的赶快交给我吧，哈哈。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30854)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30854,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n又是一年春节到，本神锅特意为三界英豪制作<I>圣魔饺子宴</I>，想要出自己的一份力么？老规矩，上交<C>5</C>个<I>高汤饺子</I>。我是不会亏待你的，嘿嘿。");
        elseif( id == 30855)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30855,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n又是一年春节到，本神锅特意为三界英豪制作<I>圣魔饺子宴</I>，想要出自己的一份力么？老规矩，上交<C>5</C>个<I>高汤饺子</I>。我是不会亏待你的，嘿嘿。");
        elseif( id == 30856)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30856,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n又是一年春节到，本神锅特意为三界英豪制作<I>圣魔饺子宴</I>，想要出自己的一份力么？老规矩，上交<C>5</C>个<I>高汤饺子</I>。我是不会亏待你的，嘿嘿。");
        elseif( id == 30857)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30857,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n又是一年春节到，本神锅特意为三界英豪制作<I>圣魔饺子宴</I>，想要出自己的一份力么？老规矩，上交<C>5</C>个<I>高汤饺子</I>。我是不会亏待你的，嘿嘿。");
        elseif( id == 30858)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30858,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n又是一年春节到，本神锅特意为三界英豪制作<I>圣魔饺子宴</I>，想要出自己的一份力么？老规矩，上交<C>5</C>个<I>水晶饺子</I>。我是不会亏待你的，嘿嘿。");
        elseif( id == 30859)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30859,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n又是一年春节到，本神锅特意为三界英豪制作<I>圣魔饺子宴</I>，想要出自己的一份力么？老规矩，上交<C>5</C>个<I>水晶饺子</I>。我是不会亏待你的，嘿嘿。");
        elseif( id == 30860)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30860,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n又是一年春节到，本神锅特意为三界英豪制作<I>圣魔饺子宴</I>，想要出自己的一份力么？老规矩，上交<C>5</C>个<I>水晶饺子</I>。我是不会亏待你的，嘿嘿。");
        elseif( id == 30861)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30861,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n又是一年春节到，本神锅特意为三界英豪制作<I>圣魔饺子宴</I>，想要出自己的一份力么？老规矩，上交<C>5</C>个<I>水晶饺子</I>。我是不会亏待你的，嘿嘿。");

	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30854)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30854,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n呵呵，闻到香味儿了吗？<I>圣魔饺子宴</I>马上开始喽！");
        elseif( id == 30855)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30855,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n呵呵，闻到香味儿了吗？<I>圣魔饺子宴</I>马上开始喽！");
        elseif( id == 30856)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30856,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n呵呵，闻到香味儿了吗？<I>圣魔饺子宴</I>马上开始喽！");
        elseif( id == 30857)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30857,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n呵呵，闻到香味儿了吗？<I>圣魔饺子宴</I>马上开始喽！");
        elseif( id == 30858)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30858,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n常言道“舒服不如躺着，好吃不如饺子”。<I>圣魔饺子宴</I>现在还没开始呢，你待会再过来吧。这是给你的新春礼物，收下它，哈哈。");
        elseif( id == 30859)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30859,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n常言道“舒服不如躺着，好吃不如饺子”。<I>圣魔饺子宴</I>现在还没开始呢，你待会再过来吧。这是给你的新春礼物，收下它，哈哈。");
        elseif( id == 30860)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30860,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n常言道“舒服不如躺着，好吃不如饺子”。<I>圣魔饺子宴</I>现在还没开始呢，你待会再过来吧。这是给你的新春礼物，收下它，哈哈。");
        elseif( id == 30861)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30861,"<font=龙字体,0,19,20><color=0xfffcf157>沸腾的神锅：<color=0xffffffff>\\n常言道“舒服不如躺着，好吃不如饺子”。<I>圣魔饺子宴</I>现在还没开始呢，你待会再过来吧。这是给你的新春礼物，收下它，哈哈。");

       end

end

HDfeitengdeshenguo = {}
HDfeitengdeshenguo["OnTalk"] = OnTalk
HDfeitengdeshenguo["OnAcceptQuestTalk"] = OnAcceptQuestTalk
HDfeitengdeshenguo["OnCompleteQuestTalk"] = OnCompleteQuestTalk