
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010090);
	Quest.SetCompleteOptions(5010090);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>雪吉：<color=0xffffffff><font=龙字体,0,17,20>\\n这么危险的地方，还是少来的好。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20309)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20309,"<font=龙字体,0,19,20><color=0xfffcf157>雪吉：<color=0xffffffff><font=龙字体,0,17,20>\\n我的姐妹被困在洞中了，里面有很可怕的野兽，你快去救救她吧。");
        elseif( id == 20313)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20313,"<font=龙字体,0,19,20><color=0xfffcf157>雪吉：<color=0xffffffff><font=龙字体,0,17,20>\\n刚才镇里来消息了，希望天神可以在等级到达<C>36</C>级的时候回到<N>天雪镇</N>，<D>天元</D>有事相求。");
	

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20308)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20308,"<font=龙字体,0,19,20><color=0xfffcf157>雪吉：<color=0xffffffff><font=龙字体,0,17,20>\\n看来新的天神就是你吧，太好了。");
        elseif( id == 20312)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20312,"<font=龙字体,0,19,20><color=0xfffcf157>雪吉：<color=0xffffffff><font=龙字体,0,17,20>\\n看来大家都平安无事。");
	
       end

end

Rxueji = {}
Rxueji["OnTalk"] = OnTalk
Rxueji["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxueji["OnCompleteQuestTalk"] = OnCompleteQuestTalk