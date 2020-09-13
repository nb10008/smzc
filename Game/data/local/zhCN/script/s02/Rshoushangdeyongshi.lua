local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5516025);
	Quest.SetCompleteOptions(5516025);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      见……见到……见到他们，我的灵魂都会震颤……");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30820)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30820,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      赫迪鲁斯曾为了圣魔大陆的繁荣和稳定与邪恶魔神沟通，在多次沟通中被控制心灵，倒戈相向。现在我们已放弃挽救他迷失的灵魂……只有杀掉他……杀掉他，也是你义不容辞的责任。");
        elseif( id == 30821)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30821,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      魔神军团首席智囊希特鲁熟悉各种黑暗法术，并且阴险诡诈，如果能将其消灭，必能一挫魔神军的锐气。");
        elseif( id == 30822)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30822,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      华恩利偶尔会出现在西洲幻林深处，他拥有强大的自然之力，一定要记住，杀他之前，要先扫除他周围的鹰犬。");
	elseif( id == 30823)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30823,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      斗神之王的弟弟阿玛恩一直在追求超越哥哥的能力，为此他摒弃一切……");
	elseif( id == 30824)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30824,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      当你拿到了光明祝福、黑暗祈祷、自然之源和力量祝福以后，来我这里可以换取宝贝。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30820)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30820,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      圣魔大陆都在为这一刻欢呼！");
        elseif( id == 30821)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30821,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      你是圣魔大陆的骄傲！");
	elseif( id == 30822)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30822,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      人民会铭记你的壮举！");
	 elseif( id == 30823)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30823,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      他还是遗憾地倒下了……");
   elseif( id == 30824)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30824,"<font=龙字体,0,19,20><color=0xfffcf157>受伤的勇士：<color=0xffffffff><font=龙字体,0,17,20>\\n      这就是你追求的宝物吧。");
       end

end

Rshoushangdeyongshi = {}
Rshoushangdeyongshi["OnTalk"] = OnTalk
Rshoushangdeyongshi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rshoushangdeyongshi["OnCompleteQuestTalk"] = OnCompleteQuestTalk