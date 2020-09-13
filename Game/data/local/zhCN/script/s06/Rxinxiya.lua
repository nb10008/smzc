
local function OnTalk()
	Quest.ClearNPCTalkOptions();  --清除对话框选项
	Quest.SetAcceptOptions(5010162);
	Quest.SetCompleteOptions(5010162);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n这里真是太混乱了啊。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	 if( id == 20420)then
	  Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20420,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n这附近的蝶精变得十分危险，以前我来这里的时候它们都很和善的。我注意到它们身上好像有什么奇怪的东西，你能帮我调查一下么。");
	 elseif( id == 20421)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20421,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n如果这种妖气铃戴久了，就会融入到体内真正魔化，那些幻林魅惑者，已经看不到带有妖气铃了，肯定是已经魔化了。");
	 elseif( id == 20422)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20422,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n果然没错，这背后果然是三鬼教在捣鬼，那附近有很多三鬼的先知，直接杀他们个措手不及！");
	 elseif( id == 20419)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20419,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n三鬼教如此强大，我们需要增加自己的实力，当然你也要继续锻炼自己，然后你可以去找辛西娅，我想她会很高兴见到你的。");
	 elseif( id == 20423)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20423,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n果然，这些先知居然是从太平坡那边来的，看来三鬼教的总部就在那里了。这是我的推断。在此之前，先将等级提升至64级吧。");
	 elseif( id == 20424)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20424,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n我的伙伴汉娜，现在正在太平坡寻找线索，你去那里找她吧。");
	 end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	 if( id == 20419)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20419,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n见到你真是太高兴了。");
	 elseif( id == 20420)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20420,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n这个东西我以前听说过，是可以将人或者动物变得十分危险的妖器。");
	 elseif( id == 20421)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20421,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n难道这也和三鬼教有关系么。");
	 elseif( id == 20422)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20422,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n这些先知的头领呢？怎么还不出现。");
	 elseif( id == 20423)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20423,"<font=龙字体,0,19,20><color=0xfffcf157>辛西娅：<color=0xffffffff>\\n来说说我们的计划吧。");
	end

end

Rxinxiya = {}
Rxinxiya["OnTalk"] = OnTalk
Rxinxiya["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxinxiya["OnCompleteQuestTalk"] = OnCompleteQuestTalk