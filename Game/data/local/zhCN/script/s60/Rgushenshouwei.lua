local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600076);
	Quest.SetCompleteOptions(5600076);
	Quest.SetNPCTalkOption( EOT_GodEvilRepute,0,0, "技能及魂晶" );-- 第二个参数控制: 0:God;1: Evil
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      这里是古神陵墓，里面沉睡着在那场神魔之战中牺牲的战士的灵魂！");
end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30833)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30833,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      数千年前，魔界越来越狂妄，不仅不信仰神，还不断地诋毁神的荣耀。虽然神是宽容的，但是我们也不允许那帮卑贱的种族如此猖狂。我们派出了最强的战士，去净化他们。那场战争持续了千年，最后连他们的魔王都被我们的神封印在魔窟中。仁慈的神让我们的战士都安息在这座神墓里，等待神的法力恢复后就将他们复活。\\n      你能来到这里，是你的机缘，也说明你有资格拥有神魂晶，里面拥有神赐予的力量。");
	end
	if( id == 30835)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30835,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      千年前的那场神魔之战，虽然我们清除了大部分的魔族，但是还有一些余孽存在，残留的魔力一直是他们的希望，你去收集一些回来吧，让他们彻底绝望！");
	end
	if( id == 30837)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30837,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      千年前的那场神魔之战，我们的战士很多都战死沙场，但是有些战士的灵魂却未能回归神的怀抱，依然徘徊在战场，希望你能将他们的灵魂带回神的身边！");
	end
	if( id == 30839)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30839,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      几千年过去了，这里也在慢慢衰落，你去找些材料来修葺一下吧，不能让我们的战士失去最后的安息之处！");
	end
	if( id == 30841)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30841,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      几千年过去了，支撑古神陵墓的魔力结晶已经消耗了大半，再不及时补充，古神陵墓的结界就会变弱甚至消失，去收集一些魔力水晶吧，为了我们之高无上的神！");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30833)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30833,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      这个神魂晶你拿去吧，希望你用它将神的光辉播散到每个角落！");
	end
	if( id == 30835)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30835,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      愿神的光辉永远照耀你！");
	end
	if( id == 30837)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30837,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      愿神的光辉永远照耀你！");
	end
	if( id == 30839)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30839,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      愿神的光辉永远照耀你！");
	end
	if( id == 30841)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30841,"<font=龙字体,0,19,20><color=0xfffcf157>古神陵墓守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      愿神的光辉永远照耀你！");
	end
end

Rgushenshouwei = {}
Rgushenshouwei["OnTalk"] = OnTalk
Rgushenshouwei["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rgushenshouwei["OnCompleteQuestTalk"] = OnCompleteQuestTalk
