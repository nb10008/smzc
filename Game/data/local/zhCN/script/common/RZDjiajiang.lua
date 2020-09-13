local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010111);
	Quest.SetCompleteOptions(5010111);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>战斗嘉奖包领取人：<color=0xffffffff>\\n多多参加战斗多多奖励可拿！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30714)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30714,"<font=龙字体,0,19,20><color=0xfffcf157>战斗嘉奖包领取人：<color=0xffffffff>\\n战场结束时，拥有“战斗的荣耀”状态的就可以在此领取1个战斗嘉奖包。由于战斗的荣耀状态只存在1小时，所以战斗结束后请尽快领取。提示：只要参加战场战斗，且战场结束时未退出战场，则均可获得战斗的荣耀状态");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30714)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30714,"<font=龙字体,0,19,20><color=0xfffcf157>战斗嘉奖包领取人：<color=0xffffffff>\\n每天都可以来领取一个啊，不要忘记。");
	end

end

RZDjiajiang = {}
RZDjiajiang["OnTalk"] = OnTalk
RZDjiajiang["OnAcceptQuestTalk"] = OnAcceptQuestTalk
RZDjiajiang["OnCompleteQuestTalk"] = OnCompleteQuestTalk