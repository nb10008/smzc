--日常任务发布人

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610026);
	Quest.SetCompleteOptions(5610026);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>日常任务发布人：<color=0xffffffff>\\n    每个30级以上的玩家可以在我这里接取<C>3</C>次<M>魔界之果</M>任务和<C>3</C>次<M>杀戮密令</M>任务。");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30333)then
  	--  Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30333,"<font=龙字体,0,19,20><color=0xfffcf157>日常任务发布人：<color=0xffffffff>\\n    在西洲幻林之后的场景中你将有机会发现尚未成长的魔界之花，使用这些魔界花粉将会加快这些魔界之花的生长。当魔界之花长大之后，带几个它的果实回来给我。");
	elseif( id == 30334)then
  	--  Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30334,"<font=龙字体,0,19,20><color=0xfffcf157>日常任务发布人：<color=0xffffffff>\\n    我接到了一个不愿意透露姓名的贵族的委托，需要铲除一些他看起来不那么顺眼的东西。如果你想承接这个任务的话就把这个嗜血标记也一并收下吧，它可以帮助你很快地找到任务目标。");

	end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30333)then
  	--  Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30333,"<font=龙字体,0,19,20><color=0xfffcf157>日常任务发布人：<color=0xffffffff>\\n    你干得很好，这是你的奖励！");
	elseif( id == 30334)then
  	--  Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30334,"<font=龙字体,0,19,20><color=0xfffcf157>日常任务发布人：<color=0xffffffff>\\n    任务完成得非常好，合作愉快！");
	end
end

Rrcrwfbr = {}
Rrcrwfbr["OnTalk"] = OnTalk
Rrcrwfbr["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rrcrwfbr["OnCompleteQuestTalk"] = OnCompleteQuestTalk
