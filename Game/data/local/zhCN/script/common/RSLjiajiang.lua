local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010115);
	Quest.SetCompleteOptions(5010115);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>胜利嘉奖人：<color=0xffffffff>\\n这个是给获得战场胜利的公会及同盟公会特别的嘉奖！！  \\n当战场结束时，胜利方的会长和副会长将获得“高级官员的荣耀”状态，其他成员将获得“胜利的荣耀”状态  \\n拥有“高级官员的荣耀”，可兑换5个高阶统治勋章，10个统治勋章  \\n拥有“胜利的荣耀”，可兑换10个统治勋章  \\n注意：“高级官员的荣耀”和“胜利的荣耀”状态在下线或离开场景后均会消失，所以当获得胜利后，请第一时间到此领取奖励！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30715)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30715,"<font=龙字体,0,19,20><color=0xfffcf157>胜利嘉奖人：<color=0xffffffff>\\n只有战斗胜利者们的统治者才可以得到这项奖励！");
	elseif( id == 30716)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30716,"<font=龙字体,0,19,20><color=0xfffcf157>胜利嘉奖人：<color=0xffffffff>\\n“胜利的荣耀”给你带来了奖励。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30715)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30715,"<font=龙字体,0,19,20><color=0xfffcf157>胜利嘉奖人：<color=0xffffffff>\\n快去准备下一场战斗吧。");
	elseif( id == 30716)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30716,"<font=龙字体,0,19,20><color=0xfffcf157>胜利嘉奖人：<color=0xffffffff>\\n只有不断的努力才可以获得更多的奖励。");
	end

end

RSLjiajiang = {}
RSLjiajiang["OnTalk"] = OnTalk
RSLjiajiang["OnAcceptQuestTalk"] = OnAcceptQuestTalk
RSLjiajiang["OnCompleteQuestTalk"] = OnCompleteQuestTalk