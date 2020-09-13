
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010057);
	Quest.SetCompleteOptions(5010057);
	Quest.NPCTalk(0,0,"<color=0xfffcf157><font=龙字体,0,19,20>师门任务使者：<color=0xffffffff><font=龙字体,0,17,20>\\n40级以上的英雄，每天都可在我这里完成5次师门任务，获得经验奖励。\\n\\n1.如果昨天没有做满5次，今天可继续完成，但次数为两天一起算。\\n2.放弃前一天的任务，今日即可做满5次。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30401)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30401,"<font=龙字体,0,19,20><color=0xfffcf157>师门任务使者：<font=龙字体,0,17,20><color=0xffffffff>\\n你确定要接取师门任务吗？");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30401)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30401,"<font=龙字体,0,19,20><color=0xfffcf157>师门任务使者：<font=龙字体,0,17,20><color=0xffffffff>\\n多谢勇士如此不辞辛劳。");
	end

end

RShiMen = {}
RShiMen["OnTalk"] = OnTalk
RShiMen["OnAcceptQuestTalk"] = OnAcceptQuestTalk
RShiMen["OnCompleteQuestTalk"] = OnCompleteQuestTalk
