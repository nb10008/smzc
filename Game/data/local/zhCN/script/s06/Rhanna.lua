
local function OnTalk()
	Quest.ClearNPCTalkOptions();  --清除对话框选项
	Quest.SetAcceptOptions(5010163);
	Quest.SetCompleteOptions(5010163);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>汉娜：<color=0xffffffff>\\n三鬼教真是无恶不作！连三岁孩子的糖果都要抢。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	 if( id == 20425)then
	  Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20425,"<font=龙字体,0,19,20><color=0xfffcf157>汉娜：<color=0xffffffff>\\n好了，我们直接开始吧，这里全部都是三鬼教的人，准备进攻吧。");
	 elseif( id == 20426)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20426,"<font=龙字体,0,19,20><color=0xfffcf157>汉娜：<color=0xffffffff>\\n这里还有一些危险的五鬼，这个任务比较危险，你要小心一点。");
	 elseif( id == 20427)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20427,"<font=龙字体,0,19,20><color=0xfffcf157>汉娜：<color=0xffffffff>\\n三鬼教的教主，是个崇拜魔族的邪人，现在就去把这家伙消灭掉，让这个邪教消失吧。");
	 elseif( id == 20428)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20428,"<font=龙字体,0,19,20><color=0xfffcf157>汉娜：<color=0xffffffff>\\n西洲幻林的事情告一段落， 等你等级提升至65级之后，就可以回到斗战天城，向斗神之王复命了。");

	 end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	 if( id == 20424)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20424,"<font=龙字体,0,19,20><color=0xfffcf157>汉娜：<color=0xffffffff>\\n来得正好！");
	 elseif( id == 20425)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20425,"<font=龙字体,0,19,20><color=0xfffcf157>汉娜：<color=0xffffffff>\\n就是这样！");
	 elseif( id == 20426)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20426,"<font=龙字体,0,19,20><color=0xfffcf157>汉娜：<color=0xffffffff>\\n那接下来就直捣黄龙吧。");
	 elseif( id == 20427)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20427,"<font=龙字体,0,19,20><color=0xfffcf157>汉娜：<color=0xffffffff>\\n终于将邪教铲除了，真是不易啊。");

	end

end

Rhanna = {}
Rhanna["OnTalk"] = OnTalk
Rhanna["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rhanna["OnCompleteQuestTalk"] = OnCompleteQuestTalk