local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010226);
	Quest.SetCompleteOptions(5010226);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>新新雪子：<color=0xffffffff><font=龙字体,0,17,20>\\n在这个雪白的世界中，雪人一定是少不了的哦！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 50028)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50028,"<font=龙字体,0,19,20><color=0xfffcf157>新新雪子：<color=0xffffffff><font=龙字体,0,17,20>\\n雪山里有什么？雪人呗。有一种神奇的雪人叫做<A>哇啦哇啦雪人</A>，专门捣乱作怪，若果你遇到了<A>哇啦哇啦雪人</A>，一定要好好教训他们。");
        elseif( id == 50029)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50029,"<font=龙字体,0,19,20><color=0xfffcf157>新新雪子：<color=0xffffffff><font=龙字体,0,17,20>\\n雪山里有什么？雪人呗。有一种神奇的雪人叫做<A>哇啦哇啦雪人</A>，专门捣乱作怪，若果你遇到了<A>哇啦哇啦雪人</A>，一定要好好教训他们。");
        elseif( id == 50030)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50030,"<font=龙字体,0,19,20><color=0xfffcf157>新新雪子：<color=0xffffffff><font=龙字体,0,17,20>\\n雪山里有什么？雪人呗。有一种神奇的雪人叫做<A>哇啦哇啦雪人</A>，专门捣乱作怪，若果你遇到了<A>哇啦哇啦雪人</A>，一定要好好教训他们。");
        elseif( id == 50031)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50031,"<font=龙字体,0,19,20><color=0xfffcf157>新新雪子：<color=0xffffffff><font=龙字体,0,17,20>\\n雪山里有什么？雪人呗。有一种神奇的雪人叫做<A>哇啦哇啦雪人</A>，专门捣乱作怪，若果你遇到了<A>哇啦哇啦雪人</A>，一定要好好教训他们。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 50028)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50028,"<font=龙字体,0,19,20><color=0xfffcf157>新新雪子：<color=0xffffffff><font=龙字体,0,17,20>\\n恩，看来你还真的教训那些家伙了，很犀利呀！");
       elseif( id == 50029)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50029,"<font=龙字体,0,19,20><color=0xfffcf157>新新雪子：<color=0xffffffff><font=龙字体,0,17,20>\\n恩，看来你还真的教训那些家伙了，很犀利呀！");
        elseif( id == 50030)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50030,"<font=龙字体,0,19,20><color=0xfffcf157>新新雪子：<color=0xffffffff><font=龙字体,0,17,20>\\n恩，看来你还真的教训那些家伙了，很犀利呀！");
        elseif( id == 50031)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50031,"<font=龙字体,0,19,20><color=0xfffcf157>新新雪子：<color=0xffffffff><font=龙字体,0,17,20>\\n恩，看来你还真的教训那些家伙了，很犀利呀！");
      end

end

Rxuezi = {}
Rxuezi["OnTalk"] = OnTalk
Rxuezi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxuezi["OnCompleteQuestTalk"] = OnCompleteQuestTalk