
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510635);
	Quest.SetCompleteOptions(5510635);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n你知道黑暗学院吗？一个人去的话可是很危险的啊。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 29001)then
			Quest.CloseNPCTalk();
	Quest.AcceptQuestTalk(29001,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n在这奔雷裂谷，曾经有一座神学院，实际是负责教导你们这些信赖的天神。可现在这所学院被魔族占领了，我需要你们这些新生力量的帮助。");
	elseif( id == 29002)then
			Quest.CloseNPCTalk();
	Quest.AcceptQuestTalk(29002,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n第一位恶魔是个魔法傀儡，如果你闯入黑暗学院，应该很快就能看到这个家伙。请帮我将它消灭掉吧。");
	elseif( id == 29003)then
			Quest.CloseNPCTalk();
	Quest.AcceptQuestTalk(29003,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n这四个恶魔中有一个是十分凶恶的，那就是魔法恶徒加杰特，对付他的时候可以一定要小心啊。");
	elseif( id == 29004)then
			Quest.CloseNPCTalk();
	Quest.AcceptQuestTalk(29004,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n不要小看那个女恶魔，她可是魔法导师，要小心应付她。黑暗学院里的恶魔，都不是一般的家伙，尤其是看似软弱的那种。");
	elseif( id == 29005)then
			Quest.CloseNPCTalk();
	Quest.AcceptQuestTalk(29005,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n四个恶魔中，最强的就是恶灵骑士米尔卡。只有将它除掉，我们才能夺回神学院，让现在的黑暗学院得到净化，一切就靠你了。");
	elseif( id == 29014)then
			Quest.CloseNPCTalk();
	Quest.AcceptQuestTalk(29014,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n魔法恶徒特松嘉只是黑暗魔法学院的力量提升的开始，只有消灭他你才能继续去消灭黑暗魔法学院的敌人。");
	elseif ( id == 29015)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29015,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n学院中有一只叫做奥利弗的傀儡，它烧掉了学院中几近全部的书籍著作，干掉它，我对它痛恨至极！");
	elseif ( id == 29016)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29016,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n特松嘉是另一个凶悍的恶徒，它同伽杰特被称作双子恶魔，当遇见它的时候要格外小心。");

	elseif ( id == 29017)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29017,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n导师艾尔玛是恶魔大军为了加强对学院的控制而新派来的，我对他一无所知，请你为我收集有关于他的信息。");
	elseif ( id == 29018) then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29018,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n埃斯特斯，这是一个让人闻风丧胆的名字……不过，我听闻他在之前的战斗中受了重伤，现在正是去杀他的最好时机。");
	elseif ( id == 29023)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29023,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n黑暗学院里面一共四个恶魔，你需要将他们全部打败，才能证明你有资格成为天神。");
	elseif ( id == 29024)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29024,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n虽然我们打败了黑暗学院的四个恶魔，但是黑暗的气息并没有减弱，看来里面还有更加强大的力量存在。再去黑暗学院一趟，调查清楚其中的秘密，将黑暗学院的恶魔彻底铲除！");
	end

end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 29001)then
		  Quest.CloseNPCTalk();
	  Quest.CompleteQuestTalk(29001,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n这学院里一共有四个强大的恶魔，分别是魔法傀儡马力欧、魔法恶徒伽杰特、魔法导师赫达、恶灵女骑士 米尔卡，只有将他们全部消灭，才能将神学院夺回来。");
	elseif( id == 29002)then
		  Quest.CloseNPCTalk();
	  Quest.CompleteQuestTalk(29002,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n真是太感谢你了。");
	elseif( id == 29003)then
		  Quest.CloseNPCTalk();
	  Quest.CompleteQuestTalk(29003,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n新生力量，果然还是值得信任的。");
	elseif( id == 29004)then
		  Quest.CloseNPCTalk();
	  Quest.CompleteQuestTalk(29004,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n离胜利应该只差一步了，还要继续加油。");
	elseif( id == 29005)then
		  Quest.CloseNPCTalk();
	  Quest.CompleteQuestTalk(29005,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n真是太好了，这些恶魔终于被清除干净了。神学院又有希望了。");
	elseif( id == 29014)then
		  Quest.CloseNPCTalk();
	  Quest.CompleteQuestTalk(29014,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n看来你有能力继续在黑暗魔法学院中走下去。");
	elseif ( id == 29015)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(29015,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n谢谢你为我除掉了心中的恶气。");
	elseif ( id == 29016)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(29016,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n果然很强悍，你的英勇我会为你传诵。");
	elseif ( id == 29017)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(29017,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n感谢你为我获得这些消息。");
	elseif ( id == 29018)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(29018,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n看来强大的埃斯特斯也不是你的对手，在你身上，我看到了这个世界的希望。");
	elseif ( id == 29023)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(29023,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n很好，但是这只是开始，后面还有更大的挑战等着你！");
	elseif ( id == 29024)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(29024,"<font=龙字体,0,19,20><color=0xfffcf157>神学院长奥德利：<color=0xffffffff>\\n黑暗学院的问题总算解决了，辛苦你了！");
	end

end

Raodeli = {}
Raodeli["OnTalk"] = OnTalk
Raodeli["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Raodeli["OnCompleteQuestTalk"] = OnCompleteQuestTalk
