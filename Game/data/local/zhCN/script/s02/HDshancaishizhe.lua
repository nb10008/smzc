
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(6040024);	
	Quest.SetCompleteOptions(6040024);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n春节到了，我给三界带来了无数的财宝，诸位恭喜发财。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30862)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30862,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n龙腾三界，瑞雪丰年。本使者特意为你送来新春财富！看到散落遍地的<I><I>聚宝盆</I></I>了吗?快去调查一下吧，只要你身上沾有足够的财气，我就有办法替你招财进宝！");
        elseif( id == 30863)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30863,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n呵呵，<I>聚宝盆</I>可不是随便就能打开的。我可以给你它的钥匙，但是你得先帮我一个忙。这是一颗<I>摇钱树种子</I>，帮我照看它一下，然后击杀邪恶的财宝精灵，怎么样？");
        elseif( id == 30864)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30864,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n呵呵，<I>聚宝盆</I>可不是随便就能打开的。我可以给你它的钥匙，但是你得先帮我一个忙。这是一颗<I>摇钱树种子</I>，帮我照看它一下，然后击杀邪恶的财宝精灵，怎么样？");
        elseif( id == 30865)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30865,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n呵呵，<I>聚宝盆</I>可不是随便就能打开的。我可以给你它的钥匙，但是你得先帮我一个忙。这是一颗<I>摇钱树种子</I>，帮我照看它一下，然后击杀邪恶的财宝精灵,怎么样？");
        elseif( id == 30866)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30866,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n呵呵，<I>聚宝盆</I>可不是随便就能打开的。我可以给你它的钥匙，但是你得先帮我一个忙。这是一颗<I>摇钱树种子</I>，帮我照看它一下，然后击杀邪恶的财宝精灵,怎么样？");

	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30862)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30862,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n哈哈，年轻人，我看你财气外露，骨骼惊奇，必是练武奇才，将来维护宇宙正义与和平的重任就交给你了！送你一份宝物，不要嫌少哦。");
        elseif( id == 30863)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30863,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n哈哈，年轻人，果然是说到做到。钥匙给你，快打开<I>聚宝盆</I>，看看你得到的财宝吧！恭喜发财！");
        elseif( id == 30864)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30864,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n哈哈，年轻人，果然是说到做到。钥匙给你，快打开<I>聚宝盆</I>，看看你得到的财宝吧！恭喜发财！");
        elseif( id == 30865)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30865,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n哈哈，年轻人，果然是说到做到。钥匙给你，快打开<I>聚宝盆</I>，看看你得到的财宝吧！恭喜发财！");
        elseif( id == 30866)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30866,"<font=龙字体,0,19,20><color=0xfffcf157>善财使者：<color=0xffffffff>\\n哈哈，年轻人，果然是说到做到。钥匙给你，快打开<I>聚宝盆</I>，看看你得到的财宝吧！恭喜发财！");

       end

end

HDshancaishizhe = {}
HDshancaishizhe["OnTalk"] = OnTalk
HDshancaishizhe["OnAcceptQuestTalk"] = OnAcceptQuestTalk
HDshancaishizhe["OnCompleteQuestTalk"] = OnCompleteQuestTalk