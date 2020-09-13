
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010088);
	Quest.SetCompleteOptions(5010088);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n这里就是人界了，想要返回天界的话就只能用传送门才可以。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20302)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20302,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n    既然天神是刚刚下凡，还是先听听镇里人们的情况吧。");
        elseif( id == 20303)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20303,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n    天神大人，不是我们不相信您，不过以前也有天神来到此地，却被魔族打败了。如果可以，能不能现为我们展示下天神的能力。镇口有个<M>融魔炉</M>，里面一直封印着某个恶魔，天神试着打败他吧。");
        elseif( id == 20304)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20304,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n    在<N>天雪镇南边</N>有一座<N>天雪关</N>，<N>天雪关</N>连接着镇子与<N>白龙谷</N>，如今<N>白龙谷</N>被魔族侵入，十分危险。天神还是到那里去帮忙吧。");
        elseif( id == 20340)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20340,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n    从这里往东边走，你会看到天兵伊恩，他很需要你的帮助。");
	     elseif( id == 20345)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20345,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n    我看见了您的身手，可是就那么一只的话，我还是有些不放心。您去消灭那些遭到魔物影响的雪地猪，然后收集它们的肉给我。");
    elseif( id == 40067)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40067,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n    您来到天雪镇已经有一段时间了，应该对这附近的情况已经比较熟悉了吧？我想请您帮我的孙子收集一些小东西，这些东西靠我自己怕是很难弄到了……");

	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20301)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20301,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n        天神真的下凡了，我们祈求多日，终于显灵了。天神啊，一定要帮助我们这些凡人渡过难关。");
        elseif( id == 20302)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20302,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n        这魔族真是害人不浅，我们就靠天神您来消灭这些魔族了。");
        elseif( id == 20303)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20303,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n        果然是天神下凡，实力果然非凡");
        elseif( id == 20339)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20339,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n        不愧是天神，居然将魔冥岭都打翻了，厉害。");
	elseif( id == 20345)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20345,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n        竟然杀了那么多，这让我对您是无比崇拜啊！");
   elseif( id == 30336)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30336,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");
   elseif( id == 40067)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40067,"<font=龙字体,0,19,20><color=0xfffcf157>元冷：<color=0xffffffff>\\n        这是我给您的报酬，多谢！");
   
   end

end

Ryuanleng = {}
Ryuanleng["OnTalk"] = OnTalk
Ryuanleng["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryuanleng["OnCompleteQuestTalk"] = OnCompleteQuestTalk
