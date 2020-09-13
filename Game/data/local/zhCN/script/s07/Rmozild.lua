
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010204);
	Quest.SetCompleteOptions(5010204);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n狂拳轰天开，弹腿裂地绽！你想试试我的风魔体术吗？");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20521)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20521,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n不管你是谁！先把沼泽里的怪物驯服再说！要是一个人做不到就叫些帮手，如果这样还做不到就别来见我了！");
        elseif( id == 20522)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20522,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n数日前，我发现似乎有天神悄悄降临文理山，之后原神教非但未被天神灭掉，反倒多出了一层幻术结界，一些原神教徒竟然还掌握了神火的用法。");
        elseif( id == 20523)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20523,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n<D>幻神艾丽娜</D>和<D>火神加尔</D>！很抱歉破坏了他们在你心中正义、无私的形象，但我敢拿我七十二魔子之疾风魔子雷丁之名保证，我所说的句句属实。");
	elseif( id == 20524)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20524,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n那边有一块神魔大战时我军用于追踪天神的<M>搜神星盘</M>，一试便可呈现出天神的幻象，不过你要小心，这样做一定会惊动他们。");
	elseif( id == 20525)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20525,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n不过我要提醒你，灭除了原神教便是正式与天神为敌，你可有这样的觉悟？你真的愿意自此背弃自己的种族？");
	elseif( id == 20526)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20526,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n去吧，文理山之事查清后去当面质问你们的<D>斗神之王玛斯</D>，如果他还敢说实话……");
	
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20520)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20520,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n天神？！左德让你来帮我？哼！别添乱就好！");
        elseif( id == 20521)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20521,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n好！好本事！难得左德没看错人！那我就把我发现的秘密告诉你吧！");
	elseif( id == 20522)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20522,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n说出来你或许不信，这些天神中我隐约见到两个老熟人，当年大战之时没少与他们交手。");
	elseif( id == 20523)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20523,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n好吧好吧！先别忙着生气，就算我手中没有证据，你也可以自己去查探一番吧？");
	elseif( id == 20524)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20524,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n事到如今你怎么看？还认为我在骗你？好吧，如果你还在怀疑就自己去独闯文理山，揪出原神教在这边的首领亲自问问吧！");
	elseif( id == 20525)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20525,"<font=龙字体,0,19,20><color=0xfffcf157>魔子雷丁：<color=0xffffffff>\\n你心中的正义让我感动！好吧！如果神界从此薄待于你，我魔族定会以最高贵的礼节，欣然接纳你的到来！我的朋友！");
       
       end

end

Rmozild = {}
Rmozild["OnTalk"] = OnTalk
Rmozild["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rmozild["OnCompleteQuestTalk"] = OnCompleteQuestTalk