--日常任务发布人

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610041);
	Quest.SetCompleteOptions(5610041);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>追风者卡瑟：<color=0xffffffff>\\n    好的坐骑能让你比你的敌人跑得更快。");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30335)then
  	--  Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30335,"<font=龙字体,0,19,20><color=0xfffcf157>追风者卡瑟：<color=0xffffffff>\\n    只有跟时间赛跑的人才更容易成功。使用我给你的这个<M>风之使者</M>，在规定的时间内到达目标的所在地，然后带着<M>逐风者印记</M>回来找我。记住，一个优秀的坐骑能够为你节省更多的时间。");
	end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30335)then
  	--  Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30335,"<font=龙字体,0,19,20><color=0xfffcf157>追风者卡瑟：<color=0xffffffff>\\n    你干得很好，这是你的奖励！");
	end
end

Rzhuifengzheks = {}
Rzhuifengzheks["OnTalk"] = OnTalk
Rzhuifengzheks["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rzhuifengzheks["OnCompleteQuestTalk"] = OnCompleteQuestTalk
