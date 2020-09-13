local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600029);
	Quest.SetCompleteOptions(5600029);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff><font=龙字体,0,17,20>\\n通过照顾玫瑰花后，可获得情人玫瑰花，情人玫瑰花是女性玩家所需要的物品；可在交易区与女性玩家交易获得情人巧克力，完成任务。每日可完整照顾10次玫瑰花，并且可免费领取5个杀虫剂，杀虫剂还可以通过与玩家交易获得。");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30744)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30744,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff>\\n你知道吗，这个博博罗曼村可是盛产玫瑰花和巧克力的地方。想要获得这些奖励，就去好好劳动，与一个异性朋友互赠玫瑰和巧克力，没准还能获得一段佳缘呢。");
	elseif( id == 30778)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30778,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff>\\n你知道吗，这个博博罗曼村可是盛产玫瑰花和巧克力的地方。想要获得这些奖励，就去好好劳动，与一个异性朋友互赠玫瑰和巧克力，没准还能获得一段佳缘呢。");
	elseif( id == 30782)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30782,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff>\\n如果你能找一块零度巧克力，就拿来兑换罗曼徽章吧。这是一种很珍奇的巧克力，当然是女生送给男生的最佳礼物。");
	elseif( id == 30783)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30783,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff>\\n如果你能找一块精灵巧克力，就拿来兑换罗曼徽章吧。这是一种很珍奇的巧克力，当然是女生送给男生的最佳礼物。");
	elseif( id == 30784)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30784,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff>\\n要劳动的话，是需要证明的。每天都可以来领取罗曼村劳动证。");
	elseif( id == 30785)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30785,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff>\\n送你一些除虫剂，要好好照顾玫瑰花。");
        end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30744)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30744,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff><font=龙字体,0,17,20>\\n恩，看来你找到了呢，找到啥了？自己琢磨下呗。");
	elseif( id == 30778)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30778,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff><font=龙字体,0,17,20>\\n这个稀有的物品要珍惜啊。");
	elseif( id == 30782)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30782,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff><font=龙字体,0,17,20>\\n你的罗曼徽章，快拿去吧。");
	elseif( id == 30783)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30783,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff><font=龙字体,0,17,20>\\n你的罗曼徽章，快拿去吧。");
	elseif( id == 30784)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30784,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff><font=龙字体,0,17,20>\\n拿着罗曼村劳动证快去劳动吧。");
	elseif( id == 30785)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30785,"<font=龙字体,0,19,20><color=0xfffcf157>博博先生：<color=0xffffffff><font=龙字体,0,17,20>\\n不要浪费这些除虫剂。");
	end
end

Rmrbobo = {}
Rmrbobo["OnTalk"] = OnTalk
Rmrbobo["OnScenarioTalk"] = OnScenarioTalk
Rmrbobo["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rmrbobo["OnCompleteQuestTalk"] = OnCompleteQuestTalk