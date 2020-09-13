
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010097);
	Quest.SetCompleteOptions(5010097);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n学者的乐趣便是不断地研究研究再研究。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20331)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20331,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n    这里出现了好多残忍的猎人，他们好像只是为了乐趣在这里猎杀我们，你能不能帮助我们将这些家伙赶走。");
        elseif( id == 20334)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20334,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n    来看看这里，一个废墟，或者叫做遗迹。我珍贵的研究材料都被这附近的匪徒抢去了，需要有人帮我抢回来");
        elseif( id == 20335)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20335,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n    这些匪徒的首领就在附近，既然是除暴安良，就真的彻底点吧。");
        elseif( id == 20336)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20336,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n    再往东边走就是魔冥山了，那里现在可是被魔族占据着，我劝你先在这里提升下等级再去吧，现在力神戈登在那里调查，你也赶紧磨练一下去帮助他吧。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20335)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20335,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n        这下可以放心的研究了。");
        elseif( id == 20334)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20334,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n        这些匪徒就会欺压弱小，这次真是活该。");
        elseif( id == 20335)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20335,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n        这下可以放心的研究了。");
        elseif( id == 20333)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20333,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n        哟？新来的天神，太好了，我这里正需要人帮忙呢。");
	 elseif( id == 20349)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20349,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n        花种的不错，北洲这个地方很快就会变得绿色盎然。");
        elseif( id == 30339)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30339,"<font=龙字体,0,19,20><color=0xfffcf157>神学者杰夫：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");

       end

end

Rjeff = {}
Rjeff["OnTalk"] = OnTalk
Rjeff["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rjeff["OnCompleteQuestTalk"] = OnCompleteQuestTalk