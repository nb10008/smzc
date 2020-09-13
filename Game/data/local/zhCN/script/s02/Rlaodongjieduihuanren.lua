
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600057);
	Quest.SetCompleteOptions(5600057);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>劳动节兑换人：<color=0xffffffff><font=龙字体,0,17,20>\\n		爱生活，爱劳动，今天你劳动了吗？");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id >= 30371 and id <= 30375)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(id,"<font=龙字体,0,19,20><color=0xfffcf157>劳动节兑换人：<color=0xffffffff><font=龙字体,0,17,20>\\n		如果你集齐了我需要的所有物品，到时千万不要忘记来我这里换取奖励哦！");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id >= 30371 and id <= 30375)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(id,"<font=龙字体,0,19,20><color=0xfffcf157>劳动节兑换人：<color=0xffffffff><font=龙字体,0,17,20>\\n		这就是通过劳动所换来的奖励啦。");
	       end
end

Rlaodongjieduihuanren = {}
Rlaodongjieduihuanren["OnTalk"] = OnTalk
Rlaodongjieduihuanren["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rlaodongjieduihuanren["OnCompleteQuestTalk"] = OnCompleteQuestTalk
