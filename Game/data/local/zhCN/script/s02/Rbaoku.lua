
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610113);
	Quest.SetCompleteOptions(5610113);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>英雄试炼委托人：<color=0xffffffff>\\n英雄宝库是一个充满危险的地方，无数的勇者倒在了探宝的路上，你能够走完探宝之路么？");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30921)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30921,"<font=龙字体,0,19,20><color=0xfffcf157>英雄试炼委托人：<color=0xffffffff>\\n英雄宝库是一个充满危险的地方，无数的勇者倒在了探宝的路上，你能够走完探宝之路么？");
		    elseif( id == 30922)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30922,"<font=龙字体,0,19,20><color=0xfffcf157>英雄试炼委托人：<color=0xffffffff>\\n英雄宝库是一个充满危险的地方，无数的勇者倒在了探宝的路上，你能够走完探宝之路么？");
       end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30921)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30921,"<font=龙字体,0,19,20><color=0xfffcf157>英雄试炼委托人：<color=0xffffffff>\\n我不敢相信你真的完成了，你用你的实力证明了自己");
        elseif( id == 30922)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30922,"<font=龙字体,0,19,20><color=0xfffcf157>英雄试炼委托人：<color=0xffffffff>\\n拿去吧，一路小心");
       end

end

Rbaoku = {}
Rbaoku["OnTalk"] = OnTalk
Rbaoku["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rbaoku["OnCompleteQuestTalk"] = OnCompleteQuestTalk
