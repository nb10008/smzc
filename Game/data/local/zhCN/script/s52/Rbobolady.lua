local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600027);
	Quest.SetCompleteOptions(5600027);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff><font=龙字体,0,17,20>\\n通过制作巧克力后，可获得情人巧克力，情人巧克力是男性玩家所需要的物品；可在交易区与男性玩家交易获得情人玫瑰花，完成任务。每日可完整制作10次巧克力，并且可免费领取5个雕刻道具，雕刻道具还可以通过与玩家交易获得。");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30745)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30745,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff>\\n你知道吗，这个博博罗曼村可是盛产玫瑰花和巧克力的地方。想要获得这些奖励，就去好好劳动，与一个异性朋友互赠玫瑰和巧克力，没准还能获得一段佳缘呢。");
	elseif( id == 30779)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30779,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff>\\n你知道吗，这个博博罗曼村可是盛产玫瑰花和巧克力的地方。想要获得这些奖励，就去好好劳动，与一个异性朋友互赠玫瑰和巧克力，没准还能获得一段佳缘呢。");
	elseif( id == 30780)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30780,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff>\\n如果你能找一朵冰晶玫瑰，就拿来兑换罗曼徽章吧。这是一种很珍奇的玫瑰，当然是男生送给女生的最佳礼物。");
	elseif( id == 30781)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30781,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff>\\n如果你能找一朵燃炎玫瑰，就拿来兑换罗曼徽章吧。这是一种很珍奇的玫瑰，当然是男生送给女生的最佳礼物。");
	elseif( id == 30774)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30774,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff>\\n要劳动的话，是需要证明的。每天都可以来领取罗曼村劳动证。");
	elseif( id == 30786)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30786,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff>\\n送你一些雕刻道具，多做一些巧克力与大家分享");
        end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30745)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30745,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff><font=龙字体,0,17,20>\\n恩，看来你找到了呢，找到啥了？自己琢磨下呗。");
	elseif( id == 30779)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30779,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff><font=龙字体,0,17,20>\\n你的罗曼徽章，快拿去吧。");
	elseif( id == 30780)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30780,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff><font=龙字体,0,17,20>\\n你的罗曼徽章，快拿去吧。");
	elseif( id == 30781)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30781,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff><font=龙字体,0,17,20>\\n你的罗曼徽章，快拿去吧。");
	elseif( id == 30774)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30774,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff><font=龙字体,0,17,20>\\n拿着罗曼村劳动证快去劳动吧。");
	elseif( id == 30786)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30786,"<font=龙字体,0,19,20><color=0xfffcf157>博博小姐：<color=0xffffffff><font=龙字体,0,17,20>\\n快去制作巧克力吧。");
	end
end

Rbobolady = {}
Rbobolady["OnTalk"] = OnTalk
Rbobolady["OnScenarioTalk"] = OnScenarioTalk
Rbobolady["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rbobolady["OnCompleteQuestTalk"] = OnCompleteQuestTalk