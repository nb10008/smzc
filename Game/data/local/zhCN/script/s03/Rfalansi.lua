
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010074);
	Quest.SetCompleteOptions(5010074);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>法兰斯：<color=0xffffffff>\\n这里好像出了一些问题，还真是难办啊。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20107)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20107,"<font=龙字体,0,19,20><color=0xfffcf157>法兰斯：<color=0xffffffff>\\n        终于有人来帮忙了，真是太好了。我就想在这植物中找到些线索，我发现<N>花海之根</N>的食人龙变得十分危险，我们要先来清理一下。");
        elseif( id == 20108)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20108,"<font=龙字体,0,19,20><color=0xfffcf157>法兰斯：<color=0xffffffff>\\n        这附近也有凶暴的野人，我们同样需要一些“样本”，交给艾丽娜大人，这些野人之前你也见识过了，要格外小心他们。");
        elseif( id == 20118)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20118,"<font=龙字体,0,19,20><color=0xfffcf157>法兰斯：<color=0xffffffff>\\n        到七花之根去看一看吧，那里的问题十分严重。我需要知道那里到底出了什么问题，你可以到较高的地势去观察一下。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20106)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20106,"<font=龙字体,0,19,20><color=0xfffcf157>法兰斯：<color=0xffffffff>\\n        原来是艾丽娜大人让你来的，那好吧。");
        elseif( id == 20107)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20107,"<font=龙字体,0,19,20><color=0xfffcf157>法兰斯：<color=0xffffffff>\\n        多亏有你，否则这还不知道要到什么时候才能弄完。");
        elseif( id == 20118)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20118,"<font=龙字体,0,19,20><color=0xfffcf157>法兰斯：<color=0xffffffff>\\n        你带来的信息，确实十分重要。");
       end

end

Rfalansi = {}
Rfalansi["OnTalk"] = OnTalk
Rfalansi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rfalansi["OnCompleteQuestTalk"] = OnCompleteQuestTalk