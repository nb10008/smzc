local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600040);
	Quest.SetCompleteOptions(5600040);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n天地宫是充满危机的地方！");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30746)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30746,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30747)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30747,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30748)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30748,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30749)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30749,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30750)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30750,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30751)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30751,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30752)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30752,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30753)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30753,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n在天地宫中的天支线中，你可以找到一种叫做天晶石的东西，如果你能收集5个，那么我就可以帮你做成天之晶核。同时会得到天之咒的效果，只有等天之咒消失后，才可再次接取任务获得天之晶核。");
	elseif( id == 30754)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30754,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n在天地宫中的地支线中，你可以找到一种叫做地晶石的东西，如果你能收集2个，那么我就可以帮你做成地之晶核。同时会得到地之咒的效果，只有等地之咒消失后，才可再次接取任务获得地之晶核。");
	elseif( id == 30755)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30755,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30756)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30756,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30757)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30757,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30758)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30758,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30759)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30759,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30760)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30760,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30761)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30761,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n如果你能在天地宫中找到那些珍贵的地晶石和天晶石，那这些丰厚的奖励就全是你的了。");
	elseif( id == 30762)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30762,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30763)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30763,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30764)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30764,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30765)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30765,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30766)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30766,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30767)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30767,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30768)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30768,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30769)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30769,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30770)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30770,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
        elseif( id == 30771)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30771,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30772)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30772,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	elseif( id == 30773)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30773,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff>\\n新年嘉奖是需要用实力换取的，那么就努力吧！");
	end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30746)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30746,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30747)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30747,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30748)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30748,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30749)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30749,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30750)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30750,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30750)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30750,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30751)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30751,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30752)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30752,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30753)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30753,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n好的，这些足以做成一个天之晶核了，那么等你身上的天之咒消失后，再来收集天晶石换取天之晶核吧。");
	elseif( id == 30754)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30754,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n好的，这些足以做成一个地之晶核了，那么等你身上的地之咒消失后，再来收集地晶石换取地之晶核吧。");
	elseif( id == 30755)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30755,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30756)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30756,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30757)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30757,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30758)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30758,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30759)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30759,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30760)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30760,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30761)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30761,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n多多努力，快去收集天之晶核和地之晶核吧，还有好多好东西可以兑换呢。");
	elseif( id == 30762)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30762,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30763)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30763,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30764)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30764,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30765)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30765,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30766)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30766,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30767)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30767,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30768)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30768,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30769)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30769,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30770)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30770,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30771)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30771,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30772)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30772,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	elseif( id == 30773)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30773,"<font=龙字体,0,19,20><color=0xfffcf157>天地宫护卫：<color=0xffffffff><font=龙字体,0,17,20>\\n你看起来做的很轻松嘛。");
	end
end

Rhuwei = {}
Rhuwei["OnTalk"] = OnTalk
Rhuwei["OnScenarioTalk"] = OnScenarioTalk
Rhuwei["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rhuwei["OnCompleteQuestTalk"] = OnCompleteQuestTalk