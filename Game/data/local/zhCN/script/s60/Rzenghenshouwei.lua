local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600077);
	Quest.SetCompleteOptions(5600077);
	Quest.SetNPCTalkOption( EOT_GodEvilRepute,1,0, "技能及魂晶" )
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      憎恨魔窟里埋葬的，都是为自由和信念而战的勇士！");
end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30834)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30834,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      数千年前，我们魔界在魔王的带领下，具备了可以与天界抗衡的力量。我们追求自由，崇尚力量。我们想按照自己的方式去生存，将自己的命运掌握在自己手中。可是天界那帮虚伪的家伙，却认为他们的地位受到了危险，发起了战争。我们当然不怕他们，那场战争持续了一千年，最终我们的魔王战胜了他们的天神，却因为能力消耗过度，跟我们的勇士一起沉睡在了魔窟里。总有一天，魔神会再次醒来，带领我们走向真正的自由。\\n      既然你来到了这里，说明你有足够的实力，我这里有一个魔魂晶，里面蕴含我们魔族的力量。");
	end
	if( id == 30836)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30836,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      千年前的那场神魔之战，虽然我们战胜了神族，却未能完全消灭他们，战场里的残留的神力始终是个威胁，你去夺一些过来吧！");
	end
	if( id == 30838)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30838,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      千年前的那场神魔之战，我们牺牲了很多勇士，虽然大部分都回到魔窟中沉睡，但是有些勇士的灵魂却依然徘徊在战场，希望你能将他们的灵魂带回魔窟里！");
	end
	if( id == 30840)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30840,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      几千年过去了，魔窟也在慢慢破败，你去找些材料来修葺一下吧，魔神终有一天会醒来的！");
	end
	if( id == 30842)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30842,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      几千年过去了，支撑憎恨魔窟的魔力水晶消耗太多了，再过不久恐怕就要耗尽了，去收集一些吧。魔神终有一天会醒来的！");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30834)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30834,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      带着魔魂晶，去追求你的自由吧，魔族的力量将与你同在！");
	end
	if( id == 30836)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30836,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      魔族的力量将与你同在！");
	end
	if( id == 30838)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30838,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      魔族的力量将与你同在！");
	end
	if( id == 30840)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30840,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      魔族的力量将与你同在！");
	end
	if( id == 30842)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30842,"<font=龙字体,0,19,20><color=0xfffcf157>憎恨魔窟守卫：<color=0xffffffff><font=龙字体,0,17,20>\\n      魔族的力量将与你同在！");
	end
end

Rzenghenshouwei = {}
Rzenghenshouwei["OnTalk"] = OnTalk
Rzenghenshouwei["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rzenghenshouwei["OnCompleteQuestTalk"] = OnCompleteQuestTalk
