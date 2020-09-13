
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600065);
	Quest.SetCompleteOptions(5600065);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>食神杰斯:<color=0xffffffff>\\n作为食神，我不可能错过每一个重要的节日，现在正是东方端午佳节，粽子是一种不可估量的美食。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30787)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30787,"<font=龙字体,0,19,20><color=0xfffcf157>食神杰斯:<color=0xffffffff>\\n完蛋了，完蛋了，今年吃不到粽子了，伟大的战士你愿意帮我弄点糯米来么？我可以用美味的粽子和你交换。不过别现在找我要粽子，我是巧妇难为无米之炊啊");
	elseif( id == 30788)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30788,"<font=龙字体,0,19,20><color=0xfffcf157>食神杰斯:<color=0xffffffff>\\n真是太糟糕了，我实在是太忙了，我又不能去陪我的父亲【斗神阿拉斯】了，我的父亲非常喜欢吃粽子，你愿意帮我把粽子送到我父亲那里吗？快点回来，我不会亏待你的。对了我的父亲住在【天神冢】");
	elseif( id == 30790)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30790,"<font=龙字体,0,19,20><color=0xfffcf157>食神杰斯:<color=0xffffffff>\\n这个水晶上有着很神秘的力量，我需要一些媒介来破除上面的禁锢魔法，给我找55个原矿来，我就能够破除禁制。放心，我是很和善的人，没有暴力倾向。");
	elseif( id == 30791)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30791,"<font=龙字体,0,19,20><color=0xfffcf157>食神杰斯:<color=0xffffffff>\\n给我一个端午礼盒和一张命运卷，我会给你一个五彩端午礼盒。五彩端午礼盒可以开出更多的珍惜道具哦。");

	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30787)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30787,"<font=龙字体,0,19,20><color=0xfffcf157>食神杰斯:<color=0xffffffff>\\n有了这些材料，一定可以吃到美味的粽子，太感谢你了。");
	elseif( id == 30789)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30789,"<font=龙字体,0,19,20><color=0xfffcf157>食神杰斯:<color=0xffffffff>\\n这一个节日中也充满阴谋，真是让人头痛。");
	elseif( id == 30790)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30790,"<font=龙字体,0,19,20><color=0xfffcf157>食神杰斯:<color=0xffffffff>\\n这联络水晶很珍贵的，你要保管好。");
    elseif( id == 30791)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30791,"<font=龙字体,0,19,20><color=0xfffcf157>食神杰斯:<color=0xffffffff>\\n这个五彩端午礼盒你拿去吧。");
	end


end

Rshishenjiesi = {}
Rshishenjiesi["OnTalk"] = OnTalk
Rshishenjiesi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rshishenjiesi["OnCompleteQuestTalk"] = OnCompleteQuestTalk
