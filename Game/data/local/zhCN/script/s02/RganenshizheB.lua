--龙神使者2号

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610083);
	Quest.SetCompleteOptions(5610083);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>龙神使者：<color=0xffffffff>\\n    活动期间，每天晚上20：30到22：10，50级以上的玩家都可以来我这里领取经验，每天最多可以领取5次。但是每个时间段只能领取一次，错过了就要等第二天了！\\n    第一次经验领取时间是20：30——20：50\\n    第二次经验领取时间是20：50——21：10\\n    第三次经验领取时间是21：10——21：30\\n    第四次经验领取时间是21：30——21：50\\n    第五次经验领取时间是21：50——22：10");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30884)then
  	--  Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30884,"<font=龙字体,0,19,20><color=0xfffcf157>龙神使者：<color=0xffffffff>\\n    现在是第二次经验领取时间，赶紧领取吧，21：10之后就进入下个经验领取时间了！");
	end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30884)then
  	--  Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30884,"<font=龙字体,0,19,20><color=0xfffcf157>龙神使者：<color=0xffffffff>\\n    好了，经验给你了，记得感谢我！");
	end
end

RganenshizheB = {}
RganenshizheB["OnTalk"] = OnTalk
RganenshizheB["OnAcceptQuestTalk"] = OnAcceptQuestTalk
RganenshizheB["OnCompleteQuestTalk"] = OnCompleteQuestTalk
