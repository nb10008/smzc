
local function OnTalk()
	Quest.ClearNPCTalkOptions();  --清除对话框选项
	Quest.SetAcceptOptions(5511016);
	Quest.SetCompleteOptions(5511016);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n西洲幻林是一个充满奇幻的地方，同时也存在很多危险。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 20430)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20430,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n都是那只幻林妖媚惹得祸，要不然也不会出现这么多的三鬼教教众，快去消灭她吧。");
	elseif( id == 20432)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20432,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n三鬼教中的那些人都做过很多邪恶的事情，我需要你收集他们的罪证，作为消灭它们的关键。");
	elseif( id == 20433)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20433,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n幻林中存在一种花，它产生的毒素可以让正常人着魔，从而进入魔道，快去铲除它们吧，");
	elseif( id == 20434)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20434,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n三鬼教就够麻烦的了，现在又多出来很多巨熊来捣乱。");
	elseif( id == 20435)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20435,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n为什么那些人会成为死士呢？这个问题困扰我很久了，请你帮我解决它。");
	elseif( id == 20436)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(20436,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n那个幻林屠夫伤我多少同胞，我是何其心痛，你来得正好，快去消灭他吧。");

	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 20429)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20429,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n销毁这些传教书，看看他们还怎么传教！");
	elseif( id == 20431)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20431,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n做得不错，如此一来三鬼教的壮大就会很难了。");
	elseif( id == 20432)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20432,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n有了这些罪证就知道如何消灭他们了。");
	elseif( id == 20433)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20433,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n它们生命力很是旺盛，要很多人才能把它们全部都消除。");
	 elseif( id == 20434)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20434,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n总是那么多的麻烦要消除，作为一个神，很不容易啊！");
	 elseif( id == 20435)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20435,"<font=龙字体,0,19,20><color=0xfffcf157>戴布拉：<color=0xffffffff>\\n原来三鬼教是那么残酷。");
	
	
	end
end

Rdebra = {}
Rdebra["OnTalk"] = OnTalk
Rdebra["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rdebra["OnCompleteQuestTalk"] = OnCompleteQuestTalk