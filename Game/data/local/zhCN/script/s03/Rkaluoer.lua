
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010032);
	Quest.SetCompleteOptions(5010032);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>卡洛儿：<color=0xffffffff>\\n        众神在上！我是不是被绑架到其他空间了？");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20104)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20104,"<font=龙字体,0,19,20><color=0xfffcf157>卡洛儿：<color=0xffffffff>\\n        这里到底发生了什么，总感觉附近有很邪恶的魔物影响了这里，好像幽幽湖的捕虫草也变成了食人花，这可太可怕了。就是那些奇琏花，如果你能取一些它们的花蕊给我，也许我能知道到底发生了什么事。");
        elseif( id == 20105)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20105,"<font=龙字体,0,19,20><color=0xfffcf157>卡洛儿：<color=0xffffffff>\\n          你猜我刚才看到了什么，是可怕的野人。跟我以前在这里见过的野人很不一样，眼神凶恶，还好没被他们发现。我想知道这些野人身上发生了什么事，帮我采集一点“样本”来吧。有时候这是必须要做的，将这些交给艾丽娜大人就好。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20103)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20103,"<font=龙字体,0,19,20><color=0xfffcf157>卡洛儿：<color=0xffffffff>\\n      真真真。。。真是太危险了，这些家伙怎么变得这么凶啊？上次我差点就被当点心吃了，还好有你帮忙。");
        elseif( id == 20104)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20104,"<font=龙字体,0,19,20><color=0xfffcf157>卡洛儿：<color=0xffffffff>\\n      这些花蕊。。。有一种邪恶的气息，我想我们是遇到了更大的麻烦了。");
       
     
       end


end

Rkaluoer = {}
Rkaluoer["OnTalk"] = OnTalk
Rkaluoer["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rkaluoer["OnCompleteQuestTalk"] = OnCompleteQuestTalk