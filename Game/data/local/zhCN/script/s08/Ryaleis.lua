
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010208);
	Quest.SetCompleteOptions(5010208);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>亚雷斯：<color=0xffffffff>\\n哎！索尔那小子到底跑到哪儿去了？");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20626)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20626,"<font=龙字体,0,19,20><color=0xfffcf157>亚雷斯：<color=0xffffffff>\\n来得正好，先帮我个忙！这里有<C>1</C>份<I>溶血草</I>，再采来<C>1</C>份<I>玉昙花</I>，可以合成<C>1</C>份<I>乾元散</I>，之后告诉你个秘密哦！");
        elseif( id == 20627)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20627,"<font=龙字体,0,19,20><color=0xfffcf157>亚雷斯：<color=0xffffffff>\\n想听秘密先帮我教训一些<A>熔岩巨人</A>，我上回只是跟他们借了个火，他们就追杀了我一整天！");
        elseif( id == 20628)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20628,"<font=龙字体,0,19,20><color=0xfffcf157>亚雷斯：<color=0xffffffff>\\n其实也不是什么秘密，南面的<N>碧峰峡</N>游荡着很多<A>熔岩巨人</A>，最近竟然还出现了<color=0xffff0000>万石之魂<color=0xffffffff>，传说这种怪物只有天神和魔族联手才可制造出来，去找出原因吧！");
	elseif( id == 20629)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20629,"<font=龙字体,0,19,20><color=0xfffcf157>亚雷斯：<color=0xffffffff>\\n回你的<N>斗战天城</N>，问问你们的<D>斗神之王玛斯</D>吧，他或许会知道！");	
	
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20625)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20625,"<font=龙字体,0,19,20><color=0xfffcf157>亚雷斯：<color=0xffffffff>\\n索尔？那小子还活着？那天他突然一个人往南跑了，我还以为他被吃了……还好！");
        elseif( id == 20626)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20626,"<font=龙字体,0,19,20><color=0xfffcf157>亚雷斯：<color=0xffffffff>\\n这么快？好吧，秘密就是……");
	elseif( id == 20627)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20627,"<font=龙字体,0,19,20><color=0xfffcf157>亚雷斯：<color=0xffffffff>\\n哼哼！大仇得报，我得意的笑！");
	elseif( id == 20628)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20628,"<font=龙字体,0,19,20><color=0xfffcf157>亚雷斯：<color=0xffffffff>\\n这……这可是大发现！如果没猜错的话，竟然有天神和魔族联手！？");
        
	
       end

end

Ryaleis = {}
Ryaleis["OnTalk"] = OnTalk
Ryaleis["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryaleis["OnCompleteQuestTalk"] = OnCompleteQuestTalk