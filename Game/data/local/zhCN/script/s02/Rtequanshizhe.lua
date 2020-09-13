
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610110);
	Quest.SetCompleteOptions(5610110);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n每个角色都可以来我这里领取一个特权，角色创建日期越早，特权等级越高：\\n白金斗圣特权：2011年10月16日之前\\n黄金斗尊特权：2011年10月16日—2012年3月16日\\n白银斗神特权：2012年3月16日之后");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30920)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30920,"<font=龙字体,0,19,20><color=0xfffcf157>特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n来，让我看下你的角色创建日期。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30920)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30920,"<font=龙字体,0,19,20><color=0xfffcf157>特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n这是你的特权卡，收好了。");
       end


end

Rtequanshizhe = {}
Rtequanshizhe["OnTalk"] = OnTalk
Rtequanshizhe["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rtequanshizhe["OnCompleteQuestTalk"] = OnCompleteQuestTalk
